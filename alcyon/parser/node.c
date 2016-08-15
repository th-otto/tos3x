/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* node allocation, node stack manipulation routines */

#include "parser.h"

/**
 * talloc - expression area tree node allocation
 *		Allocates area and checks for overflow.
**/
char *
talloc(size)			/* returns pointer to node*/
int size;				/* size of node to alloc*/
{
	register char *p;

	p = opap;
	if( (p + size) >= &exprarea[EXPSIZE] )
		ferror("expression too complex");
	opap = p + size;
	return(p);
}

/**
 * enalloc - external name alloc
 *		Allocates an expression tree node for an external name and
 *		copies symbol table info and symbol into tree node.
**/
char *
enalloc(sp)			/* returns - none*/
struct symbol *sp;			/* pointer to symbol table entry*/
{
	register struct extnode *ep;

	ep = (struct extnode *)talloc(sizeof(*ep));
	ep->t_op = SYMBOL;
	ep->t_sc = sp->s_sc;
	ep->t_type = sp->s_type;
	ep->t_dp = sp->s_dp;
	ep->t_ssp = sp->s_ssp;
	ep->t_offset = sp->s_offset;
	symcopy(sp->s_symbol,ep->t_symbol);
	return((char *)ep);
}

/**
 * cnalloc - constant node allocation
 *		Allocates a constant tree node and fills the info fields.
**/
char *
cnalloc(type,value)		/* returns pointer to node*/
int type;				/* data type*/
int value;				/* constant value*/
{
	register struct conode *cp;

	cp = (struct conode *)talloc(sizeof(*cp));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_value = value;
	return((char *)cp);
}

/**
 *	lcnalloc - long constant node allocation
 *		Allocates a constant tree node and fills the info fields.
**/
char *
lcnalloc(type,value)	/* returns pointer to node*/
int type;				/* data type*/
long value;				/* constant value*/
{
	register struct lconode *cp;

	cp = (struct lconode *)talloc(sizeof(*cp));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_lvalue = value;
	return((char *)cp);
}

/**
 *	fpcnalloc - floating point constant node allocation
 *		Allocates a constant tree node and fills the info fields.
**/
char *
fpcnalloc(type,value)		/*[vlh] 3.4 returns pointer to node*/
int type;				/* data type*/
long value;				/* constant value*/
{
	register struct lconode *cp;

	cp = (struct lconode *)talloc(sizeof(*cp));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_lvalue = value;
	return((char *)cp);
}

/**
 * tnalloc - tree node allocation
 *		Allocates an operator tree node and fills the info fields
**/
char *
tnalloc(op,type,dp,ssp,left,right)	/* returns pointer to node*/
int op;					/* operator*/
int type;				/* operator type*/
int dp;					/* dimension pointer or other info*/
int ssp;				/* structure length pointer*/
struct tnode *left;		/* left subtree*/
struct tnode *right;	/* right subtree*/
{
	register struct tnode *tp;
	
	tp = (struct tnode *)talloc(sizeof(*tp));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_dp = dp;
	tp->t_ssp = ssp;
	tp->t_left = left;
	tp->t_right = right;
#ifdef DEBUG
	if (treedebug) 
		printf("tnalloc: op %d type %d dp %d ssp %d\n",op,type,dp,ssp);
#endif
	return((char *)tp);
}

/**
 * snalloc - symbol node allocation
 *		Allocates a tree symbol node and sets the info in it
**/
char *
snalloc(type,sc,off,dp,ssp)	/* returns pointer to node alloc'ed*/
int type;				/* symbol type*/
int sc;					/* storage class*/
int off;				/* offset*/
int dp;					/* dimension pointer or other info*/
int ssp;				/* structure size pointer*/
{
	register struct symnode *snp;

	snp = (struct symnode *)talloc(sizeof(*snp));
	snp->t_op = SYMBOL;
	snp->t_sc = sc;
	snp->t_type = type;
	snp->t_dp = dp;
	snp->t_ssp = ssp;
	snp->t_offset = off;
	return((char *)snp);
}

/**
 * pushopd - put operand node onto operand stack
 *		Checks for operand stack overflow.
**/
pushopd(tp)				/* returns - none*/
struct tnode *tp;			/* pointer to tree node to push*/
{
	if( opdp >= &opdstack[OPDSIZE] )
		ferror("expression too complex");
	*opdp++ = (char *)tp;
}

/**
 * popopd - pop operand stack
 *		Checks for stack underflow
**/
char *
popopd()				/* returns ptr to top operand*/
{
	register char *tp;	/* struct tnode */

	if( opdp <= &opdstack[0] )
		return(0);
	tp = *--opdp;
	return(tp);
}

/* doopd - handle constant or symbol node operand*/
/*		Pushes node onto operand stack and handles opdontop flag.*/
doopd(tp)						/* returns 1 if syntax error, 0 for ok*/
struct tnode *tp;				/* pointer to tree node*/
{
	pushopd(tp);
	if( opdontop )
		return(1);
	opdontop++;
	return(0);
}
