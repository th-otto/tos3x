/*
    Copyright 1981
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "as68.h"


/* Expression evaluator */

/* precedence of operators */
#define PAO    2						/* AND, OR */
#define PPM    2						/* + - */
#define PMD    3						/* * / */
#define PLP    1						/* ( */
#define PRP    4						/* ) */
#define PEE    0						/* all other special chars */

#define OPSTLEN 10
#define TREELEN 20

/* globals for this package */
struct it exitm;						/* expression item */
short prcnt;							/* paren count */
short rval;								/* relocation value */
short lpflg;
short lastopr;							/* last token was operator when set */


VOID collapse PROTO((NOTHING));
VOID get_val PROTO((int reloc_val));
VOID exerr PROTO((NOTHING));
int gprc PROTO((int dprc));
long gval PROTO((struct it *avwrd));
int ckspc PROTO((int acksc));
int ckrl1 PROTO((int rv1, int rv2));
int ckrl2 PROTO((int rv1, int rv2));
int ckrl3 PROTO((int rv1, int rv2));
VOID fixext PROTO((struct symtab *p));






/*
 * expression evaluator
 *  call with:
 *      address of function to get input
 *  returns:
 *      item type in itype
 *      item value in ival
 *      relocation flag in reloc:
 *          0 => absolute
 *          1 => data
 *          2 => text
 *          3 => bss
 *          4 => external
 *
 * The only expressions involving externals which are legal are
 *      external+constant or external-constant
 */

struct it *piop, *pitr;
short iop, itr;
struct it opstk[OPSTLEN];				/* operator stack */
struct it tree[TREELEN];				/* operand stack */


VOID expr(P(aexpr) iploc)
PP(aexpr iploc;)
{
	register short i, ipr;

	extflg = starmul = iop = lpflg = 0;
	piop = &opstk[0];
	itr = -1;							/* tree stack pointer */
	pitr = &tree[0];
	pitr--;
	/* form end of expression operator */
	opstk[0].itty = ITSP;				/* special character */
	opstk[0].itop.u.loword = '?';
	lastopr = 1;

	/* get an input item */
	while (1)
	{
		if (itr >= TREELEN - 2)
		{
			rpterr("expr tree overflow\n");
			asabort();
		}
		if (iop >= OPSTLEN - 1)
		{
			rpterr("expr opstk overflow\n");
			asabort();
		}
		(*iploc) ();					/* get an input term */
		if (itype == ITPC)
			return;
		starmul = 0;					/* * is location counter */

		/* special character */
		if (itype == ITSP)
		{
			ipr = gprc(i = ival.u.loword);	/* get precedence of character */
			if (ipr == PEE)				/* end of expression */
				break;
			lastopr = 1;
			if (ipr == PLP)
			{							/* left paren */
				lpflg++;
				prcnt++;
				iop++;					/* up stack pointer */
				piop++;
				piop->swd1 = exitm.swd1;	/* put operator on stack */
				piop->itop.p = exitm.itop.p;
				continue;
			}
			if (ipr == PRP)
			{							/* right paren */
				if (lpflg)
				{
					exerr();
					return;
				}
				starmul = 1;			/* * is multiply */
				prcnt--;				/* down one level */

				while (piop->itop.l != '(')
				{						/* top stk is '(' */
					itr++;				/* up tree pointer */
					pitr++;
					pitr->swd1 = piop->swd1;	/* move operator */
					pitr->itop.l = piop->itop.l;
					iop--;				/* reduce operand stack */
					piop--;
				}
				iop--;					/* remove stack */
				piop--;
				continue;
			}

			while (ipr <= gprc(i = piop->itop.u.loword))
			{							/* >= precedence */
				itr++;
				pitr++;
				pitr->swd1 = piop->swd1;	/* move operator */
				pitr->itop.p = piop->itop.p;
				iop--;					/* reduce operand stack */
				piop--;
			}
			iop++;						/* up operator stack */
			piop++;
			piop->swd1 = exitm.swd1;	/* put in operator stack */
			piop->itop.p = exitm.itop.p;
			continue;
		}

		/* symbol or constant */
		else
		{
			lastopr = lpflg = 0;		/* clear flag */
			itr++;						/* up tree pointer */
			pitr++;
			pitr->swd1 = exitm.swd1;	/* put in tree */
			pitr->itop.p = exitm.itop.p;
			starmul = 1;				/* * is multiply */
			continue;
		}
	}									/* end while(1)... */

	/* output the rest of the operator stack to the tree */
	for (i = iop; i >= 0; i--)
	{
		itr++;
		pitr++;
		pitr->swd1 = piop->swd1;		/* move operator */
		pitr->itop.p = piop->itop.p;
		piop--;
	}

	collapse();
}


/* collapse the tree into one entry */
VOID collapse(NOTHING)
{
	register short rv1, rv2, topr, i, bos, low;
	register union iival tv1;
	union iival tv2;

	bos = 0;
  exct1:
	if (itr >= 3)
	{
		piop = &tree[bos];
		iop = bos;
		while (iop <= (itr - 3 + bos) && (piop->itty == ITSP || (piop + 1)->itty == ITSP || (piop + 2)->itty != ITSP))
		{
			iop++;
			piop++;
		}
		if (iop <= (itr - 3 + bos))
		{
			tv1.l = gval(piop);			/* get value of first operand */
			rv1 = rval;					/* relocation value */
			tv2.l = gval(piop + 1);
			rv2 = rval;
			topr = (piop + 2)->itop.l;	/* operator */

			/* handle operators */
			if (topr == '+')
			{
				tv1.l += tv2.l;
				rv1 = ckrl1(rv1, rv2);	/* relocation */
			} else if (topr == '-')
			{
				tv1.l -= tv2.l;
				rv1 = ckrl2(rv1, rv2);	/* relocation */
			} else
			{
				switch (topr)
				{						/* operator */
				case '/':				/* division */
					tv1.l /= tv2.l;
					break;
				case '*':				/* multiplication */
					tv1.l *= tv2.l;
					break;
				case '&':				/* logical and */
					tv1.l &= tv2.l;
					break;
				case '!':				/* logical or */
					tv1.l |= tv2.l;
					break;
				case '<':				/* left shift */
					low = tv2.u.loword;
					tv1.l <<= low;
					break;
				case '>':				/* right shift */
					low = tv2.u.loword;
					tv1.l >>= low;
					break;
				default:				/* invalid operator */
					exerr();
					return;
				}
				rv1 = ckrl3(rv1, rv2);	/* relocation */
			}

			/* put new value in tree */
			if (iop == bos)
			{
				bos += 2;
				iop = bos;
			}
			piop = &tree[iop];
			piop->itty = ITCN;			/* must be constant */
			piop->itop.l = tv1.l;		/* value */
			piop->itrl = rv1;			/* relocation value */

			if (iop != bos)
			{							/* push up the rest of the tree... */
				i = iop + 2 - bos;
				pitr = piop + 2;
				for (; i < itr; i++)
				{
					piop++;
					pitr++;
					piop->swd1 = pitr->swd1;
					piop->itop.p = pitr->itop.p;
				}
			}
			itr -= 2;
			goto exct1;
		}
	}

	/* check for unary minus and unary plus */
	if (tree[bos + 1].itty != ITSP && tree[bos].itop.u.loword == '?')
	{
		exerr();
		return;
	}
	if (tree[bos + 1].itty != ITSP || tree[bos].itty == ITSP)
	{
		reloc = ABS;
		ival.l = 0;
		itype = ITCN;
		return;
	}

	if (tree[bos + 1].itop.u.loword != '?')
	{									/* end of statement */
		if (tree[bos + 1].itop.u.loword != '+')
		{								/* ignore unary plus */
			if (tree[bos + 1].itop.u.loword != '-')
			{							/* invalid operator */
				exerr();
				return;
			}
			tree[bos + 1].itop.l = -gval(&tree[bos]);
			tree[bos + 1].itty = ITCN;
			tree[bos + 1].itrl = tree[bos].itrl;
			bos++;
			itr--;
			goto exct1;
		}
	}
	/* send results back to caller */
	if ((itype = tree[bos].itty) == ITCN)
	{
		ival.l = gval(&tree[bos]);
	} else
	{
		ival.p = tree[bos].itop.p;
		if (itype == ITSY && !(ival.ptrw2->flags & SYDF))
		{								/* undef symbol */
			reloc = ABS;
			ival.p = 0;
			itype = ITCN;
			return;
		}
	}
	get_val(tree[bos].itrl);
}


/*
 *if defined symbol get value and say constant
 * except for externals and equated registers
 */
VOID get_val(P(int) reloc_val)
PP(int reloc_val;)
{
	if (itype == ITSY && (ival.ptrw2->flags & (SYXR | SYER)) == 0)
	{
		if (ival.ptrw2->flags & SYRA)	/* get symbol relocation factor */
			reloc = DATA;
		else if (ival.ptrw2->flags & SYRO)
			reloc = TEXT;
		else if (ival.ptrw2->flags & SYBS)
			reloc = BSS;
		else
			reloc = ABS;
		ival.l = ival.ptrw2->vl1.l;			/* symbol vaue */
		itype = ITCN;					/* constant */
	} else if (itype == ITSY && ival.ptrw2->flags & SYXR)
	{									/* external symbol */
		fixext(ival.ptrw2);
		reloc = EXTRN;
	} else
		reloc = reloc_val;				/* relocation value of item */
}


VOID exerr(NOTHING)
{
	uerr(6);
	ival.l = 0;
	itype = ITCN;
	reloc = ABS;
}


/*
 * get precedence of a operator
 *  call with
 *      operator
 *  returns
 *      precedence
 */
int gprc(P(int) dprc)
PP(int dprc;)
{
	switch (dprc)
	{
	case '+':
	case '-':
	case '&':							/* and */
	case '!':							/* or */
	case '^':							/* exclusive or */
		return PPM;

	case '/':
	case '*':
	case '<':							/* left shift */
	case '>':							/* right shift */
		return PMD;

	case '(':
		if (lastopr)
			return PLP;
		break;

	case ')':
		if (!prcnt)						/* no left parens */
			break;
		return PRP;

	}
	return PEE;							/* end of expression */
}


/*
 * get value from an it format item
 *  call with
 *      address of it format item
 *  returns
 *      the value
 *      relocation value in rval
 *  calls uerr if it cant get a value
 */
long gval(P(struct it *) avwrd)
PP(struct it *avwrd;)
{
	register struct it *vwrd;
	register struct symtab *p;

	vwrd = avwrd;
	if (vwrd->itty == ITCN)
	{									/* constant */
		rval = vwrd->itrl;
		return vwrd->itop.l;			/* value */
	}
	if (vwrd->itty != ITSY)
	{
		uerr(6);
		rval = ABS;
		return 0;
	}
	p = vwrd->itop.ptrw2;
	if (p->flags & SYXR)
	{									/* external reference */
		fixext(p);
		return 0;
	}
	if ((p->flags & SYDF) != SYDF || (p->flags & SYER))
	{
		uerr(6);
		rval = ABS;
		return 0;
	}
	rval = (p->flags & SYRA) ? DATA : (p->flags & SYRO)	/* reloc of item */
		? TEXT : (p->flags & SYBS) ? BSS : ABS;
	return p->vl1.l;
}


/*
 * get items for expression evaluator (pass one)
 *  returns:
 *      item type in itype
 *      item value in ival
 *      item in it format in exitm
 */
VOID p1gi(NOTHING)
{
	if (fcflg)							/* used item so must pass it */
		gterm(TRUE);
	if (!fcflg && ckspc(fchr) == 1)
	{
		fcflg = 1;						/* just pass first character */
		itype = ITSP;					/* special char */
		ival.l = fchr;					/* value is the char */
	} else
	{									/* get a whole term */
		fcflg = 0;
		gterm(TRUE);					/* get a term */
		if (itype == ITSY)
		{								/* got a symbol */
			ival.ptrw2 = lemt(FALSE, sirt);	/* look up in main table */
			if (ival.ptrw2 == lmte)		/* not there before */
				mmte();					/* put it in table */
		} else if (itype == ITCN)
		{
			exitm.itrl = (char)reloc; /* XXX */
		}
	}
	exitm.itty = (char)itype; /* XXX */
	exitm.itop.p = ival.p;
}


/*
 * get items for expression evaluator (pass 2)
 * returns:
 *      item type in itype
 *      item value in ival
 *      item in it format in exitm
 */
VOID p2gi(NOTHING)
{
	if (pitw == pnite)
	{									/* end of statement */
		itype = ITSP;
		ival.l = ' ';						/* blank */
		exitm.itty = (char)itype; /* XXX */
		exitm.itop.p = ival.p;
		return;
	}

	if ((itype = pitw->itty) == ITPC)
	{
		pitw->itop.l = loctr;
		if (p2flg || format == 6)
			itype = pitw->itty = ITCN;
	}
	ival.p = pitw->itop.p;					/* value */
	exitm.swd1 = pitw->swd1;
	exitm.itop.p = ival.p;
	pitw++;
}


/*
 *check for a special character
 *  call with
 *      character to check
 *  returns:
 *      0 => character is number or letter
 */
int ckspc(P(int) acksc)
PP(int acksc;)
{
	register short cksc;

	cksc = acksc;
	if (isalnum(cksc))
		return 0;
	return strindex("_~*.@$%\'", cksc) != -1 ? 0 : 1;
}


/* generate new relocation for op + op */
int ckrl1(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv1 == rv2)
		return rv1;
	if (rv1 == ABS || rv2 == ABS)
		return rv1 + rv2;				/* the one that is not ABS */
	uerr(27);
	return ABS;
}


/* generate new relocation for op - op */
int ckrl2(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv2 == EXTRN)
		uerr(26);
	if (rv1 == rv2)
		return ABS;
	if (rv2 == ABS)
		return rv1 + rv2;
	uerr(27);
	return ABS;
}


/* generate new relocation for op / * & | < > ^ ! op */
int ckrl3(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv1 != ABS || rv2 != ABS)
		uerr(27);
	return ABS;
}


VOID fixext(P(struct symtab *) p)
PP(struct symtab *p;)
{
	if (extflg)
		uerr(36);						/* two externals in expr */
	extflg++;
	extref = p->vextno;					/* get external # */
	rval = EXTRN;
	itype = ITCN;
	ival.p = NULL;
}
