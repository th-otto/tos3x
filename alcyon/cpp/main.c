/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

int status = 0;
int pflag;
int Cflag;
int Eflag;
int asflag;
int aesflag;
char *source;			/* preprocessor source file */
char dest[MAXPSIZE];		/* preprocessor destination file */
FILE *inbuf, *outbuf;

static int ndefs;
static char stdincl[MAXPSIZE];
static char compat_incl[MAXPSIZE];


static int v_compat;
char const program_name[] = "cp68";



/* if source t.c dest <= t.i */
static VOID make_intermediate(NOTHING)
{
	register char *d, *s;
	register int ndx;

	s = source;
	while ((ndx = strindex(s, FILESEP)) >= 0)
		s += ndx + 1;
	while ((ndx = strindex(s, FILESEP2)) >= 0)
		s += ndx + 1;
	for (d = dest; (*d++ = *s++) != 0; )
		;
	*(d - 2) = 'i';						/* overwrite termination character */
}


static VOID make_stdincl(P(const char *) argv0)
PP(const char *argv0;)
{										/* if source t.c dest <= t.i */
	register char *d;
	register const char *s;
	register const char *end;
	register int ndx;
	
	s = argv0;
	while ((ndx = strindex(s, FILESEP)) >= 0)
		s += ndx + 1;
	while ((ndx = strindex(s, FILESEP2)) >= 0)
		s += ndx + 1;
	if (s == argv0)
	{
		d = stdincl;
		strcpy(d, ".");
	} else
	{
		s--;
		ndx = (__intptr_t)s - (__intptr_t)argv0;
		if (ndx > 4 && (s[-4] == FILESEP || s[-4] == FILESEP2) && strncmp(s - 3, "bin", 3) == 0)
			s -= 4;
		end = s;
		s = argv0;
		for (d = stdincl; s != end; )
			*d++ = *s++;
		*d = '\0';
	}
	if (strcmp(stdincl, ".") == 0)
		strcpy(stdincl, "../include");
	else
		strcpy(d, "/include");
}


static VOID usage(NOTHING)
{
	printf("usage: %s [-C] [-P] [-E] [-D] [-I] [-6] [-7] [-3] source [dest]\n", program_name);
}


/*
 * main - main routine for c68 Compiler system
 *      Handles the C68 arguments.  For each C file given, the macro
 *      pre-processor is called, then the parser, code generator and
 *      assember are fexec'd.  The loader arguments are collected and
 *      the loader is fexec'd.
 */
int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(register char **argv;)
{
	register char *arg;
	const char *argv0;
	register int c, i, x;
	
#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	argv0 = NULL;
	if (argc >= 1)
		argv0 = *argv++;
	if (argv0 == NULL || *argv0 == 0)
	{
		strcpy(stdincl, "/usr/include");
	} else
	{
		make_stdincl(argv0);
	}
	
	if (argc < 2)
	{									/* cpp source */
		usage();
		exit(EXIT_FAILURE);
	}

	for (; --argc > 0 && **argv == '-';)
	{									/* process arguments */
		arg = *argv++;
		arg++;
		for (i = 0; (c = *arg++) != 0;)
		{
			switch (c)
			{
			case 'D':
				if (*arg == '\0')
				{
					if (--argc <= 0)
					{
						usage();
						exit(EXIT_FAILURE);
					}
					arg = *argv++;
				}
				defs[ndefs].ptr = arg;
				if ((x = strindex(arg, '=')) >= 0)
				{
					defs[ndefs++].value = (arg + x + 1);
					arg[x] = 0;			/* get rid of value */
				} else
				{
					defs[ndefs++].value = 0;
				}
				i++;
				break;

			case 'I':
				if (*arg == '\0')
				{
					if (--argc <= 0)
					{
						usage();
						exit(EXIT_FAILURE);
					}
					arg = *argv++;
				}
				incl[nincl++] = arg;
				i++;
				break;

			case 'C':					/* Leave comments in... */
				Cflag++;
			case 'E':					/* Preprocessor to stdout */
				Eflag++;
				continue;

			case 'P':					/* preprocessor pass only */
				pflag++;
				continue;

			case '6':					/* v6 compatibility */
				v_compat = 6;
				continue;

			case '7':					/* v7 compatibility */
				v_compat = 7;
				continue;

			case '3':					/* s3 compatibility */
				v_compat = 3;
				continue;

			case '5':					/* s5 compatiblity */
				v_compat = 5;
				continue;

			case 'A':
				aesflag++;
				continue;

			default:
				usage();
				exit(EXIT_FAILURE);
			}
			if (i)
				break;
		}
	}

	if (argc != 1 && argc != 2)
	{									/* source [dest] */
		usage();
		exit(EXIT_FAILURE);
	}

	switch (v_compat)
	{
	case 6:
		strcat(strcpy(compat_incl, stdincl), "/v6");
		incl[nincl++] = compat_incl;
		break;
	case 7:
		strcat(strcpy(compat_incl, stdincl), "/v7");
		incl[nincl++] = compat_incl;
		break;
	case 3:
		strcat(strcpy(compat_incl, stdincl), "/sys3");
		incl[nincl++] = compat_incl;
		break;
	case 5:
		strcat(strcpy(compat_incl, stdincl), "/sys5");
		incl[nincl++] = compat_incl;
		break;
	}
	incl[nincl++] = stdincl;
	
	source = *argv++;
	outbuf = stdout;
	if (!Eflag)
	{
		if (argc == 2)					/* destination file specified */
			strcpy(dest, *argv);
		else
			make_intermediate();
	} else
	{
		strcpy(dest, "-");
	}
	
	asflag = source[strlen(source) - 1] == 's';
	domacro(ndefs);
	return status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}


/*
 * cexit - exit from C compiler driver
 *      This deletes any existing temps and exits with the error status.
 */
VOID cexit(NOTHING)
{
	exit(status == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}


/*
 * itoa - integer to ASCII conversion
 *      Converts integer to ASCII string, handles '-'.
 */
VOID myitoa(P(int) n, P(char *) s, P(int) w)
PP(int n;)									/* number to convert */
PP(char *s;)								/* resulting string */
PP(int w;)									/* minimum width of string */
{
	register char *tp;
	register int sign, i;
	char temp[20];

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	tp = &temp[0];
	do
	{
		i++;
		*tp++ = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
	{
		i++;
		*tp++ = '-';
	}
	while (--w >= i)					/* pad on left with blanks */
		*s++ = ' ';
	while (--i >= 0)					/* move chars reversed */
		*s++ = *--tp;
	*s = '\0';
}


/*
 * strindex - find the index of a character in a string
 *      This is identical to Software Tools index.
 * returns index of c in str or -1
 */
int strindex(P(const char *) str, P(char) chr)
PP(const char *str;)
PP(char chr;)
{
	register const char *s;
	register int i;

	for (s = str, i = 0; *s != '\0'; i++)
	{
		if (*s++ == chr)
			return i;
	}
	return -1;
}


int atoi(P(const char *) as)
PP(const char *as;)
{
	register int n, sign;
	register const char *s;

	s = as;
	while (*s == ' ' || *s == '\n' || *s == '\t')
		s++;
	sign = 1;
	if (*s == '+' || *s == '-')
		sign = (*s++ == '+') ? 1 : -1;
	for (n = 0; *s >= '0' && *s <= '9'; s++)
		n = (n * 10) + (*s - '0');
	return sign * n;
}
