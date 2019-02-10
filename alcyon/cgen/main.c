/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*
 *	ALCYON C Compiler for the Motorola 68000 - Code Generator
 *
 *	Called from c68:
 *
 *		c168 icode link asm
 *
 *	icode:		parsed intermediate code with some assembly code
 *              preceded by left parens.
 *
 *	link:		contains the procedure link and movem instructions.
 *
 *	asm:	    output assembler code for as68.
 *
 *	The basic structure of the code generator is as follows:
 *
 *	main				- main routine
 *		readicode		- code generation driven by intermediate code
 *
 */

#include "cgen.h"
#include <stdlib.h>


char *opap;
short stacksize;
char exprarea[EXPSIZE];
short onepass;
short bol;
short lineno;
short errcnt;

short dflag; /* bool: include line numbers in assembly output */
short cflag; /* bool: debug code reader */
short eflag; /* bool: debug skeleton expansion */
short mflag; /* bool: debug skeleton match */
short oflag; /* bool: debug operators */
short gflag; /* bool: generate line labels for cdb */
short m68010; /* bool: generate code for m68010 */
short lflag = 1; /* bool: assume long address variables */
short aesflag; /* bool: hack for TOS 1.x AES */


short nextlabel = 10000;

struct tnode null;


char source[PATHSIZE] = "";

/* io buffer declaration */
static FILE *ifil, *lfil, *ofil;

static char const program_name[] = "c168";





/* readshort - reads an integer value from intermediate code */
static short readshort(NOTHING)
{
	register short c;
	register short i;

	i = 0;
	for (;;)
	{
		switch (c = getc(ifil))
		{
		case '\r':
			break;
		case '.':
		case '\n':
			return i;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			i <<= 4;
			i += (c - '0');
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			i <<= 4;
			i += (c - ('a' - 10));
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			i <<= 4;
			i += (c - ('A' - 10));
			break;

		default:
			error(_("intermediate code error reading short - %d ($%x)"), c, c);
			break;
		}
	}
}


/* readlong - reads a long value from intermediate code */
static int32_t readlong(NOTHING)
{
	union {
		struct words w;
		int32_t l;
	} l;
	register unsigned short w1, w2;
	register short c, onedot;

	w1 = 0;
	w2 = 0;
	onedot = 0;
	for (;;)
	{
		switch (c = getc(ifil))
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			w2 <<= 4;
			w2 += (c - '0');
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			w2 <<= 4;
			w2 += (c - ('a' - 10));
			break;

		case 'A':						/* sw Hex in upper case as well... */
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			w2 <<= 4;
			w2 += (c - ('A' - 10));
			break;

		case '\r':
			break;

		case '.':
			if (!onedot++)
			{
				w1 = w2;
				w2 = 0;
				continue;
			}
			/* fall through */
		case '\n':
			if (onedot)
			{
				l.w.hiword = w1;
				l.w.loword = w2;
				return l.l;
			}
			/* fall through */
		default:
			error(_("intermediate code error reading long - %d ($%x)"), c, c);
			break;
		}
	}
}


/* readsym - read a symbol from intermediate code */
static char *readsym(P(char *) sym)
PP(char *sym;)
{
	register short i, c;
	register char *s;

	for (i = SSIZE, s = sym; (c = getc(ifil)) != '\n';)
	{
		if (c < 0)
			break;
		if (c != '\r' && --i >= 0)
			*s++ = c;
	}
	if (i > 0)
		*s = '\0';
	return sym;
}


/* readtree - recursive intermediate code tree read */
static struct tnode *readtree(NOTHING)						/* returns ptr to expression tree */
{
	register short op, type, sc;
	register struct tnode *tp, *rtp;
	char sym[SSIZE];

	if ((op = readshort()) <= 0)
		return NULL;
	type = readshort();
	switch (op)
	{
	case SYMBOL:
		if ((sc = readshort()) == EXTERNAL)
			tp = cenalloc(type, sc, readsym(sym));
		else
			tp = snalloc(type, sc, (int32_t) readshort(), 0, 0);
		break;

	case CINT:
		tp = cnalloc(type, readshort());
		break;

	case CLONG:
		tp = lcnalloc(type, readlong());
		break;

	case CFLOAT:
		tp = fpcnalloc(type, readlong());
		break;

	case IFGOTO:
	case BFIELD:
		sc = readshort();
		if ((tp = readtree()) != NULL)
			tp = tnalloc(op, type, sc, 0, tp, &null);
		break;

	default:
		if (BINOP(op))
		{
			if (!(tp = readtree()))
				return 0;
			if (!(rtp = readtree()))
				return 0;
			tp = tnalloc(op, type, 0, 0, tp, rtp);
		} else if ((tp = readtree()) != NULL)
		{
			tp = tnalloc(op, type, 0, 0, tp, &null);
		}
		break;
	}
	return tp;
}


/* readfid - read source filename out of intermediate file */
static VOID readfid(NOTHING)
{
	register char *p;
	register int c;
	
	p = &source[0];
	while ((c = getc(ifil)) > 0 && c != '\n')
		if (c != '\r')
			*p++ = c;
	*p = 0;
}


/*
 * readicode - read intermediate code and dispatch output
 * This copies assembler lines beginning with '(' to assembler
 * output and builds trees starting with '.' line.
 */
static VOID readicode(NOTHING)
{
	register short c;
	register struct tnode *tp;

	while ((c = getc(ifil)) > 0)
	{
		switch (c)
		{
		case '.':
			lineno = readshort();
			readfid();
			opap = exprarea;
			if ((tp = readtree()) != NULL)
			{
				PUTEXPR(cflag, "readicode", tp);
				switch (tp->t_op)
				{
				case INIT:
					outinit(tp->t_left);
					break;

				case CFORREG:
					outcforreg(tp->t_left);
					break;

				case IFGOTO:
					outifgoto(tp->t_left, tp->t_type, tp->t_su);
					break;

				default:
					outexpr(tp);
					break;
				}
			} else
			{
				outline();
			}
			break;

		case '(':
			while ((c = getc(ifil)) > 0 && c != '\n')
				oputchar(c);
			if (c > 0)
				oputchar(c);
			break;

		case '%':
			while ((c = getc(ifil)) > 0 && c != '\n')
				;						/* skip over carriage return */
			while ((c = getc(lfil)) > 0 && c != '%')
				oputchar(c);
			if (c < 0)
				fatal(_("early termination of link file"));
			break;

		default:
			error(_("intermediate code error reading tree %d ($%x)"), c, c);
			break;
		}
	}
}


static VOID endit(P(int) stat)
PP(int stat;)
{
	if (ifil)
		fclose(ifil);
	if (ofil)
		fclose(ofil);
	if (lfil)
		fclose(lfil);
	exit(stat);
}


/* error - output an error message */
VOID error(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	errcnt++;
	if (lineno != 0)
		fprintf(stderr, "\"%s\", ** %d: ", source, lineno);
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


/* warning - output a warning message */
VOID warning(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	if (lineno != 0)
		fprintf(stderr, "\"%s\", ** %d: (warning) ", source, lineno);
	else
		fprintf(stderr, "(warning) ");
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


/* fatal - output error message and die */
VOID fatal(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	errcnt++;
	if (lineno != 0)
		fprintf(stderr, "\"%s\", ** %d: ", source, lineno);
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	endit(EXIT_FAILURE);
}


/*
 * oputchar - special version
 *		This allows the use of printf for error messages, debugging
 *		output and normal output.
 */
VOID oputchar(P(char) c)
PP(char c;)
{
	if (dflag > 1)
		fputc(c, stdout);
	if (c != '\r')
		fputc(c, ofil);						/* put to assembler file */
}


VOID oprintf(P(const char *) string _va_alist)
PP(const char *string;)
_va_dcl
{
	va_list args;
	
	va_start(args, string);
	if (dflag > 1)
		vfprintf(stdout, string, args);
	vfprintf(ofil, string, args);
	va_end(args);
}



/* usage - output usage message */
static VOID usage(NOTHING)
{
	error(_("usage: %s icode link asm [-DTacemov]"), program_name);
	error(_("options:"));
	error(_("    -L    assume long (32bit) address variables (default)"));
	error(_("    -a    assume short (16bit) address variables"));
	error(_("    -g    generate line labels for cdb"));
	error(_("    -d    include line numbers in assembly output"));
	error(_("    -t    generate code for 68010"));
#ifdef DEBUG
	error(_("    -c    debug code generator"));
	error(_("    -e    debug skeleton expansion"));
	error(_("    -m    debug skeleton match"));
	error(_("    -o    debug operators"));
#endif
	endit(EXIT_FAILURE);
}


/* main - main routine, handles arguments and files */
int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char *q;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (argc < 4)
		usage();
	if ((ifil = fopen(argv[1], "r")) == NULL)
		fatal(_("can't open %s"), argv[1]);
	if ((lfil = fopen(argv[2], "r")) == NULL)
		fatal(_("can't open %s"), argv[2]);
	if ((ofil = fopen(argv[3], "w")) == NULL)
		fatal(_("can't create %s"), argv[3]);

	for (argc -= 4, argv += 4; argc--;)
	{
		q = *argv++;
		if (*q++ != '-')
			usage();
		for (;;)
		{
			switch (*q++)
			{
			case 'a':					/* alter ego of the '-L' flag */
				lflag = 0;
				continue;

			case 'f':
				/* fflag++; */
				continue;

			case 'g':					/* generate line labels for cdb */
				gflag++;
				continue;

			case 'D':
			case 'd':
				dflag++;
				continue;

#ifdef DEBUG
			case 'c':
				cflag++;
				continue;

			case 'e':
				eflag++;
				continue;

			case 'm':
				mflag++;
				continue;

			case 'o':
				oflag++;
				continue;
#endif

			case 'L':					/* OBSOLETE */
			case 'l':
				lflag++;
				continue;

			case 'T':					/* generates code for the 68010 */
			case 't':
				m68010++;
				continue;

			case 'A':
				aesflag++;
				continue;

			case '\0':
				break;

			default:
				usage();
				break;
			}
			break;
		}
	}

	readicode();
	endit(errcnt != 0);
	return EXIT_SUCCESS;
}
