/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include <stdio.h>
#include <stdlib.h>
#include "../util/util.h"

#define BINEXACT 1

#include "icode.h"
#include "cskel.h"

#define _(x) x

#define	QUICKVAL		8
#define	LEP				14
#define	FORCC			1
#define	FOREFF			2
#define	FORSTACK		3
#define	FORCREG			4
#define	FORSP			5
#define	FORREG			4
#define	HICREG			2
#define	NCREGS			3
#define	AREGLO			8
#define	NOTLOFFSET		0

/* tcopy + outaexpr flags - generate prefix operators, postfix operators */
#define	A_DOPRE			1
#define	A_DOPOST		2
#define	A_DOIMMED		4
#define	A_NOIMMED		0

#define PATHSIZE		128

extern char const brtab[][2];
extern short const invrel[];
extern short const swaprel[];
extern const char *const strtab[];
extern const char *const opname[];


#ifndef VAX11
struct words { short hiword; short loword; };
#else
struct words { short loword; short hiword; };
#endif

struct xlsym {
	short sc;						/* storage class */
	long  offset;					/* register offset */
	short reg;						/* register number */
	short label;					/* label number if static */
};

struct xesym {
	short sc;						/* storage class */
	long  offset;					/* register offset */
	short reg;						/* register number */
	char symbol[SSIZE];				/* symbol name if external */
};

/* 68000 special - indexed symbol node, used to generate a An(off,Xn.type) address */
struct xxsym {
	short sc;						/* storage class */
	long  offset;					/* register offset */
	short reg;						/* register number */
	short xreg;
	short xtype;
};

union tval {
	struct {
		struct tnode *left;			/* left sub-tree */
		struct tnode *right;		/* right sub-tree (undefined if unary) */
	} t;
	struct words w;
	short s;						/* value or label number */
	long l;							/* value or label number */
	struct xlsym lsym;
	struct xesym esym;
	struct xxsym xsym;
};


/* operator tree node for unary and binary operators */
struct tnode {
	short t_op;						/* operator */
	short t_type;					/* data type of result */
	short t_su;						/* Sethy-Ullman number */
	short t_ssp;
	union tval v;
};

#define t_value v.s
#define t_lvalue v.l
#define t_left v.t.left
#define t_right v.t.right
#define t_sc v.lsym.sc
#define t_offset v.lsym.offset
#define t_reg v.lsym.reg
#define t_label v.lsym.label
#define t_symbol v.esym.symbol
#define t_xreg v.xsym.xreg
#define t_xtype v.xsym.xtype

extern struct tnode null;

/* Code generation argument flags */
extern short m68010;
extern short dflag;
extern short mflag;
extern short cflag;
extern short eflag;
extern short gflag;
extern short oflag;
extern short lflag;

/* expression tree storage */
#define EXPSIZE     4096
extern char exprarea[EXPSIZE];
extern char *opap;

/* Miscellaneous variables */
extern short lineno;
extern short errcnt;
extern short onepass;
extern short bol;
extern short const opinfo[];
extern short nextlabel;
extern char const optab[][6];
extern const char *const mnemonics[];
extern const struct skeleton *const codeskels[];
extern short stacksize;

/* general define macros */
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
#define UNARYOP(op)			((opinfo[op] & (OPBIN|OPTERM)) == 0)
#define LEAFOP(op)			((opinfo[op] & OPTERM) != 0)
#define NOTLEAFOP(op)		((opinfo[op] & OPTERM) == 0)
#define LVALOP(op)			((opinfo[op] & OPLVAL) != 0)
#define OPPRIORITY(op)		(opinfo[op] & OPPRI)
#define COMMOP(op)			((opinfo[op] & OPCOM) != 0)
#define CONVOP(op)			((opinfo[op] & OPCONVS) != 0)
#define NOTCONVOP(op)		((opinfo[op] & OPCONVS) == 0)
#define SIMPLE_TYP(typ)		((typ >= CHAR) && (typ <= DOUBLE))

#undef MAX
#undef MIN
#define	MAX(a,b)		((a) > (b) ? (a) : (b))
#define MIN(a,b)		((a) < (b) ? (a) : (b))

/* one line routines turned into defines for speed */

#if BINEXACT
/*
 * Note: these two macros are actually buggy and create side-effects
 * when used in codegen.c, but can't be fixed when we want o
/* outgoto - output "bra L[labno]" */
#define OUTGOTO(lab)	if (lab > 0) oprintf("bra L%d\n",lab)
/* outlab - output "L[labno]:" */
#define OUTLAB(lab)		if (lab > 0) oprintf("L%d:",lab)
#else
/* outgoto - output "bra L[labno]" */
#define OUTGOTO(lab)	if ((lab) > 0) oprintf("bra L%d\n",lab)
/* outlab - output "L[labno]:" */
#define OUTLAB(lab)		if ((lab) > 0) oprintf("L%d:",lab)
#endif

/* outext - output register sign extension */
#define OUTEXT(reg)		oprintf("ext.l R%d\n",reg)
/* outuext - output unsigned to long register extension */
#define OUTUEXT(reg)	oprintf("swap R%d\nclr R%d\nswap R%d\n",reg,reg,reg)
/* outswap - output swap register instruction */
#define OUTSWAP(reg)	oprintf("swap R%d\n",reg)
/* outaddr - output "add [type] R1 R2" instruction */
#define OUTADDR(r1,r2,tp)	outrr("add",r1,r2,(tp))
/* outccsave - ouput instruction to move cc's to register */
#define OUTSRSAVE(reg)	oprintf("move sr,R%d\n",reg)
#define OUTCCSAVE(reg)	oprintf("move ccr,R%d\n",reg)
/* outccrestore - output instruction to restore cc's from register */
#define OUTCCRESTORE(reg)	oprintf("move R%d,ccr\n",reg)
/* basetype - get the btype info sans unsigned */
#define BASETYPE(type)	((type == UNSIGNED) ? INT : type)
#define UNSIGN(type)	((type) == UNSIGNED)
#define LONGORPTR(type)	(type == LONG || (type & SUPTYP))
#define UNORPTR(type)	(type == UNSIGNED || (type & SUPTYP))
#define DREG(reg)		((reg) & (~AREGLO))
#define AREG(reg)		((reg) | AREGLO)
#define ISAREG(reg)		((reg) >= AREGLO)
#define ISDREG(reg)		((reg) < AREGLO)
#define ISREG(tp)		((tp)->t_op == SYMBOL && (tp)->t_sc == REGISTER)

#define CONSTZERO(ltyp,p) ((ltyp && !p->t_lvalue) || (!ltyp && !p->t_value))
#define SETVAL(ltyp,p,val) if (ltyp) p->t_lvalue = val; else p->t_value = val

#define BFOFFS(su) (((su) >> 8) & 0xff)
#define BFLEN(su)  (((su)     ) & 0xff)


#ifdef DEBUG
#	define PUTEXPR(cond,id_str,node_ptr)	if (cond) putexpr(id_str,node_ptr)
#else
#	define PUTEXPR(cond,id_str,node_ptr)
#endif

/* Functions pre-declared */

/*
 * canon.c
 */
struct tnode *canon PROTO((struct tnode *tp));
struct tnode *constant PROTO((struct tnode *tp, short *lconst));
int indexreg PROTO((struct tnode *tp));
int onebit PROTO((long val));

/*
 * codegen.c
 */
int scodegen PROTO((struct tnode *tp, int cookie, int reg));
short codegen PROTO((struct tnode *tp, int cookie, int reg));
struct tnode *coffset PROTO((struct tnode *tp));
VOID condbr PROTO((struct tnode *tp, int dir, int lab, int reg));

/*
 * interf.c
 */
VOID outexpr PROTO((struct tnode *tp));
VOID outifgoto PROTO((struct tnode *tp, int dir, int lab));
VOID outcforreg PROTO((struct tnode *tp));
VOID outinit PROTO((struct tnode *tp));
struct tnode *snalloc PROTO((int type, int sc, long offset, int dp, int ssp));
VOID outline PROTO((NOTHING));


/*
 * main.c
 */
VOID error PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID warning PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
VOID fatal PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));
struct tnode *tnalloc PROTO((int op, int type, int info, int dummy, struct tnode *left, struct tnode *right));
struct tnode *cnalloc PROTO((int type, int value));
struct tnode *lcnalloc PROTO((int type, long value));
struct tnode *fpcnalloc PROTO((int type, long value));
struct tnode *talloc PROTO((int size));

VOID oputchar PROTO((char c));
VOID oprintf PROTO((const char *s, ...)) __attribute__((format(__printf__, 1, 2)));

/*
 * putexpr.c
 */
VOID putexpr PROTO((const char *name, struct tnode *tp));
VOID puttsu PROTO((struct tnode *tp));


/*
 * smatch.c
 */
int expand PROTO((struct tnode *tp, int cookie, int freg, const struct skeleton *skp));
const struct skeleton *match PROTO((struct tnode *tp, int cookie, int reg));


/*
 * sucomp.c
 */
short sucomp PROTO((struct tnode *tp, int nregs, int flag));


/*
 * util.c
 */
struct tnode *xnalloc PROTO((int type, int ar, long off, int xr, int xt));
struct tnode *tcopy PROTO((struct tnode *tp, int autof));
VOID outaexpr PROTO((struct tnode *tp, int flags));
VOID outtype PROTO((int type));
VOID outatype PROTO((int type));
VOID outextend PROTO((struct tnode *tp, int type, int reg));
VOID outrr PROTO((const char *ins, int r1, int r2, struct tnode *tp));
VOID outmovr PROTO((int r1, int r2, struct tnode *tp));
VOID outcreg PROTO((int reg));
VOID outcmp0 PROTO((int reg, struct tnode *tp));
VOID outrpush PROTO((int reg, struct tnode *tp, int pflag));
int outdbra PROTO((int dir, int op, struct tnode *ltp, struct tnode *rtp, int lab));
struct tnode *cenalloc PROTO((int type, int sc, const char *sym));
VOID popstack PROTO((int nb));
VOID outcmpm PROTO((struct tnode *tp));
