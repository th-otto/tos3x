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
static struct it exitm;					/* expression item */
static int prcnt;						/* paren count */
static int rval;						/* relocation value */
static int lpflg;
static int lastopr;						/* last token was operator when set */

static struct it *piop, *pitr;
static short iop, itr;
static struct it opstk[OPSTLEN];			/* operator stack */
static struct it tree[TREELEN];				/* operand stack */


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


/* generate new relocation for op + op */
static int ckrl1(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv1 == rv2)
		return rv1;
	if (rv1 == ABS)
		return rv2;				/* the one that is not ABS */
	if (rv2 == ABS)
		return rv1;				/* the one that is not ABS */
	uerr(27);	/* relocation error */
	return ABS;
}


/* generate new relocation for op - op */
static int ckrl2(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv2 == EXTRN)
		uerr(27);	/* relocation error */
	if (rv1 == rv2)
		return ABS;
	if (rv2 == ABS)
		return rv1;
	uerr(27); /* relocation error */
	return ABS;
}


/* generate new relocation for op / * & | < > ^ ! op */
static int ckrl3(P(int) rv1, P(int) rv2)
PP(int rv1;)
PP(int rv2;)
{
	if (rv1 != ABS || rv2 != ABS)
		uerr(27); /* relocation error */
	return ABS;
}


static VOID fixext(P(struct symtab *) p)
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


/*
 * if defined symbol get value and say constant
 * except for externals and equated registers
 */
static VOID get_val(P(int) reloc_val)
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
		ival.l = ival.ptrw2->vl1;		/* symbol value */
		if (itype != ITCW)
			itype = ITCN;				/* constant */
	} else if (itype == ITSY && ival.ptrw2->flags & SYXR)
	{									/* external symbol */
		fixext(ival.ptrw2);
		reloc = EXTRN;
	} else
	{
		reloc = reloc_val;				/* relocation value of item */
	}
}


static VOID exerr(NOTHING)
{
	uerr(6); /* illegal expr */
	ival.l = 0;
	itype = ITCN;
	reloc = ABS;
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
static long gval(P(struct it *) avwrd)
PP(struct it *avwrd;)
{
	register struct it *vwrd;
	register struct symtab *p;

	vwrd = avwrd;
	if (vwrd->itty == ITCN || vwrd->itty == ITCW)
	{									/* constant */
		rval = vwrd->itrl;
		return vwrd->itop.l;			/* value */
	}
	if (vwrd->itty != ITSY)
	{
		uerr(6); /* illegal expr */
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
		uerr(6); /* illegal expr */
		rval = ABS;
		return 0;
	}
	rval = (p->flags & SYRA) ? DATA : (p->flags & SYRO)	/* reloc of item */
		? TEXT : (p->flags & SYBS) ? BSS : ABS;
	return p->vl1;
}


/* collapse the tree into one entry */
static VOID collapse(NOTHING)
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
					low = tv2.l;
					tv1.l <<= low;
					break;
				case '>':				/* right shift */
					low = tv2.l;
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

	if ((tree[bos].itty == ITCN || tree[bos].itty == ITCW || tree[bos].itty == ITSY) &&
		tree[bos + 1].itty == ITSY &&
		(tree[bos + 1].itop.ptrw2->flags & SYER))
	{
		ival.l = gval(&tree[bos]);
		if (tree[bos + 1].itop.ptrw2->vl1 == WORD_ID)
		{
			itype = tree[bos].itty = ITCW;
			reloc = ABS;
			return;
		}
		if (tree[bos + 1].itop.ptrw2->vl1 == LONG_ID)
		{
			itype = tree[bos].itty = ITCN;
			return;
		}
	}
	
	/* check for unary minus and unary plus */
	if (tree[bos + 1].itty != ITSP && tree[bos].itop.l == '?')
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

	if (tree[bos + 1].itop.l != '?')
	{									/* end of statement */
		if (tree[bos + 1].itop.l != '+')
		{								/* ignore unary plus */
			if (tree[bos + 1].itop.l != '-')
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
	if ((itype = tree[bos].itty) == ITCN || itype == ITCW)
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
 * get precedence of a operator
 *  call with
 *      operator
 *  returns
 *      precedence
 */
static int gprc(P(int) dprc)
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
	opstk[0].itop.l = '?';
	lastopr = 1;

	/* get an input item */
	for (;;)
	{
		if (itr >= TREELEN - 2)
		{
			rpterr(_("expr tree overflow"));
			asabort();
		}
		if (iop >= OPSTLEN - 1)
		{
			rpterr(_("expr opstk overflow"));
			asabort();
		}
		(*iploc) ();					/* get an input term */
		if (itype == ITPC)
			return;
		starmul = 0;					/* * is location counter */

		/* special character */
		if (itype == ITSP)
		{
			i = ival.l;
			ipr = gprc(i);				/* get precedence of character */
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

			while (ipr <= gprc(i = piop->itop.l))
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
	}

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


/*
 * index - find the index of a character in a string
 *      This is identical to Software Tools index.
 */
static int strindex(P(const char *) str, P(char) chr)
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


/*
 *check for a special character
 *  call with
 *      character to check
 *  returns:
 *      0 => character is number or letter
 */
static int ckspc(P(int) acksc)
PP(int acksc;)
{
	register int cksc;

	cksc = acksc;
	if (isalnum(cksc))
		return 0;
	return strindex("_~*.@$%\'", cksc) != -1 ? 0 : 1;
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
			{
				mmte();					/* put it in table */
			}
		} else if (itype == ITCN || itype == ITCW)
		{
			exitm.itrl = reloc;
		}
	}
	exitm.itty = itype;
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
		exitm.itty = itype;
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
