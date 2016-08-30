/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)main.c	1.7	12/28/83
*/

char *version = "@(#)main.c	1.7 12/28/83";

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


FILE ibuf, lbuf, obuf;
short m68010;
char *opap;
short stacksize;
char exprarea[EXPSIZE];
short onepass;
short dflag;
short cflag;
short bol;
short eflag;
short fflag;
short gflag;
short lineno;
short mflag;
short oflag;
short errcnt;


short nextlabel = 10000;

char null[] = "";  /* BUG: should be tnode */

short lflag = 1;

char source[PATHSIZE] = "";


VOID readicode PROTO((NOTHING));
struct tnode *readtree PROTO((NOTHING));
VOID readfid PROTO((NOTHING));
short readshort PROTO((NOTHING));
long readlong PROTO((NOTHING));
char *readsym PROTO((char *sym));
VOID usage PROTO((const char *calledby));
int x1flush PROTO((FILE *mybuf));




/* main - main routine, handles arguments and files */
int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char *q;
	register const char *calledby;

	calledby = *argv++;
	calledby = "c168";

	if (argc < 4)
		usage(calledby);
	if (fopen(*argv, &ibuf, 0) < 0)
		ferror("can't open %s", *argv);
	if (fopen(*++argv, &lbuf, 0) < 0)
		ferror("can't open %s", *argv);
	if (fcreat(*++argv, &obuf, 0) < 0)
		ferror("can't create %s", *argv);

	for (argc -= 4; argc--;)
	{
		q = *++argv;
		if (*q++ != '-')
			usage(calledby);
		while (1)
		{
			switch (*q++)
			{
			case 'a':					/* alter ego of the '-L' flag */
				lflag = 0;
				continue;

			case 'f':
				fflag++;
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

			case '\0':
				break;

			default:
				usage(calledby);
			}
			break;
		}
	}

	readicode();
	x1flush(&obuf);
	exit(errcnt != 0);
}


/*
 * readicode - read intermediate code and dispatch output
 * This copies assembler lines beginning with '(' to assembler
 * output and builds trees starting with '.' line.
 */
VOID readicode(NOTHING)
{
	register short c;
	register struct tnode *tp;

	while ((c = getc(&ibuf)) > 0)
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
				outline();
			break;

		case '(':
			while ((c = getc(&ibuf)) != '\n')
				putchar(c);
			putchar(c);
			break;

		case '%':
			while ((c = getc(&ibuf)) != '\n')
				;						/* skip over carriage return */
			while ((c = getc(&lbuf)) != '%' && c != EOF)
				putchar(c);
			if (c == EOF)
				ferror("early termination of link file");
			break;

		default:
			error("intermediate code error %c,%d", c, c);
			break;
		}
	}
}


/* readtree - recursive intermediate code tree read */
struct tnode *readtree(NOTHING)						/* returns ptr to expression tree */
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
			tp = snalloc(type, sc, (long) readshort(), 0, 0);
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
			tp = tnalloc(op, type, sc, 0, tp, (struct tnode *)null); /* BUG */
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
			tp = tnalloc(op, type, 0, 0, tp, (struct tnode *)null); /* BUG */
		}
		break;
	}
	return tp;
}


/* readfid - read source filename out of intermediate file */
VOID readfid(NOTHING)
{
	register char *p;

	p = &source[0];
	while ((*p = getc(&ibuf)) != '\n')
		p++;
	*p = 0;
}


/* readshort - reads an integer value from intermediate code */
short readshort(NOTHING)
{
	register short c;
	register short i;

	i = 0;
	while (1)
	{
		switch (c = getc(&ibuf))
		{
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
			error("intermediate code error - %c,%d", c, c);
		}
	}
}


/* readlong - reads a long value from intermediate code */
long readlong(NOTHING)
{
	union {
		struct words w;
		long l;
	} l;
	register unsigned short w1, w2;
	register short c, onedot;

#ifndef __ALCYON__
	w1 = 0;
#endif
	w2 = 0;
	onedot = 0;
	while (1)
	{
		switch (c = getc(&ibuf))
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

		case '.':
			if (!onedot++)
			{
				w1 = w2;
				w2 = 0;
				continue;
			}
		case '\n':
			if (onedot)
			{
				l.w.hiword = w1;
				l.w.loword = w2;
				return l.l;
			}
		default:
			error("intermediate code error - %c,%d", c, c);
		}
	}
}

/* readsym - read a symbol from intermediate code */
char *readsym(P(char *) sym)
PP(char *sym;)
{
	register short i, c;
	register char *s;

	for (i = SSIZE, s = sym; (c = getc(&ibuf)) != '\n';)
		if (--i >= 0)
			*s++ = c;
	if (i > 0)
		*s = '\0';
	return sym;
}


/* error - output an error message */
#ifdef __ALCYON__
VOID error(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	errcnt++;
	if (lineno != 0)
		fprintf(stderr, "\"%s\", ** %d: ", source, lineno);
	fprintf(stderr, s, x1, x2, x3, x4, x5, x6);
	fprintf(stderr, "\n");
}
#else
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
#endif


#ifdef __ALCYON__
/* warning - output a warning message */
VOID warning(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	if (lineno != 0)
		fprintf(stderr, "\"%s\", ** %d: (warning) ", source, lineno);
	else
		fprintf(stderr, "(warning) ");
	fprintf(stderr, s, x1, x2, x3, x4, x5, x6);
	fprintf(stderr, "\n");
}
#else
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
#endif


/* ferror - output error message and die */
#ifdef __ALCYON__
VOID ferror(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	error(s, x1, x2, x3, x4, x5, x6);
	exit(EXIT_FAILURE);
}
#else
VOID ferror(P(const char *) s _va_alist)
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
	exit(EXIT_FAILURE);
}
#endif


/* tnalloc - allocate binary expression tree node */
/*	returns ptr to node made. */
struct tnode *tnalloc(P(int) op, P(int) type, P(int) info, P(int) dummy, P(struct tnode *) left, P(struct tnode *) right)
PP(int op;)									/* operator */
PP(int type;)								/* resultant node type */
PP(int info;)								/* info field */
PP(int dummy;)								/* dummy field - used to match pass1 args */
PP(struct tnode *left;)						/* left sub-tree */
PP(struct tnode *right;)					/* righst sub-tree */
{
	register struct tnode *tp;

	tp = talloc(sizeof(struct tnode) - sizeof(union tval) + 2 * sizeof(struct tnode *));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_su = info;					/* info for bit-field & condbr's */
	tp->t_left = left;
	tp->t_right = right;
	return tp;
}


/* cnalloc - allocate constant expression tree node */
struct tnode *cnalloc(P(int) type, P(int) value)
PP(int type;)								/* type of constant */
PP(int value;)								/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(short));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_value = value;
	return cp;
}


/* lcnalloc - allocate constant expression tree node */
struct tnode *lcnalloc(P(int) type, P(long) value)
PP(int type;)								/* type of constant */
PP(long value;)								/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(long));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_lvalue = value;
	return cp;
}


/* fpcnalloc - allocate constant expression tree node */
struct tnode *fpcnalloc(P(int) type, P(long) value)
PP(int type;)								/* type of constant */
PP(long value;)								/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(*cp) - sizeof(union tval) + sizeof(long));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_lvalue = value;
	return cp;
}


/* talloc - allocate expression tree area */
struct tnode *talloc(P(int) size)
PP(int size;)
{
	register char *p;

	p = opap;
	if (p + size >= &exprarea[EXPSIZE])
		ferror("expression too complex");
	opap = p + size;
	return (struct tnode *)p;
}


/* symcopy - copy symbol */
VOID symcopy(P(const char *) from, P(char *) to)
PP(const char *from;)								/* from symbol */
PP(char *to;)								/* to symbol */
{
	register const char *p;
	register char *q;
	register short i;

	for (p = from, q = to, i = SSIZE; --i >= 0;)
	{
#ifdef __ALCYON__
		asm("tst.b     (a5)");
		asm("beq.s     l8887");
		asm("move.b    (a5)+,d0");
		asm("ext.w     d0");
		asm("bra.s     l8888");
		asm("l8887:");
		asm("clr.w     d0");
		asm("l8888:");
		asm("ext.w     d0");
		asm("move.b    d0,(a4)+");
#else
		*q++ = (*p ? *p++ : '\0');
#endif
	}
}


/* usage - output usage message */
VOID usage(P(const char *) calledby)
PP(const char *calledby;)
{
#ifdef DEBUG
	ferror("usage: %s icode link asm [-DTacemov]", calledby);
#else
	ferror("usage: %s icode link asm [-Tav]", calledby);
#endif
}


/* cputc - put a character to a file descriptor (used by error) */
VOID cputc(P(char) c, P(int) fn)
PP(char c;)
PP(int fn;)
{
	if (fn == STDERR)
		write(STDERR, &c, 1);
	else
		putchar(c);
}


/*
 * putchar - special version
 *		This allows the use of printf for error messages, debugging
 *		output and normal output.
 */
VOID putchar(P(char) c)
PP(char c;)
{
	if (dflag > 1)
		write(1, &c, 1);				/* to standard output */
	putc(c, &obuf);						/* put to assembler file */
}


int x1flush(P(FILE *) mybuf)
PP(register FILE *mybuf;)
{
	register int i;

	i = BLEN - mybuf->cc;
	mybuf->cc = BLEN;
	mybuf->cp = &(mybuf->cbuf[0]);
	if (write(fileno(mybuf), mybuf->cp, i) != i)
		return EOF;
	return 0;
}


#ifdef __ALCYON__
VOID printf(string, a, b, c, d, e, f, g)
const char *string;
int a, b, c, d, e, f, g;
{
	char area[256];
	register char *p;

	sprintf(area, string, a, b, c, d, e, f, g);
	for (p = &area[0]; *p; p++)
		putchar(*p);
}
#else
VOID printf(P(const char *) string _va_alist)
PP(const char *string;)
_va_dcl
{
	char area[256];
	register char *p;
	va_list args;
	
	va_start(args, string);
	vsprintf(area, string, args);
	for (p = &area[0]; *p; p++)
		putchar(*p);
	va_end(args);
}
#endif



/*
 * these are here only to be able to compile & link with non-Alcyon compilers;
 * running the program will not work
 */

#ifndef __ALCYON__
#undef FILE
#undef getc
#undef putchar
#undef fopen
int xgetc(struct iob *i)
{
	FILE *fp = *((FILE **)i);
	return fgetc(fp);
}
int xputc(char c, struct iob *o)
{
	FILE *fp = *((FILE **)o);
	return fputc(c, fp);
}
int xfopen(const char *fname, struct iob *i, int binary)
{
	FILE *fp;
	fp = fopen(fname, "r");
	if (fp)
	{
		*((FILE **)i) = fp;
		return 0;
	}
	return -1;
}
int xfcreat(const char *fname, struct iob *o, int binary)
{
	FILE *fp;
	fp = fopen(fname, "w");
	if (fp)
	{
		*((FILE **)o) = fp;
		return 0;
	}
	return -1;
}
#endif
