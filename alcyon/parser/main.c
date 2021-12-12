/*
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, Ca.  92121
 */

#define _GNU_SOURCE

#include "parser.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/stdarg.h"


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

static char *strfile;

static char const program_name[] = "c068";

#if KLUDGE
static struct kludge_iob obuf, lbuf, sbuf, ibuf;
#endif

struct swtch swtab[SWSIZE];

struct ops opstack[OPSSIZE];
struct ops *opp;
FILE *ofil, *lfil, *sfil, *ifil, *obp;

short scope_decls[SCOPE_LEVLS]; /* decls at this scope ?? */
short scope_level;				/* global=0, func=1 */
short indecl;					/* are we in a decl ?? */
short predecl;					/* value previous to sizeof */	
short tdflag;					/* declaration is a typedef proto */
struct symbol *tdp; 			/* points to typedef prototype */
short localsize;				/* length of local variables */
short naregs;					/* keeps track of ptr registers alloc'd */
short ndregs;					/* keep track of data registers alloc'd */
short boffset;					/* current bit offset in structure */
short in_struct;				/* set when in structure declaration */


/* Miscellaneous Variables for expression handling */
short opdotsave;				/* vars used by the expression evaluator */
short opdontop; 				/* op on top of expr stack ?? */
VOIDPTR *opdsave;
struct ops *oprsave;
VOIDPTR opdstack[OPDSIZE];		/* operand stack */
VOIDPTR *opdp;					/* operand stack pointer */
char *opap; 					/* ptr to next avail loc in exprarea */
short commastop;				/* stop parse at comma */
short colonstop;				/* stop parse at colon */

/* Miscellaneous Variables for statement generation */
short cswp; 					/* current low switch table index */
short clabel;					/* continue label */
short blabel;					/* break label */
short rlabel;					/* return label */
short dlabel;					/* default label */
	
/* Miscellaneous Variables */
short lineno;					/* current line number of input */
short lst_ln_id;				/* last line an id was output on... */
short cr_last;					/* determine if # is file specification */
short errcnt;					/* count of errors */
char source[PATHSIZE];			/* source filename for error reporting */
struct tnode *frp;				/* pointer to function return info node */
short smember;					/* set when seen . or -> */
short instmt;					/* in a stmt */
short infunc;					/* set when in function body */
short reducep;					/* if(procid); reduction */
short peektok;					/* peeked at token */


/* Parser flags */
short fflag;					/* FFP floats */
short gflag;					/* symbolic debugger flag */
short xflag;					/* translate int's to long's */
short tflag;					/* put strings into text seg */
short wflag;					/* don't generate warning messages */
short aesflag;					/* hack for TOS 1.x AES */
#ifndef NOPROFILE
short profile;					/* profiler output */
#endif
#ifdef DEBUG
short initdebug;				/* init debug flag */
short symdebug; 				/* sym debug flag */
short treedebug;				/* expr tree debug flag */
#endif

/* dimension table */
int32_t dtab[DSIZE];			/* short => long */
short cdp;						/* next entry in dtab to alloc */

/* lexical analyzer values */
short cvalue;					/* current token if keyword or CINT */
short ccbytes;					/* number of bytes in char constant */
short cstrsize; 				/* current string size */
int32_t clvalue;				/* current token value if long constant */
struct symbol *csp; 			/* current token symbol ptr if SYMBOL */
char cstr[STRSIZE]; 			/* current token value if CSTRING */
struct symbol *dsp; 			/* declarator symbol pointer */


/* 0 no structure */
struct symbol *struc_parent[10];/* ptrs to struc symbols */
struct symbol *struc_sib[10];	/* ptrs to struc symbols */
struct symbol *hold_sib;		/* wrap sib past struct decl */

struct farg fargtab[NFARGS];

/* forward referenced structure prototype names */
struct symbol *frstab[NFRSTR];
short frstp;


static VOID outeof(NOTHING)
{
	if (lfil)
		kflush(lfil);
	if (sfil)
		kflush(sfil);
	if (ofil)
		kflush(ofil);
}


static VOID cleanup(NOTHING)
{
	signal(SIGINT, SIG_IGN);
	if (lfil)
		kfclose(lfil);
	if (ifil && ifil != (FILE *)stdin)
		kfclose(ifil);
	if (ofil && ofil != (FILE *)stdout)
		kfclose(ofil);
	if (sfil)
		kfclose(sfil);
	if (strfile)
		unlink(strfile);
	exit(errcnt != 0);
}


/* copysfile - copy string file to end of output file */
static VOID copysfile(P(const char *) fname)
PP(const char *fname;)
{
	register short c;

	kfclose(sfil);
	if ((sfil = xfopen(fname, &sbuf)) == NULL)
		fatal(_("can't copy %s"), fname);
	while ((c = kgetc(sfil)) > 0)
		kputc(c, ofil);
	kflush(ofil);
	kfclose(sfil);
	sfil = NULL;
}


/* usage - output usage error message and die */
static VOID usage(NOTHING)
{
	error(_("usage: %s source link icode strings [-e|-f] [-w] [-t]"), program_name);
	error(_(" options:"));
	error(_("    -e       ieee floats (default)"));
	error(_("    -f       FFP floats"));
	error(_("    -g       symbolic debug output"));
	error(_("    -t       put strings into text segment"));
	error(_("    -w       suppress warning messages"));
#ifdef DEBUG
	error(_("    -d[isx]  debug generator:"));
	error(_("             i=init, s=symbols, x=tree"));
#endif
	errcnt = -1;
	cleanup();
}


/*
 * main - main routine for parser
 *		Checks arguments, opens input and output files, does main loop
 *		for external declarations and blocks.
 */
int main(P(int) argc, P(char **)argv)
PP(int argc;)								/* argument count */
PP(char **argv;)							/* argument pointers */
{
	register char *q, *p;

	if (argc < 5)
		usage();

	signal(SIGINT, (sighandler_t)cleanup);
	signal(SIGQUIT, (sighandler_t)cleanup);
	signal(SIGHUP, (sighandler_t)cleanup);
	signal(SIGTERM, (sighandler_t)cleanup);

	argv++;
	for (q = source, p = *argv++; (*q++ = *p++) != 0;)
		;
	if ((ifil = kfopen(source, &ibuf)) == NULL)
		fatal(_("can't open %s"), source);
	source[(int)strlen(source) - 1] = 'c';
	if ((ofil = kfcreat(*argv++, &obuf)) == NULL || (lfil = kfcreat(*argv++, &lbuf)) == NULL)
		fatal(_("temp creation error"));

	strfile = *argv++;
	if ((sfil = kfcreat(strfile, &sbuf)) == NULL)
		fatal(_("string file temp creation error"));
	obp = ofil;
	lineno++;
	frstp = -1;							/* initialize only once */
	cr_last = 1;

	for (argc -= 5; argc; argv++, argc--)
	{
		q = *argv;
		if (*q++ != '-')
			usage();
		for (;;)
		{
			switch (*q++)
			{
			case 'e':					/* ieee floats */
				fflag = 0;
				continue;

			case 'f':					/* FFP floats */
				fflag = 1;
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

			case 'd':					/* turn debugging on */
				while (*q)
				{
					switch (*q)
					{
					case 'i':					/* if debug on, debug initialization */
#ifdef DEBUG
						initdebug++;
#endif
						break;
					case 's':					/* if debug on, debug symbols */
#ifdef DEBUG
						symdebug++;
#endif
						break;
					case 'x':					/* if debug on, debug expr tree */
#ifdef DEBUG
						treedebug++;
#endif
						break;
					}
					q++;
				}
				continue;

			case 'A':
				aesflag = 1;
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
	return 0;
}


static VOID verror(P(const char *) s, P(va_list) args)
PP(const char *s;)
PP(va_list args;)
{
	if (lineno)
		fprintf(stderr, "\"%s\", * %d: ", source, lineno);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	errcnt++;
}


/*
 * error - report an error message
 *		outputs current line number and error message
 *		generate filename and approp line number
 */
VOID error(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	va_end(args);
}


/*
 * fatal - fatal error
 * Outputs error message and exits
 */
VOID fatal(P(const char *) s _va_alist)
PP(const char *s;)
_va_dcl
{
	va_list args;
	
	va_start(args, s);
	verror(s, args);
	va_end(args);
	errcnt = -1;
	cleanup();
}


/*
 * warning - Bad practices error message (non-portable code)
 *		Outputs error message
 *		generate filename and approp line number
 */
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


/*
 * synerr - syntax error
 * Outputs error message and tries to resyncronize input.
 */
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


VOID oprintf(P(const char *) string _va_alist)
PP(const char *string;)
_va_dcl
{
	char area[256];
	register char *p;
	va_list args;
	
	va_start(args, string);
	vsprintf(area, string, args);
	for (p = &area[0]; *p; p++)
		oputchar(*p);
	va_end(args);
}
