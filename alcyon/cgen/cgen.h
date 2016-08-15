/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "../icode.h"

#ifdef DRI
#	include <stdio.h>
#	include <klib.h>
#	undef putchar
#	define putchar xputchar
#	undef ferror
#	define ferror xferror
#	define printf xprintf
#endif

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
#define	IMMED			1
#define	NOTIMMED		0
#define	NOTLOFFSET		0
#define	NOAUTO			1
#define	DOAUTO			0

#define STDERR			2
#define PATHSIZE		128

char brtab[][2];
short invrel[];
short swaprel[];
char *strtab[];

	/*operator tree node for unary and binary operators*/
struct tnode {
	short t_op;						/*operator*/
	short t_type;					/*data type of result*/
	short t_su;						/*Sethy-Ullman number*/
	short t_ssp;
	struct tnode *t_left;			/*left sub-tree*/
	struct tnode *t_right;			/*right sub-tree (undefined if unary)*/
};

	/*constant terminal node*/
struct conode {
	short t_op;						/*operator*/
	short t_type;						/*type*/
	short t_su;						/*Sethy-Ullman number*/
	short t_ssp;
	short t_value;					/*value or label number*/
};

	/*long constant terminal node*/
struct lconode {
	short t_op;						/*operator*/
	short t_type;						/*type*/
	short t_su;						/*Sethy-Ullman number*/
	short t_ssp;
	long t_lvalue;					/*value or label number*/
};

	/*local symbol terminal node*/
struct symnode {
	short t_op;						/*operator*/
	short t_type;						/*symbol data type*/
	short t_su;						/*Sethy-Ullman number*/
	short t_ssp;
	short t_sc;						/*storage class*/
	short t_offset;					/*register offset*/
	short t_reg;						/*register number*/
	short t_label;					/*label number if static*/
};

	/*external symbol reference node*/
struct extnode {
	short t_op;						/*operator*/
	short t_type;						/*symbol data type*/
	short t_su;						/*Sethy-Ullman number*/
	short t_ssp;
	short t_sc;						/*storage class*/
	short t_offset;					/*register offset*/
	short t_reg;						/*register number*/
	char t_symbol[SSIZE];			/*symbol name*/
};

	/*68000 special - indexed symbol node*/
	/*this is used to generate a An(off,Xn.type) address*/
struct indexnode {
	short t_op;
	short t_type;
	short t_su;
	short t_ssp;
	short t_sc;
	short t_offset;
	short t_reg;
	short t_xreg;
	short t_xtype;
};

	/* io buffer declaration */
#define BSIZE	512
struct iob {
	int  fd;
	int  cc;
	char *cp;
	char cbuf[BSIZE];
} ibuf, lbuf, obuf;

	/* Code generation argument flags */
short dflag;
short mflag;
short cflag;
short eflag;
short fflag;
short oflag;
short lflag;
short m68010;			/* [vlh] 4.2, differentiate between chip destination */

	/* Miscellaneous variables */
short lineno;
short errcnt;
short opinfo[];
short nextlabel;
char optab[][6];
char *mnemonics[];
char *codeskels[];
short stacksize;

	/* general define macros */
#define	WALLIGN(add)	((add+1)&(~1))
#define	ISARRAY(type)	((type&SUPTYP)==ARRAY)
#define	ISFUNCTION(type)	((type&SUPTYP)==FUNCTION)
#define	ISPOINTER(type)	((type&SUPTYP)==POINTER)
#define	NOTARRAY(type)		((type&SUPTYP)!=ARRAY)
#define	NOTFUNCTION(type)	((type&SUPTYP)!=FUNCTION)
#define	NOTPOINTER(type)	((type&SUPTYP)!=POINTER)
#define ISFLOAT(type)	(type==FLOAT)
#define	BTYPE(type)		(type&TYPE)
#define	SUPTYPE(type)	(type&SUPTYP)
#define	ISALLTYPE(type)	(type&(SUPTYP|TYPE))
#define	ISASGOP(op)		((opinfo[op]&OPASSIGN)!=0)
#define	RELOP(op)		((opinfo[op]&OPREL)!=0)
#define	LINTEGRAL(op)	((opinfo[op]&OPLWORD)!=0)
#define	RINTEGRAL(op)	((opinfo[op]&OPRWORD)!=0)
#define	RASOP(op)		((opinfo[op]&OPRAS)!=0)
#define	BINOP(op)		((opinfo[op]&OPBIN)!=0)
#define	UNARYOP(op)		((opinfo[op]&(OPBIN|OPTERM))==0)
#define	LEAFOP(op)		((opinfo[op]&OPTERM)!=0)
#define	NOTLEAFOP(op)	((opinfo[op]&OPTERM)==0)
#define	LVALOP(op)		((opinfo[op]&OPLVAL)!=0)
#define	OPPRIORITY(op)	(opinfo[op]&OPPRI)
#define	COMMOP(op)		((opinfo[op]&OPCOM)!=0)
#define	CONVOP(op)		((opinfo[op]&OPCONVS)!=0)
#define	NOTCONVOP(op)	((opinfo[op]&OPCONVS)==0)
#define	MAX(a,b)		(a>b?a:b)
#define MIN(a,b)		(a<b?a:b)

/* one line routines turned into defines [vlh] for speed */

	/*outgoto - output "bra L[labno]"*/
#define OUTGOTO(lab)	if (lab>0) printf("bra L%d\n",lab)
	/*outlab - output "L[labno]:"*/
#define OUTLAB(lab)		if (lab>0) printf("L%d:",lab)

	/*outext - output register sign extension*/
#define OUTEXT(reg)		printf("ext.l R%d\n",reg)
	/*outuext - output unsigned to long register extension*/
#define OUTUEXT(reg)	printf("swap R%d\nclr R%d\nswap R%d\n",reg,reg,reg)
	/*outswap - output swap register instruction*/
#define OUTSWAP(reg)	printf("swap R%d\n",reg)
	/*outaddr - output "add [type] R1 R2" instruction*/
#define OUTADDR(r1,r2,tp)	outrr("add",r1,r2,(tp))
	/*outccsave - ouput instruction to move cc's to register*/
#define OUTSRSAVE(reg)	printf("move sr,R%d\n",reg)
#define OUTCCSAVE(reg)	printf("move ccr,R%d\n",reg)
	/*outccrestore - output instruction to restore cc's from register*/
#define OUTCCRESTORE(reg)	printf("move R%d,ccr\n",reg)
	/*basetype - get the btype info sans unsigned*/
#define BASETYPE(type)	((type==UNSIGNED) ? INT : type)
#define UNSIGN(type)	((type) == UNSIGNED)
#define LONGORPTR(type)	(type==LONG || (type&SUPTYP))
#define UNORPTR(type)	(type==UNSIGNED || (type&SUPTYP))
#define DREG(reg)		((reg) & (~AREGLO))
#define AREG(reg)		((reg) | AREGLO)
#define ISAREG(reg)		((reg) >= AREGLO)
#define ISDREG(reg)		((reg) < AREGLO)
#define ISREG(tp)		((tp)->t_op == SYMBOL && (tp)->t_sc == REGISTER)

#define CONSTZERO(ltyp,p) ((ltyp && !p->t_lvalue) || (!ltyp && !p->t_value))
#define SETVAL(ltyp,p,val) if (ltyp) p->t_lvalue = val; else p->t_value = val

#ifdef DEBUG
#	define PUTEXPR(cond,id_str,node_ptr)	if (cond) putexpr(id_str,node_ptr)
#else
#	define PUTEXPR(cond,id_str,node_ptr)
#endif

	/* Functions pre-declared */
char *tnalloc();
char *snalloc();
char *cenalloc();
char *xnalloc();
char *talloc();
char *cnalloc();
char *lcnalloc();
char *fpcnalloc();
char *canon();
char *commute();
char *constant();
char *match();
char *addptree();
char *fixbfield();
char *coffset();
char *tcopy();
char *fixptree();
long readlong();
short readshort();

