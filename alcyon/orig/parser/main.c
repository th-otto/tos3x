/*
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, Ca.  92121
 */

char *version = "@(#)main.c	1.8	12/28/83";

#include "parser.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "def.h"


/*
 *	ALCYON C Compiler for the Motorola 68000 - Parser
 *
 *	Called from c68:
 *
 *		c068 source icode link
 *
 *	source:		input source code, preprocessed with comments stripped
 *
 *	icode:		contains the intermediate code for the code generator,
 *				for a detailed explanaion see ../doc/icode.
 *
 *	link:		contains the procedure link and movem instructions.
 *
 *	The basic structure of the parser is as follows:
 *
 *	main							main driver for parser
 *		syminit						initializes symbol table
 *		doextdef					external definition syntax
 *			getatt					get type attributes
 *				dlist				declaration list for strucs/unions
 *					getatt			recursive gettype call
 *					dodecl			do one declaration
 *						declarator	handle declarator syntax
 *			dodecl					do one external declaraion
 *			initlist				external initialization list
 *				cexpr				constant expressions
 *					expr			arithmetic expressions
 *						maketree	build operator tree
 *			funcbody				function body
 *				dlist				declaration list
 *				stmt				function statements
 *					stmt			recursive stmt call
 *					expr			arithmetic expressions
 *
 */

char *strfile;


VOID cleanup PROTO((NOTHING));
VOID usage PROTO((char *calledby));



/*
 * main - main routine for parser
 *		Checks arguments, opens input and output files, does main loop
 *		for external declarations and blocks.
 */
int main(P(int) argc, P(char **)argv)
PP(int argc;)								/* argument count */
PP(char **argv;)							/* argument pointers */
{
	register char *q, *p, *calledby;

	calledby = *argv++;
	calledby = "c068";
	if (argc < 5)
		usage(calledby);

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, (sighandler_t)cleanup);
	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
		signal(SIGQUIT, (sighandler_t)cleanup);
	if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
		signal(SIGHUP, (sighandler_t)cleanup);
	signal(SIGTERM, (sighandler_t)cleanup);

	for (q = source, p = *argv++; (*q++ = *p++) != 0;)
		;
	if (fopen(source, &ibuf, 0) < 0)
		ferror("can't open %s", source);
	source[(int)strlen(source) - 1] = 'c';
	if (fcreat(*argv++, &obuf, 0) < 0 || fcreat(*argv++, &lbuf, 0) < 0)
		ferror("temp creation error");

	strfile = *argv++;
	if (fcreat(strfile, &sbuf, 0) < 0)
		ferror("string file temp creation error");
	obp = &obuf;
	lineno++;
	frstp = -1;							/* initialize only once */
	cr_last = 1;

	for (argc -= 5; argc; argv++, argc--)
	{
		q = *argv;
		if (*q++ != '-')
			usage(calledby);
		while (1)
		{
			switch (*q++)
			{
			case 'e':
				eflag++;
				continue;

			case 'f':
				fflag++;
				continue;

			case 'g':					/* symbolic debugger flag */
				gflag++;
				continue;

			case 't':					/* put strings into text segment */
				tflag++;
				continue;
#ifndef NOPROFILE
			case 'p':					/* profiler output file */
				profile++;
				continue;
#endif
			case 'w':					/* warning messages, not fatal */
				wflag++;
				continue;

#ifdef DEBUG
			case 'D':					/* turn debugging on */
			case 'd':
				debug++;
				continue;

			case 'i':					/* if debug on, debug initialization */
				if (debug)
					initdebug++;
				continue;

			case 's':					/* if debug on, debug symbols */
				if (debug)
					symdebug++;
				continue;

			case 'x':					/* if debug on, debug expr tree */
				if (debug)
					treedebug++;
				continue;
#endif

			case '\0':
				break;

			default:
				usage(calledby);

			}
			break;
		}
	}

	syminit();
	while (!PEEK(CEOF))
		doextdef();
	outeof();
	if (!tflag)
		outdata();
	else
		OUTTEXT();
	copysfile(strfile);
	cleanup();
#ifndef __ALCYON__
	return 0;
#endif
}


VOID cleanup(NOTHING)
{
	signal(SIGINT, SIG_IGN);
	unlink(strfile);
	close(fileno(&lbuf));
	exit(errcnt != 0);
}


/* usage - output usage error message and die */
VOID usage(P(char *) calledby)
PP(char *calledby;)
{
	ferror("usage: %s source link icode strings [-e|-f] [-w] [-T]", calledby);
}


#ifndef __ALCYON__
static VOID verror(P(const char *) s, P(va_list) args)
{
	fprintf(stderr, "\"%s\", * %d: ", source, lineno);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	errcnt++;
}
#endif


/*
 * error - report an error message
 *		outputs current line number and error message
 *		generate filename and approp line number
 */
#ifdef __ALCYON__
VOID error(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	fprintf(stderr, "\"%s\", * %d: ", source, lineno);
	fprintf(stderr, s, x1, x2, x3, x4, x5, x6);
	fprintf(stderr, "\n");
	errcnt++;
}
#else
VOID error(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	va_end(args);
}
#endif


/*
 * ferror - fatal error
 * Outputs error message and exits
 */
#ifdef __ALCYON__
VOID ferror(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	error(s, x1, x2, x3, x4, x5, x6);
	errcnt = -1;
	cleanup();
}
#else
VOID ferror(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	errcnt = -1;
	cleanup();
}
#endif


/*
 * warning - Bad practices error message (non-portable code)
 *		Outputs error message
 *		generate filename and approp line number
 */
#ifdef __ALCYON__
VOID warning(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	if (wflag)
		return;
	fprintf(stderr, "\"%s\", * %d: (warning) ", source, lineno);
	fprintf(stderr, s, x1, x2, x3, x4, x5, x6);
	fprintf(stderr, "\n");
}
#else
VOID warning(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	if (wflag)
		return;
	va_start(args, s);
	fprintf(stderr, "\"%s\", * %d: (warning) ", source, lineno);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}
#endif


/*
 * synerr - syntax error
 * Outputs error message and tries to resyncronize input.
 */
#ifdef __ALCYON__
VOID synerr(s, x1, x2, x3, x4, x5, x6)
const char *s;
int x1, x2, x3, x4, x5, x6;
{
	register short token;

	error(s, x1, x2, x3, x4, x5, x6);
	while ((token = gettok(0)) != SEMI && token != CEOF && token != LCURBR && token != RCURBR)
		;
	pbtok(token);
}
#else
VOID synerr(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	register short token;
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	while ((token = gettok(0)) != SEMI && token != CEOF && token != LCURBR && token != RCURBR)
		;
	pbtok(token);
}
#endif


int v6flush(P(FILE *) v6buf)
PP(FILE *v6buf;)
{
	register short i;

	i = BLEN - v6buf->cc;
	v6buf->cc = BLEN;
	v6buf->cp = &(v6buf->cbuf[0]);
	if (write(fileno(v6buf), v6buf->cp, i) != i)
		return EOF;
	return 0;
}


/*
 * strindex - find the index of a character in a string
 * This is identical to Software Tools index.
 * returns index of c in str or -1
 */
int strindex(P(const char *) str, P(char) chr)
PP(const char *str;)								/* pointer to string to search */
PP(char chr;)								/* character to search for */
{
	register const char *s;
	register short i;

	for (s = str, i = 0; *s != '\0'; i++)
		if (*s++ == chr)
			return i;
	return -1;
}


/* genunique - generate a unique structure name */
VOID genunique(P(char *) ptr)
PP(char *ptr;)
{
	register short num;

	*ptr++ = ' ';						/* symbols will never have names starting with a space */
	for (num = structlabel; num != 0;)
	{
		*ptr++ = (num % 10) + '0';
		num /= 10;
	}
	*ptr = '\0';
	structlabel++;
}


#ifdef __ALCYON__
static char _uniqlet = 'A';

char *mktemp(P(char *) ap)
PP(char *ap;)
{
	register char *p;
	register int i, j;

	p = ap;
	i = getpid();						/* process id */

	while (*p)
		p++;

	for (j = 5; --j != -1;)
	{
		*--p = ((i & 7) + '0');
		i >>= 3;
	}
	*--p = _uniqlet;

	_uniqlet++;
	if (_uniqlet > 'Z')
		_uniqlet = 'a';
	if (_uniqlet == 'z')
		return 0;
	return ap;
}
#endif


/*
 *	strlen - compute string length.
 *		computes number of bytes in a string.
 */
size_t strlen(P(const char *) s)
PP(const char *s;)
{
	register int n;

	for (n = 0; *s++ != '\0';)
		n++;
	return n;
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
