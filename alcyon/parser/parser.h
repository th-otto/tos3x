/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.	92121
 */

/*
 *	C68 Parser - include file
 */
#include "../include/compiler.h"
#include <stdio.h>
#include <stdint.h>
#include "klib.h"

#define NOPROFILE
/* #define DEBUG */

#include "icode.h"

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define _(x) x


/* symbol attribute fields */
#define SRESWORD	001 		/* is symbol a reserved word? */
#define SGLOBAL 	002 		/* is symbol global? */
#define STYPEDEF	004 		/* typedef declaration? */
#define SDEFINED	010 		/* symbol defined? */

/* reserved words */
#define R_AUTO		1
#define R_BREAK 	2
#define R_CASE		3
#define R_CHAR		4
#define R_CONTINUE	5
#define R_DO		6
#define R_DEFAULT	7
#define R_DOUBLE	8
#define R_GOTO		9
#define R_ELSE		10
#define R_EXTERNAL	11
#define R_FLOAT 	12
#define R_FOR		13
#define R_IF		14
#define R_INT		15
#define R_LONG		16
#define R_REGISTER	17
#define R_RETURN	18
#define R_SHORT 	19
#define R_SIZEOF	20
#define R_STATIC	21
#define R_STRUCT	22
#define R_SWITCH	23
#define R_TYPEDEF	24
#define R_UNION 	25
#define R_UNSIGNED	26
#define R_WHILE 	27
#define R_ASM		28

/*
 * mixed-mode conversions, entries in 2-d array indexed by:
 * (int,unsn,long,doub,ptr)
 */
#define INT_CHAR		1
#define UNSN_CHAR		1
#define LONG_CHAR		1
#define DOUB_CHAR		1
#define PTR_CHAR		1
#define INT_UNSN		0		/* no conversion is generated */
#define INT_LONG		2
#define INT_DOUB		3
#define INT_PTR 		4
#define UNSN_INT		0		/* no conversion is generated */
#define UNSN_LONG		6
#define UNSN_DOUB		7
#define UNSN_PTR		8
#define LONG_INT		9
#define LONG_UNSN		10
#define LONG_DOUB		11
#define LONG_PTR		12
#define DOUB_INT		13
#define DOUB_UNSN		14
#define DOUB_LONG		15
#define PTR_INT 		16
#define PTR_UNSN		17
#define PTR_LONG		18
#define PTR_PTR 		19
#define BADCONV 		20

/* miscellaneous constants */
#define OPSSIZE 	40			/* operator stack size */
#define OPDSIZE 	80			/* operand stack size */
#define HSIZE		512 		/* hash table size, 3.4 made prime */
#define BSIZE		512 		/* io buffer size */
#define SWSIZE		256 		/* max no. of cases in a switch */
#define DSIZE		1000		/* dimension table size */
#define BITSPWORD	16			/* bits per word */
#define AREGLO		010 		/* A reg flag */
#define DREG		0100		/* data loadable into D-register? */
#define HICREG		2			/* highest reg # used for code gen */
#define BITSPCHAR	8			/* bits per char */
#define CHRSPWORD	2			/* chars per word */
#define STRSIZE 	1024 		/* max string length */
#define NFARGS		40			/* max no. of args to function */
#define NFRSTR		128 		/* max no. of forward ref struct proto */
#define PATHSIZE	128
#define SCOPE_LEVLS 50			/* # of scope levels */
#define GLOB_SCOPE	0			/* global level is 0 */
#define FUNC_SCOPE	1			/* function level is 1 */

#define SYMSIZE 	128			/* # of symbol structures to allocate */

#ifdef VAX11
#define LW_LITTLE 1
#endif
#ifdef __i386__
#define LW_LITTLE 1
#endif
#ifdef __x86_64__
#define LW_LITTLE 1
#endif
#ifdef LW_LITTLE
struct words { short loword; short hiword; };
#else
struct words { short hiword; short loword; };
#endif



/* symbol table node */
struct symbol {
	char s_attrib;			/* defined, resword, global, typedef */
	char s_sc;				/* auto, static, external, register */
	short s_type;			/* 4bits specified, 2 bit fields for ptr */
	short s_dp; 			/* index into dimension table */
	short s_ssp;			/* dimension table/function arg table */
	short s_offset; 		/* offset inside of structure */
	short s_scope;			/* scope of symbol */
	char s_symbol[SSIZE];	/* symbol identifier, to SSIZE chars */
	struct symbol *s_par;	/* if struct, ptr to parent (sys III) */
	struct symbol *s_child; /* if struct, ptr to 1st child (sys III) */
	struct symbol *s_sib;	/* if struct, ptr to sibling (sys III) */
	struct symbol *s_next;	/* next symbol table entry */
};

/* expression tree operator node */
struct tnode {
	short t_op;
	short t_type;
	short t_dp;
	short t_ssp;
	struct tnode *t_left;		/* left sub-tree */
	struct tnode *t_right;		/* right sub-tree (undefined if unary) */
};

/* expression tree node for symbol - only keeps location */
struct symnode {
	short t_op;
	short t_type;				/* data type of symbol */
	short t_dp; 				/* dimension pointer of symbol */
	short t_ssp;				/* structure size index to dtab */
	short t_sc; 				/* storage class of symbol */
	short t_offset; 			/* offset of symbol */
	short t_label;
};

/* expressioon tree node for external symbol - need to keep name */
struct extnode {
	short t_op;
	short t_type;
	short t_dp;
	short t_ssp;
	short t_sc;
	short t_offset;
	short t_reg;
	char  t_symbol[SSIZE];		/* symbol name */
};

/* expression tree node for integer constant */
struct conode {
	short t_op;
	short t_type;
	short t_dp;
	short t_ssp;
	short t_value;				/* constant value */
};

/* long constant node structure */
struct lconode {
	short t_op;
	short t_type;
	short t_dp;
	short t_ssp;
	union {
		struct words w;
		int32_t l;				/* value or label number */
	} _l;
#define t_lvalue _l.l			/* constant value */
};

/* switch table structure */
struct swtch {
#ifndef VAX11
	short sw_label;
	short sw_value;
#else
	short sw_label;
	short sw_value;
#endif
} swtab[SWSIZE];

/* operator and operand stack used by expr */
struct ops {					/* operator stack */
	short o_op; 				/* operator */
	short o_pri;				/* priority */
} opstack[OPSSIZE];
struct ops *opp;
	
/* output buffers for intermediate code and strings */
FILE *ofil, *lfil, *sfil, *ifil, *obp;

#define EXPSIZE 	4096
extern char exprarea[EXPSIZE];


/* Miscellaneous Variables for declarations */
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
short strassign;
VOIDPTR *opdsave;
struct ops *oprsave;
VOIDPTR opdstack[OPDSIZE];		/* operand stack */
VOIDPTR *opdp;					/* operand stack pointer */
char *opap; 					/* ptr to next avail loc in exprarea */
extern short const opinfo[];	/* operator info table */
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
short strassign;
struct tnode *frp;				/* pointer to function return info node */
short smember;					/* set when seen . or -> */
short instmt;					/* in a stmt */
short infunc;					/* set when in function body */
short reducep;					/* if(procid); reduction */
short peektok;					/* peeked at token */

/* Parser Variables which are initialized in parser.ext */
extern char *exprp; 			/* place to start building expression */
extern short nextlabel; 		/* generates unique label numbers */
extern char const dinfo[];

/* Parser flags */
short fflag;					/* FFP floats */
short gflag;					/* symbolic debugger flag */
short xflag;					/* translate int's to long's */
short tflag;					/* put strings into text seg */
short wflag;					/* don't generate warning messages */
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

/* lexical analyzer value s */
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

/* function argument table, used to collect function parameters */
struct farg {
	struct symbol *f_sp;
	short f_offset;
} fargtab[NFARGS];

/* forward referenced structure prototype names */
struct symbol *frstab[NFRSTR];
short frstp;

/* Macro's used by the Parser */
#define ISTYPEDEF(sp)		(sp->s_attrib & STYPEDEF)
#define WALIGN(add)			((add + 1) & (~1))
#define ISARRAY(type)		(((type) & SUPTYP) == ARRAY)
#define ISFUNCTION(type)	(((type) & SUPTYP) == FUNCTION)
#define ISPOINTER(type)		(((type) & SUPTYP) == POINTER)
#define NOTARRAY(type)		(((type) & SUPTYP) != ARRAY)
#define NOTFUNCTION(type)	(((type) & SUPTYP) != FUNCTION)
#define NOTPOINTER(type)	(((type) & SUPTYP) != POINTER)
#define ISFLOAT(type)		((type) == FLOAT)
#define BTYPE(type)			((type) & TYPE)
#define SUPTYPE(type)		((type) & SUPTYP)
#define ISALLTYPE(type)		((type) & (SUPTYP|TYPE))
#define ISASGOP(op)			((opinfo[op] & OPASSIGN) != 0)
#define RELOP(op)			((opinfo[op] & OPREL) != 0)
#define COMOP(op)			((opinfo[op] & OPCOM) != 0)
#define LINTEGRAL(op)		((opinfo[op] & OPLWORD) != 0)
#define RINTEGRAL(op)		((opinfo[op] & OPRWORD) != 0)
#define RASOP(op)			((opinfo[op] & OPRAS) != 0)
#define BINOP(op)			((opinfo[op] & OPBIN) != 0)
#define UNARYOP(op)			((opinfo[op] & OPBIN) == 0)
#define LEAFOP(op)			((opinfo[op] & OPTERM) != 0)
#define NOTLEAFOP(op)		((opinfo[op] & OPTERM) == 0)
#define LVALOP(op)			((opinfo[op] & OPLVAL) != 0)
#define OPPRIORITY(op)		(opinfo[op] & OPPRI)
#define COMMOP(op)			((opinfo[op] & OPCOM) != 0)
#define CONVOP(op)			((opinfo[op] & OPCONVS) != 0)
#define NOTCONVOP(op)		((opinfo[op] & OPCONVS) == 0)
#define SIMPLE_TYP(typ)		((typ >= CHAR) && (typ <= DOUBLE))

/* checks for symbol with structure element storage class */
#define ISSTEL(tp)	(tp->t_op == SYMBOL && SESC(((struct symnode *)(tp))))
#define SESC(x) 	(x->t_sc == STELCL || x->t_sc == UNELCL || x->t_sc == BFIELDCL)

/* peek at next token, if not read token put back, else delete */
/* 1 if matched, 0 otherwise */
#define PEEK(tok)	( (peektok=gettok(0)) == tok )

/* outbentry - output symbol '%', signifying routine entry, for link info */
#define OUTBENTRY() oprintf("%%\n")
#define OUTCOMMON(sym,size) oprintf("\t.comm _%.*s,%ld\n", SSIZE, sym, (long)(size))
#define OUTGOTO(lab)		if( lab > 0 ) oprintf("\tbra L%d\n",lab)
/* change to text segment */
#define OUTTEXT()		oprintf("\t.text\n")
/* change segment to bss */
#define OUTBSS()		oprintf("\t.bss\n")
/* get on a word boundary */
#define OUTEVEN()		oprintf("\t.even\n")
/* output global symbol references */
#define OUTEXTDEF(sym)	oprintf("\t.globl _%.*s\n", SSIZE, sym)
/* outputs reserved memory */
#define OUTRESMEM(size) oprintf("\t.ds.b %ld\n", (long)(size))
/* output padding for word alignments */
#define OUTPAD()		oprintf("\t.even\n")
/* output long constant to assembler */
#define OUTLCON(val)	oprintf("\t.dc.l %ld\n", (long)(val))
/* output label constant */
#define OUTCLAB(lab)	oprintf("\t.dc.l L%d\n", lab)
/* output a label */
#define OUTLAB(lab) 	oprintf("\tL%d:", lab)
/* output function label */
#define OUTFLAB(sym)	oprintf("\t_%.*s:\n", SSIZE, sym); if (gflag) oprintf("\t~~%.*s:\n", SSIZE, sym)
#ifndef NOPROFILE
/* output function label */
#define OUTPCALL(sym)	oprintf("\tmove.l _%.*s,(sp)\n\tjsr ___popen\n", SSIZE, sym)
#endif
/* output data label */
#define OUTDLAB(sym)	oprintf("\t_%.*s:\n", SSIZE, sym)
/* output a null tree */
#define OUTNULL()		oprintf("0\n")

/* Debugging Macros */
#ifdef DEBUG
#define PUTEXPR(dbg,id_str,node_ptr) if (dbg) putexpr(id_str,node_ptr)
#else
#define PUTEXPR(dbg,id_str,node_ptr)
#endif

/* On Disk Symbol Table macro */
#define TO_DSK(symp,addr)
#define READ_ST(symp,addr)
#define ZERO_DSP()			dsp = 0

/* functions prototypes */

/*
 * decl.c
 */
VOID doextdef PROTO((NOTHING));
short gettype PROTO((short *defsc, short *deftype, int32_t *size, int declok));
int32_t dlist PROTO((int defsc));
int declarator PROTO((int castflg));
int getdecl PROTO((int castflg));
short addtdtype PROTO((struct symbol *tddp, int type, int dp, short *ssp));


/*
 * node.c
 */
VOIDPTR talloc PROTO((int size));
struct extnode *enalloc PROTO((struct symbol *sp));
struct conode *cnalloc PROTO((int type, int value));
struct lconode *lcnalloc PROTO((int type, int32_t value));
struct lconode *fpcnalloc PROTO((int type, int32_t value));
struct tnode *tnalloc PROTO((int op, int type, int dp, int ssp, struct tnode *left, struct tnode *right));
struct symnode *snalloc PROTO((int type, int sc, int off, int dp, int ssp));
VOID pushopd PROTO((VOIDPTR tp));
VOIDPTR popopd PROTO((NOTHING));
int doopd PROTO((struct tnode *tp));


/*
 * expr.c
 */
struct tnode *expr PROTO((int preset));
VOID expr_setup PROTO((NOTHING));
int is_terminal PROTO((short *token));
VOIDPTR get_symbol PROTO((NOTHING));
int binopeval PROTO((int op, struct lconode *ltp, struct lconode *rtp));
int unopeval PROTO((int op, struct lconode *tp));
int32_t cexpr PROTO((NOTHING));


/*
 * icode.c
 */
VOID outline PROTO((NOTHING));
VOID defbdata PROTO((NOTHING));
VOID defwdata PROTO((NOTHING));
VOID defldata PROTO((NOTHING));
VOID outc PROTO((int type, int value));
VOID outdata PROTO((NOTHING));
VOID outldata PROTO((NOTHING));
VOID outfpdata PROTO((NOTHING));
VOID outbexit PROTO((int nlocs, int nds, int nas));
VOID outlocal PROTO((int type, int sc, const char *sym, int val));
VOID outswitch PROTO((int ncases, int deflab, struct swtch *sp));
VOID outfp_or_l PROTO((int32_t l));
VOID outtstr PROTO((int lab));
int32_t outstr PROTO((int32_t maxsize, int32_t strsize));


/*
 * init.c
 */
VOID doinit PROTO((struct symbol *sp));
int32_t initlist PROTO((struct symbol *sp, int type, int dp, int ssp));


/*
 * interf.c
 */
VOID outinit PROTO((struct tnode *tp, int type));
VOID outcforreg PROTO((struct tnode *tp));
VOID outifgoto PROTO((struct tnode *tp, int dir, int lab));
VOID outasm PROTO((NOTHING));
VOID outexpr PROTO((struct tnode *tp));


/*
 * lex.c
 */
int gettok PROTO((int force));
int peekis PROTO((int tc));
int peekc PROTO((int tc));
int ngetch PROTO((NOTHING));
VOID putback PROTO((int c));
VOID getstr PROTO((char *str, int nchars, char endc));
int next PROTO((int tok));
VOID pbtok PROTO((int tok));


/*
 * main.c
 */
VOID error PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID fatal PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID warning PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID synerr PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
int strindex PROTO((const char *str, char chr));
VOID oprintf PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID oputchar PROTO((char c));


/*
 * misc.c
 */
short dalloc PROTO((int32_t dimsize));
int addsp PROTO((int type, int nsptype));
int delsp PROTO((int type));
int revsp PROTO((int type));
int falign PROTO((int type, int flen, unsigned int offset));
int salign PROTO((int type, unsigned int offset));
int delspchk PROTO((int type));
int dosizeof PROTO((struct tnode *tp, int tbool));
int32_t psize PROTO((struct tnode *tp));
int32_t dsize PROTO((int type, int dp, int sp));
VOID integral PROTO((struct tnode *tp, int atype));


/*
 * putexpr.c
 */
VOID putexpr PROTO((const char *name, struct tnode *tp));


/*
 * stmt.c
 */
VOID stmt PROTO((NOTHING));
VOID outassign PROTO((struct tnode *ltp, struct tnode *rtp));


/*
 * symt.c
 */
VOID syminit PROTO((NOTHING));
struct symbol *lookup PROTO((const char *sym, int force));
VOID freesyms PROTO((int level));
VOID chksyms PROTO((int ok));
VOID symcopy PROTO((const char *sym1, char *sym2));


/*
 * tree.c
 */
int maketree PROTO((int op));
