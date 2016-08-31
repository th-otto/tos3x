/**
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, Ca.  92121
**/

char *version = "@(#)c068 parser 4.2 - Sep 6, 1983";

#include "parser.h"
#include "def.h"
#include <signal.h>

/**
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
**/

char strfile[] = "/tmp/pXXXXXX";

int cleanup();

/**
 * main - main routine for parser
 *		Checks arguments, opens input and output files, does main loop
 *		for external declarations and blocks.
**/
main(argc, argv)						/* returns - none */
int argc;								/* argument count */

char *argv[];							/* argument pointers */
{
	register char *q,
	*p,
	*calledby;

	calledby = *argv++;
	if (argc < 4)
		usage(calledby);
	signal(SIGINT, cleanup);
	for (q = &source, p = *argv++; *q++ = *p++;) ;
	if (fopen(source, &ibuf, 0) < 0)	/* 3rd arg for versados */
		ferror("can't open %s", source);
	source[strlen(source) - 1] = 'c';
	if (fcreat(*argv++, &obuf, 0) < 0 || fcreat(*argv++, &lbuf, 0) < 0)
		ferror("temp creation error");

	mktemp(strfile);
	if (fcreat(strfile, &sbuf, 0) < 0)
		ferror("string file temp creation error");
	obp = &obuf;
	lineno++;
	frstp = -1;							/* [vlh] 3.4 - initialize only once */
	cr_last = 1;						/* [vlh] 4.2 */

#ifndef VERSADOS
	for (argc -= 4; argc; argv++, argc--)
	{									/* get args.... */
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

			case 'w':					/* [vlh] 4.1 warning messages, not fatal */
				wflag++;
				continue;

			case 't':					/* [vlh] 4.1, put strings into text segment */
				tflag++;
				continue;

#ifdef DEBUG
			case 'D':					/* [vlh] 4.1, turn debugging on */
				debug++;
				continue;

			case 's':					/* [vlh] 4.1, if debug on, debug symbols */
				if (debug)
					symdebug++;
				continue;

			case 'x':					/* [vlh] 4.1, if debug on, debug expr tree */
				if (debug)
					treedebug++;
				continue;
#endif

			case '\0':
				break;

			default:
				usage(calledby);

			}							/* end of case */
			break;
		}								/* end of while loop */
	}									/* end of for loop to get flags */
#endif

	syminit();
	while (!PEEK(EOF))
		doextdef();
	outeof();
	if (!tflag)							/* [vlh] 4.1 */
		outdata();
	else								/* [vlh] 4.1, output strings into text segment */
		OUTTEXT();
	copysfile(strfile);
	cleanup();
}

cleanup()
{
	signal(SIGINT, SIG_IGN);
	close(lbuf.fd);
	unlink(strfile);
	exit(errcnt != 0);
}

/* usage - output usage error message and die*/
usage(calledby)
char *calledby;
{
	ferror("usage: %s source link icode [-e|-f] [-w] [-T]", calledby);
}

/**
 * error - report an error message
 *		outputs current line number and error message
 *		[vlh] 4.2 generate filename and approp line number
**/
error(s, x1, x2, x3, x4, x5, x6)		/* returns - none */
char *s;								/* error message */

int x1,
 x2,
 x3,
 x4,
 x5,
 x6;									/* args for printf */
{
	printf((char *) STDERR, "\"%s\", * %d: ", source, lineno);
	printf((char *) STDERR, s, x1, x2, x3, x4, x5, x6);
	cputc('\n', STDERR);
	errcnt++;
}

/* ferror - fatal error*/
/*		Outputs error message and exits*/
ferror(s, x1, x2, x3, x4, x5, x6)		/* returns - none */
char *s;								/* error message */

int x1,
 x2,
 x3,
 x4,
 x5,
 x6;									/* args for printf */
{
	error(s, x1, x2, x3, x4, x5, x6);
	errcnt = -1;
	cleanup();
}

/**
 * warning - Bad practices error message (non-portable code)
 *		Outputs error message
 *		[vlh] 4.2, generate filename and approp line number
**/
warning(s, x1, x2, x3, x4, x5, x6)		/* returns - none */
char *s;								/* error message */

int x1,
 x2,
 x3,
 x4,
 x5,
 x6;									/* args for printf */
{
	if (wflag)
		return;
	printf((char *) STDERR, "\"%s\", * %d: (warning) ", source, lineno);
	printf((char *) STDERR, s, x1, x2, x3, x4, x5, x6);
	cputc('\n', STDERR);
}

/* synerr - syntax error*/
/*		Outputs error message and tries to resyncronize input.*/
synerr(s, x1, x2, x3, x4, x5, x6)		/* returns - none */
char *s;								/* printf format string */

int x1,
 x2,
 x3,
 x4,
 x5,
 x6;									/* printf arguments */
{
	register short token;

	error(s, x1, x2, x3, x4, x5, x6);
	while ((token = gettok(0)) != SEMI && token != EOF && token != LCURBR && token != RCURBR)
		;
	pbtok(token);
}

v6flush(v6buf)
struct iob *v6buf;
{
	register short i;

	i = BLEN - v6buf->cc;
	v6buf->cc = BLEN;
	v6buf->cp = &(v6buf->cbuf[0]);
	if (write(v6buf->fd, v6buf->cp, i) != i)
		return (-1);
	return (0);
}

/* index - find the index of a character in a string*/
/*		This is identical to Software Tools index.*/
index(str, chr)							/* returns index of c in str or -1 */
char *str;								/* pointer to string to search */

char chr;								/* character to search for */
{
	register char *s;

	register short i;

	for (s = str, i = 0; *s != '\0'; i++)
		if (*s++ == chr)
			return (i);
	return (-1);
}


/* genunique - generate a unique structure name */
genunique(ptr)
char *ptr;
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

static char _uniqlet = 'A';

char *mktemp(ap)
char *ap;
{
	register char *p;

	register int i,
	 j;

	p = ap;
	i = getpid();						/*process id */

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
		return (0);
	return (ap);
}

/** 
 *	strlen - compute string length.
 *		computes number of bytes in a string.
**/
strlen(s)
char *s;
{
	register int n;

	for (n = 0; *s++ != '\0';)
		n++;
	return (n);
}

#ifdef DRI
printf(string, a, b, c, d, e, f, g)
char *string;

int a,
 b,
 c,
 d,
 e,
 f,
 g;
{
	char area[256];

	register char *p;

	sprintf(area, string, a, b, c, d, e, f, g);
	for (p = &area[0]; *p; p++)
		putchar(*p);
}
#endif
