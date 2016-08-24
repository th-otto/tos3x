/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

char *version = "@(#)main.c	1.6	12/28/83";

#include "preproc.h"
#include <ctype.h>
#define NARGS   64
#define FLAGS "[-C] [-P] [-E] [-D] [-I] [-6] [-7] [-3]"
#define USAGE "usage: %s %s source [dest]\n"

#ifdef REGULUS							/*sw not MC68000 */
char *v6incl = "/usr/include/v6";
char *v7incl = "/usr/include/v7";
char *s3incl = "/usr/include/sys3";
char *ucbincl = "/usr/include/ucb";
char *s5incl = "/usr/include/sys5";
#endif

#ifdef UNIX
char *v6incl = "/usr/include/c68/v6";
char *v7incl = "/usr/include/c68/v7";
char *s3incl = "/usr/include/c68/sys3";
char *ucbincl = "/usr/include/c68/ucb";
char *s5incl = "/usr/include/c68/sys5";
#endif

#ifdef VMS
# ifdef DECC
char *v6incl = "v6";
char *v7incl = "v7";
char *s3incl = "sys3";
char *ucbincl = "ucb";
char *s5incl = "sys5";
# else
char *v6incl = "lib:";
char *v7incl = "lib:";
char *s3incl = "lib:";
char *ucbincl = "lib:";
char *s5incl = "lib:";
# endif
#endif

#ifdef CPM
char *v6incl = "";
char *v7incl = "";
char *s3incl = "";
char *ucbincl = "";
char *s5incl = "";
#endif

#ifdef GEMDOS
char *v6incl = "";
char *v7incl = "";
char *s3incl = "";
char *ucbincl = "";
char *s5incl = "";
#endif

char *stdincl;							/*sw define it so i can clobber it ... */
char *incl[NINCL];
int ndefs, nincl;

int status = 0;

VOID make_intermediate PROTO((NOTHING));
VOID cexit PROTO((NOTHING)) __attribute__((noreturn));



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
	register char *arg, *calledby;
	register int c, i, j, x;
	register char *p;

	calledby = *argv++;
#ifdef CPM
	calledby = "cp68";
#endif
#ifdef GEMDOS
	calledby = "cp68";
#endif
	if (argc < 2)
	{									/* cpp source */
		printf(USAGE, calledby, FLAGS);
		exit(-1);
	}

	/* process arguments */
	for (;;)
	{
		 if (--argc <= 0 || **argv != '-')
		 	break;
#ifdef __ALCYON__
		*(arg = *argv++); /* BUG: value computed is not used */
#else
		arg = *argv++;
#endif
		arg++;
		for (i = 0; ;)
		{
		cont:;
			if (!(c = *arg++))
				break;
			switch (c)
			{
			case 'D':
			case 'd':					/*sw Make case-insensitive */
				defs[ndefs].ptr = arg;
				if ((x = strindex(arg, '=')) != -1)
				{
					defs[ndefs++].value = (arg + x + 1);
					arg[x] = 0;			/*get rid of value */
				} else
				{
					defs[ndefs++].value = 0;
				}
				i++;
#ifdef CPM								/*sw Up-case the argument */
				p = defs[ndefs - 1].ptr;
				while (*p)
				{
					*p = toupper(*p);
					p++;
				}
#endif
#ifdef GEMDOS
				p = defs[ndefs - 1].ptr;
				for (;;)
				{
					if (*p == '\0')
						break;
					*p = toupper((__uint8_t)*p);
					p++;
				}
#endif
#ifdef	WHITESM							/*sw Up-case the argument */
				p = defs[ndefs - 1].ptr;
				while (*p)
				{
					*p = toupper(*p);
					p++;
				}
#endif
				break;

			case 'I':
			case 'i':
#ifdef CPM
				stdincl = *argv++;		/*sw Compatible with our past... */
				--argc;
#endif
#ifdef GEMDOS
				/* WTF: that only allows one -I argument */
				stdincl = *argv++;
				--argc;
#endif
#ifdef WHITESM
				stdincl = *argv++;		/*sw Compatible with our past... */
				--argc;
#endif
#ifdef	REGULUS
				incl[nincl++] = arg;
				i++;
#endif
#ifdef	UNIX
				incl[nincl++] = arg;
				i++;
#endif
#ifdef	VERSADOS						/*sw ?Will this work? */
				incl[nincl++] = arg;
				i++;
#endif
				break;

			case 'C':					/* [vlh] 4.2 Leave comments in... */
			case 'c':
				Cflag++;
				/* sw "break" here???? */
			case 'E':					/* [vlh] 4.0 Preprocessor to stdout */
			case 'e':
				Eflag++;
				goto cont;

			case 'P':					/* preprocessor pass only */
			case 'p':
				pflag++;
				goto cont;

			case '6':					/* [vlh] 3.4 v6 compatibility */
				incl[nincl++] = v6incl;
				goto cont;

			case '7':					/* [vlh] 3.4 v7 compatibility */
				incl[nincl++] = v7incl;
				goto cont;

			case '3':					/* [vlh] 3.4 s3 compatibility */
				incl[nincl++] = s3incl;
				goto cont;

			case '4':					/* [vlh] 4.3 ucb compatibility */
				incl[nincl++] = ucbincl;
				goto cont;

			case '5':					/* [vlh] 3.4 s5 compatiblity */
				incl[nincl++] = s5incl;
				goto cont;

			default:
				printf(USAGE, calledby, FLAGS);
				exit(-1);

			}							/* end of case statement */
			if (i)
				break;
		}								/* end of for statement */
	}									/* end of for statement */

	if (argc > 2)
	{									/* source [dest] */
		printf(USAGE, calledby, FLAGS);
		exit(-1);
	}
	source = *argv++;
	if (!Eflag)
	{
		if (argc == 2)					/* destination file specified */
			strcpy(dest, *argv);
		else
			make_intermediate();
	}
	
	asflag = source[(int)strlen(source) - 1] == 's';
	domacro(ndefs);
	UNUSED(j);
	cexit();
}


/*
 * cexit - exit from C compiler driver
 *      This deletes any existing temps and exits with the error status.
 */
VOID cexit(NOTHING)
{
	exit(status);
}


/*
 * itoa - integer to ASCII conversion
 *      Converts integer to ASCII string, handles '-'.
 */
VOID itoa(P(int) n, P(char *) s, P(int) w)
PP(int n;)									/* number to convert */
PP(char *s;)								/* resulting string */
PP(int w;)									/* minimum width of string */
{
	register char *tp;
	register int sign, i;
	char temp[6];

	sign = n;
	if (sign < 0)
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
	for (;;)					/* pad on left with blanks */
	{
		if (--w < i)
			break;
		*s++ = ' ';
	}
	for (;;)					/* move chars reversed */
	{
		if (--i < 0)
			break;
		*s++ = *--tp;
	}
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

	for (s = str, i = 0; ; i++)
	{
		if (*s == '\0')
			break;
		if (*s++ == chr)
			return i;
	}
#ifdef __ALCYON__
	asm("moveq.l   #-1,d0");
	asm("nop");
#else
	return -1;
#endif
}


int atoi(P(const char *) as)
PP(const char *as;)
{
	register int n, sign;
	register const char *s;

	s = as;
	for (;;)
	{
		if (*s != ' ' && *s != '\n' && *s != '\t')
			break;
		s++;
	}
	sign = 1;
	if (*s == '+' || *s == '-')
		sign = (*s++ == '+') ? 1 : -1;
	for (n = 0; ; s++)
	{
		if (*s < '0' || *s > '9')
			break;
		n = (n * 10) + (*s - '0');
	}
#ifdef __ALCYON__
	asm("move.w d6,d0");
	asm("muls.w d7,d0");
	asm("nop");
#else
	return sign * n;
#endif
}


/* if source t.c dest <= t.i */
VOID make_intermediate(NOTHING)
{
	register char *d, *s;
	register int ndx;

	s = source;
	for (;;)
	{
		if ((ndx = strindex(s, '/')) == -1)
			break;
		s += ndx + 1;
	}
	for (d = dest; ;)
	{
		if ((*d++ = *s++) == '\0')
			break;
	}
	*(d - 2) = 'i';						/* overwrite termination character */
}


/* cputc - put a character to a file descriptor (used by error) */
VOID cputc(P(char) c, P(int) fn)
PP(char c;)
PP(int fn;)
{
#ifdef VERSADOS
	versaputchar(c);
#else
	if (fn == STDERR)
		write(STDERR, &c, 1);
	else
		putchar(c);
#endif
}


#ifdef VERSADOS
extern struct iob *fout;
VOID putchar(P(int) c)
PP(int c);
{
	versaputchar(c);
}
#endif


int v6flush(P(FILE *) v6buf)
PP(FILE *v6buf;)
{
	register int i;

	i = BLEN - v6buf->cc;
	v6buf->cc = BLEN;
	v6buf->cp = &(v6buf->cbuf[0]);
	if (write(fileno(v6buf), v6buf->cp, i) != i)
		return EOF;
#ifdef __ALCYON__
	asm("clr.w     d0");
	asm("nop");
#else
	return 0;
#endif
}

#ifdef VERSADOS

struct iob versfout = { 1, BLEN, &versfout.cbuf[0] };

int versaputchar(c)
char c;
{
	if (c == '\n')
	{									/* end of line */
		if (versaflush())				/* write one line */
			return EOF;
		return c;
	}

	/* buffered output */
	if (versfout.cc <= 0)
	{
		versfout.cp = &(versfout.cbuf[0]);
		if (write(versfout.fd, versfout.cp, BLEN) != BLEN)
			return EOF;
		versfout.cc = BLEN;
	}
	*(versfout.cp)++ = c;
	versfout.cc--;
	return c;
}


int versaflush()
{
	register short size;
	register short fildes;

	if ((size = (BLEN - versfout.cc)) == 0)
		return 0;
	versfout.cc = BLEN;
	versfout.cp = &(versfout.cbuf[0]);
	fildes = (versfout.fd <= STDERR) ? 6 : versfout.fd;
	if (write(fildes, versfout.cp, size) < 0)
		return -1;
	return 0;
}

#endif




/*
 * these are here only to be able to compile & link with non-Alcyon compilers;
 * running the program will not work
 */

#ifndef __ALCYON__
int _open PROTO((const char *fname, int mode, int binary));
int _open PROTO((const char *fname, int mode, int binary)) { return -1; }
int xcreat PROTO((const char *fname, int mode, int binary)) { return -1; }
VOID xwritefail(NOTHING) { abort(); }
char const __atab[256];
#endif
