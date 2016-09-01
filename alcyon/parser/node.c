/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* node allocation, node stack manipulation routines */

#include "parser.h"

/*
 * talloc - expression area tree node allocation
 *		Allocates area and checks for overflow.
 */
VOIDPTR talloc(P(int) size)
PP(int size;)
{
	register char *p;

	p = opap;
	if ((p + size) >= &exprarea[EXPSIZE])
		fatal(_("expression too complex"));
	opap = p + size;
	return p;
}


/*
 * enalloc - external name alloc
 *		Allocates an expression tree node for an external name and
 *		copies symbol table info and symbol into tree node.
 */
struct extnode *enalloc(P(struct symbol *) sp)
PP(struct symbol *sp;)						/* pointer to symbol table entry */
{
	register struct extnode *ep;

	ep = (struct extnode *) talloc(sizeof(*ep));
	ep->t_op = SYMBOL;
	ep->t_sc = sp->s_sc;
	ep->t_type = sp->s_type;
	ep->t_dp = sp->s_dp;
	ep->t_ssp = sp->s_ssp;
	ep->t_offset = sp->s_offset;
	symcopy(sp->s_symbol, ep->t_symbol);
	return ep;
}


/*
 * cnalloc - constant node allocation
 *		Allocates a constant tree node and fills the info fields.
 */
struct conode *cnalloc(P(int) type, P(int) value)
PP(int type;)								/* data type */
PP(int value;)								/* constant value */
{
	register struct conode *cp;

	cp = (struct conode *) talloc(sizeof(*cp));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_value = value;
	return cp;
}


/*
 *	lcnalloc - long constant node allocation
 *		Allocates a constant tree node and fills the info fields.
 */
struct lconode *lcnalloc(P(int) type, P(long) value)
PP(int type;)								/* data type */
PP(long value;)								/* constant value */
{
	register struct lconode *cp;

	cp = (struct lconode *) talloc(sizeof(*cp));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_lvalue = value;
	return cp;
}


/*
 *	fpcnalloc - floating point constant node allocation
 *		Allocates a constant tree node and fills the info fields.
 */
struct lconode *fpcnalloc(P(int) type, P(long) value)
PP(int type;)								/* data type */
PP(long value;)								/* constant value */
{
	register struct lconode *cp;

	cp = (struct lconode *) talloc(sizeof(*cp));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_dp = 0;
	cp->t_ssp = 0;
	cp->t_lvalue = value;
	return cp;
}


/*
 * tnalloc - tree node allocation
 *		Allocates an operator tree node and fills the info fields
 */
struct tnode *tnalloc(P(int) op, P(int) type, P(int) dp, P(int) ssp, P(struct tnode *) left, P(struct tnode *) right)
PP(int op;)									/* operator */
PP(int type;)								/* operator type */
PP(int dp;)									/* dimension pointer or other info */
PP(int ssp;)								/* structure length pointer */
PP(struct tnode *left;)						/* left subtree */
PP(struct tnode *right;)					/* right subtree */
{
	register struct tnode *tp;

	tp = (struct tnode *) talloc(sizeof(*tp));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_dp = dp;
	tp->t_ssp = ssp;
	tp->t_left = left;
	tp->t_right = right;
#ifdef DEBUG
	if (treedebug)
		fprintf(stderr, "tnalloc: op %d type %d dp %d ssp %d\n", op, type, dp, ssp);
#endif
	return tp;
}


/*
 * snalloc - symbol node allocation
 *		Allocates a tree symbol node and sets the info in it
 */
struct symnode *snalloc(P(int) type, P(int) sc, P(int) off, P(int) dp, P(int) ssp)
PP(int type;)								/* symbol type */
PP(int sc;)									/* storage class */
PP(int off;)								/* offset */
PP(int dp;)									/* dimension pointer or other info */
PP(int ssp;)								/* structure size pointer */
{
	register struct symnode *snp;

	snp = (struct symnode *) talloc(sizeof(*snp));
	snp->t_op = SYMBOL;
	snp->t_sc = sc;
	snp->t_type = type;
	snp->t_dp = dp;
	snp->t_ssp = ssp;
	snp->t_offset = off;
	return snp;
}


/*
 * pushopd - put operand node onto operand stack
 *		Checks for operand stack overflow.
 */
VOID pushopd(P(VOIDPTR ) tp)
PP(VOIDPTR tp;)						/* pointer to tree node to push */
{
	if (opdp >= &opdstack[OPDSIZE])
		fatal(_("expression too complex"));
	*opdp++ = tp;
}


/*
 * popopd - pop operand stack
 * Checks for stack underflow
 */
VOIDPTR popopd(NOTHING)							/* returns ptr to top operand */
{
	register VOIDPTR tp;					/* struct tnode */

	if (opdp <= &opdstack[0])
		return NULL;
	tp = *--opdp;
	return tp;
}


/*
 * doopd - handle constant or symbol node operand
 * Pushes node onto operand stack and handles opdontop flag.
 * returns 1 if syntax error, 0 for ok
 */
int doopd(P(struct tnode *) tp)
PP(struct tnode *tp;)			/* pointer to tree node */
{
	pushopd(tp);
	if (opdontop)
		return 1;
	opdontop++;
	return 0;
}
