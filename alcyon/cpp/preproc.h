/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "../util/util.h"


/* cexpr operators */
#define CEOF		0
#define SUB			1
#define ADD			2
#define NOT			3
#define NEG			4
#define LPAREN		5
#define RPAREN		6
#define QMARK		7
#define COLON		8
#define OR			9
#define AND			10
#define XOR			11
#define EQUAL		12
#define NEQUAL		13
#define LESS		14
#define LSEQUAL		15
#define GREAT		16
#define GREQUAL		17
#define LSHIFT		18
#define RSHIFT		19
#define MULT		20
#define DIV			21
#define MOD			22
#define	COMPL		23
#define	CONST		24
#define	LASTOP		COMPL					/* up to here used by cexpr */
#define	SQUOTE		25
#define	DQUOTE		26
#define	ANYC		27
#define	BADC		28
#define	COMMA		29
#define	NEWL		30
#define	POUND		31
#define	ALPHA		32
#define	DIGIT		33
#define	BSLASH		34
#define	WHITE		35

/* Types of preprocessor macros */
#define	DEFINE		1
#define	UNDEF		2
#define	INCLUDE		3
#define	IFDEF		4
#define	IFNDEF		5
#define	ELSE		6
#define	ENDIF		7
#define	IF			8
#define LINE		9

/* Magic Numbers used in Macros */
#define	ARG			-1
#define	NEWLABEL	-2
#define	LABEL		-3
#define	NOARGS		-4

/* Skip state, using #ifdef... */
#define	SKIP		0
#define	NOSKIP		1

/* General Defines */
#define	SOH			'\01'
#define	SSIZE		8
#define	LINESIZE	512
#define	MAXARGS		60
#define	ARGBSIZE	1000
#define	TOKSIZE		300
#define	DEFSIZE		1024
#define PBSIZE		1000

#define TRUE		1
#define FALSE		0
#define NDEFS		20

#define CSTKSIZE    20
#define FILESEP     '/'
#define FILESEP2    '\\'
#define NINCL       10
#define LABSTART    1000

#ifndef VERSADOS
#	define	HSIZE	1024
#else
#	define	HSIZE	2048
#endif

/* Symbol Table Entry structure */
struct symbol {
	char s_name[SSIZE];
	char *s_def;
};
extern struct symbol symtab[HSIZE];

/* buffered I/O structure */
extern FILE *inbuf, *outbuf;

/* command line define structure */
struct defstruc {
	char *ptr;
	char *value;
};
extern struct defstruc defs[NDEFS];

#define	FSTACK	 10
#define MAXPSIZE 128
struct stackstruc {
	FILE *ifd;
	char ifile[MAXPSIZE];
	int lineno;
};
extern struct stackstruc filestack[FSTACK], *filep;		/* stack of incl files, ptr to... */

/* Variables used by #line macros */
extern int literal;			/* using #line */
extern int lit_num;			/* for error messages */
extern char lit_file[];		/* for error messages */

/* Flag Variable Declarations */
extern int pflag;
extern int Cflag;
extern int Eflag;
extern int asflag;
extern char *source;			/* preprocessor source file */
extern char dest[MAXPSIZE];		/* preprocessor destination file */

/* line to output after macro substitution */
extern char line[LINESIZE+2];				/* line buffer */
extern char *linep;							/* current line pointer */
extern int loverflow;						/* line overflow flag */
extern int lineno;

/* push back buffer */
extern char pbbuf[PBSIZE];					/* push back buffer */
extern char *pbp;							/* push back pointer */
extern int pbflag;							/* checks for recursive definition */

extern char null[];

/* Function declarations */

/*
 * cexpr.c
 */
int cexpr PROTO((NOTHING));
int getctok PROTO((NOTHING));
int stkop PROTO((int opr));
int constexpr PROTO((const char *str));

/*
 * lex.c
 */
extern char const ctype[];

VOID symcopy PROTO((const char *sym1, char *sym2));
VOID error PROTO((const char *s, ...)) __attribute__((format(printf, 1, 2)));
VOID putback PROTO((int c));
VOID pbtok PROTO((const char *s));
int ngetch PROTO((NOTHING));
struct symbol *getsp PROTO((const char *name));
int gettok PROTO((char *token));
int getstr PROTO((char *str, int nchars, char endc));
struct symbol *lookup PROTO((const char *name));

/*
 * macro.c
 */
extern int nincl;
extern char *incl[NINCL];

VOID putid PROTO((const char *fname, int lnum));
VOID install PROTO((const char *name, int def));
VOID dinstall PROTO((const char *name, const char *def));
int kwlook PROTO((const char *name));
VOID ppputl PROTO((int c));
VOID initl PROTO((NOTHING));
VOID putd PROTO((int c));
VOID expand PROTO((struct symbol *sp));
int getntok PROTO((char *token));
int domacro PROTO((int nd));

/*
 * main.c
 */
extern int status;

VOID cexit PROTO((NOTHING));
VOID itoa PROTO((int n, char *s, int w));
int strindex PROTO((const char *str, char chr));
int atoi PROTO((const char *as));
