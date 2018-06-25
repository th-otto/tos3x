/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "cgen.h"


/*
 * ccexpr - compute constant expression
 * Evaluates constant expressions, including ?: and relationals
 */
static int ccexpr(P(struct tnode **) tpp)								/* returns 1 if changes, 0 otherwise */
PP(struct tnode **tpp;)						/* pointer to tree */
{
	register struct tnode *ltp, *rtp, *tp;
	register short op, anyfloat;
	register int32_t rval, lval;
	short anylong, lconst;

	rval = 0; /* quiet compiler */
	tp = *tpp;
	op = tp->t_op;
	if (LEAFOP(op))
		return 0;
	anylong = anyfloat = 0;
	ltp = tp->t_left;
	if (ltp->t_op == CFLOAT)
	{
		lval = ltp->t_lvalue;
		anyfloat++;
	} else if ((ltp = constant(ltp, &anylong)) != NULL)
	{
		lval = (anylong) ? ltp->t_lvalue : ltp->t_value;
	} else
	{
		return 0;
	}
	if (BINOP(op))
	{
		rtp = tp->t_right;
		if (op == QMARK)
		{
			ltp = rtp->t_left;
			rtp = rtp->t_right;
			if (lconst && (ltp->t_op != CLONG || rtp->t_op != CLONG))
				return 0;
			if (!lconst && (ltp->t_op != CINT || rtp->t_op != CINT))
				return 0;
			if (lconst)
				ltp->t_lvalue = (lval) ? ltp->t_lvalue : rtp->t_lvalue;
			else
				ltp->t_value = (lval) ? ltp->t_value : rtp->t_value;
			*tpp = ltp;
			return 1;
		}
		if (rtp->t_op == CFLOAT)
		{
			anyfloat++;
			rval = rtp->t_lvalue;
			switch (op)
			{
			case MULT:
			case DIV:
			case MOD:
			case LSH:
			case RSH:
			case XOR:
				return 0;
			}
		} else if ((rtp = constant(rtp, &lconst)) != NULL)
		{
			rval = (lconst) ? rtp->t_lvalue : rtp->t_value;
			anylong += lconst;
		} else
		{
			return 0;
		}
	}
	
	switch (op)
	{
	case ADD:
		lval += rval;
		break;

	case SUB:
		lval -= rval;
		break;

	case MULT:
		lval *= rval;
		break;

	case DIV:
		lval /= rval;
		break;

	case MOD:
		lval %= rval;
		break;

	case RSH:
		lval >>= rval;
		break;

	case LSH:
		lval <<= rval;
		break;

	case XOR:
		lval ^= rval;
		break;

	case GREAT:
		lval = (lval > rval);
		break;

	case GREATEQ:
		lval = (lval >= rval);
		break;

	case LESS:
		lval = (lval < rval);
		break;

	case LESSEQ:
		lval = (lval <= rval);
		break;

	case UMINUS:
		lval = -lval;
		break;

	case COMPL:
		lval = ~lval;
		break;

	case NOT:
		lval = !lval;
		break;

	case OR:
		lval |= rval;
		break;

	case AND:
		lval &= rval;
		break;

	default:
		return 0;
	}
	if (anylong)
		ltp = lcnalloc(LONG, lval);
	else if (anyfloat)
		ltp = fpcnalloc(FLOAT, lval);
	else
		ltp->t_value = lval;
	*tpp = ltp;
	return 1;
}


/* chklong - check for tree being a long */
static int chklong(P(struct tnode *) tp)								/* returns 1 if long, 0 otherwise */
PP(struct tnode *tp;)
{
	if (tp->t_op == CLONG)
		return 1;
	return LONGORPTR(tp->t_type);
}


/*
 * power2 - do multiply and divide by powers of two
 *      This changes multiplies and divides by constant powers of two
 *      to shifts.
 */
static int power2(P(struct tnode **) tpp)
PP(struct tnode **tpp;)
{
	register struct tnode *p;
	register short op;
	register struct tnode *tp;
	short lconst;
	short value;

	tp = *tpp;
	if ((p = constant(tp->t_right, &lconst)) != NULL)
	{
		if (!lconst && ((value = onebit((int32_t) p->t_value)) < 0))
			return 0;
		if (lconst && ((value = onebit(p->t_lvalue)) < 0))
			return 0;
		op = tp->t_op;
		/* THIS FIX MUST WAIT FOR UNSIGNED LONG IMPLEMENTATION........... */
		if (op != MULT && op != EQMULT && !UNSIGN(tp->t_left->t_type))
			return 0;
		switch (op)
		{
		case MULT:
			op = LSH;
			break;

		case EQMULT:
			op = EQLSH;
			break;

		case DIV:
			op = RSH;
			break;

		case EQDIV:
			op = EQRSH;
			break;

		case MOD:
			op = AND;
			if (!lconst)
				value = p->t_value - 1;
			else
				value = p->t_lvalue - 1;
			break;

		case EQMOD:
			op = EQAND;
			if (!lconst)
				value = p->t_value - 1;
			else
				value = p->t_lvalue - 1;
			break;

		default:
			return 0;
		}								/* end of case */
		tp->t_op = op;
		if (lconst)
			p->t_op = CINT;				/* assure constant int.... */
		p->t_value = value;
#ifdef DEBUG
		if (oflag > 1)
			oprintf("lconst %d op = %d value = %d\n", lconst, op, value);
#endif
		return 1;
	}
	return 0;
}


/*
 * multop - handle multiplicative operators
 *      This checks for powers of two optimizations, then for a hard
 *      long operation.
 */
static int multop(P(struct tnode **)tpp)
PP(struct tnode **tpp;)
{
	register struct tnode *tp, *rtp;
	register short change;

	tp = *tpp;
	PUTEXPR(oflag, "multop", *tpp);
	if ((change = power2(tpp)) != 0)
		tp = *tpp;
	if (chklong(rtp = tp->t_right) || chklong(tp->t_left))
	{
		switch (tp->t_op)
		{
		case MULT:
			tp->t_op = LMULT;
			break;

		case DIV:
		case MOD:
			if (chklong(tp->t_left))
			{							/* only hard if divisor is long */
				if (rtp->t_type == INT)	/* only occurs if changed !! */
					break;
				tp->t_op += (LDIV - DIV);
				if (rtp->t_op == CLONG)
					rtp->t_op = DCLONG;
			}
			break;

		case EQDIV:
		case EQMOD:
			if (!chklong(rtp))
				break;
			if (rtp->t_op == CLONG)
				rtp->t_op = DCLONG;
			/* fall through */
		case EQMULT:
			tp->t_op += (LMULT - EQMULT);
			*tpp = tnalloc(ASSIGN, tp->t_type, 0, 0, tcopy(tp->t_left, A_DOPOST), tcopy(tp, A_DOPRE));
			change++;
			break;
		}
	}
	PUTEXPR(oflag, "after multop ", tp);
	return change;
}


/*
 * optim - optimize expression tree
 *      This takes expression tree and performs the following
 *      translations:  folds auto names to accesses off the local
 *      environment pointer, performs mostly "machine-independent"
 *      optimizations, such as multiply by zero and one, etc.,
 *      turns field accesses into and's and or's, etc.
 */
static int optim(P(struct tnode **) tpp)
PP(struct tnode **tpp;)
{
	register struct tnode *ltp, *rtp, *tp;
	register struct tnode *p;
	register short i, changes, op, tmp;
	short lconst, tlc;
	int32_t cval;							/* handle all integer constants as longs */

	cval = 0;
	tp = *tpp;
	if (tp->t_op != STASSIGN && (tp->t_type & ~TYPE))
	{
		tp->t_type &= TYPE;
		tp->t_type |= POINTER;
	}
	if (LEAFOP(op = tp->t_op))
		return 0;
	lconst = changes = 0;
	if (BINOP(op))
		changes += optim(&tp->t_right);
	changes += optim(&tp->t_left);
	for (; !LEAFOP(op = tp->t_op); changes++)
	{
#ifdef DEBUG
		if (oflag > 1)
			oprintf("optim: op = %d, changes %d\n", op, changes);
#endif
		PUTEXPR(oflag, "optim", tp);
		*tpp = tp;
		ltp = tp->t_left;
		if (BINOP(op))
		{
			rtp = tp->t_right;
			if (tp->t_type == CHAR)
				tp->t_type = INT;
			p = constant(rtp, &lconst);
		} else
		{
			p = constant(ltp, &lconst);
			rtp = NULL;
		}
		if (p)
			cval = (lconst) ? p->t_lvalue : p->t_value;
		switch (op)
		{
		case ADD:
			if (p)
			{
				if (!cval)
				{
					tp = ltp;
					continue;
				}
				if (ltp->t_op == ADDR)
				{
					ltp->t_left->t_offset += cval;
					tp = ltp;
					continue;
				}
			}
			break;

		case ASSIGN:
			if (rtp->t_op == ADD && indexreg(rtp->t_left) && rtp->t_right->t_op == CINT)
			{
				ltp = rtp->t_left;
				ltp->t_sc = REGOFF;
				ltp->t_offset += rtp->t_right->t_value;
				tp->t_right = ltp;
				tp->t_op = EQADDR;
				continue;
			}
			if (rtp->t_op == LONG2I && (rtp->t_left->t_op == LDIV || rtp->t_left->t_op == LMOD))
			{
				rtp = rtp->t_left;
				if (rtp->t_right->t_op == DCLONG)
				{
					if (rtp->t_right->t_lvalue < 0)
						break;
					if (rtp->t_right->t_lvalue > 0xffff)
						break;
					rtp->t_right->t_op = CINT;
					rtp->t_right->t_type = INT;
				} else if (rtp->t_right->t_op == INT2L)
					rtp->t_right = rtp->t_right->t_left;
				rtp->t_op -= (LDIV - DIV);
				tp->t_right = rtp;
				continue;
			}
			/* fall through */
		case EQSUB:
		case EQADD:
		case EQAND:
		case EQOR:
			if (p && ltp->t_type == CHAR)
			{
			  ctrunc:
				if (lconst)
				{
					tmp = rtp->t_lvalue;
					rtp->t_op = CINT;
				} else
					tmp = rtp->t_value;
				if (tmp > 255)
				{
					warning("value assigned to char truncated");
					rtp->t_value = tmp & 0xff;
				} else if (tmp < -128)
				{
					warning("value assigned to char truncated");
					rtp->t_value = ((char) (tmp & 0xff));
				}
				break;
			}
			break;

		case SUB:
			if (p)
			{
				if (!cval)
				{
					tp = ltp;
					continue;
				}
				tp->t_op = ADD;
				SETVAL(lconst, p, -cval);
				continue;
			}
			if ((p = constant(ltp, &tlc)) != NULL && CONSTZERO(tlc, p))
			{
				tp->t_op = UMINUS;
				tp->t_left = ltp = rtp;
				continue;
			}							/* 0-x = -x */
			break;

		case DIV:
		case EQDIV:
			if (p)
			{
				if (!cval)
				{						/* X/0->error */
					error("divide by zero");
					tp = rtp;
					continue;
				}
				if (cval == 1)
				{
					tp = ltp;
					continue;
				}
				if (cval == -1)
				{
					tp->t_op = (tp->t_op == DIV ? UMINUS : EQNEG);
					continue;
				}
			}
			if ((p = constant(ltp, &tlc)) != NULL && CONSTZERO(lconst, p))
			{
				tp = ltp;
				continue;
			}
			if (multop(tpp))
			{
				tp = *tpp;
				continue;
			}
			break;

		case EQMOD:
		case MOD:
			if (p)
			{
				if (!cval)
				{						/* X%0->error */
					error("modulus by zero");
					tp = rtp;
					continue;
				}
				if (cval == 1)
				{						/* X%1->0 */
					SETVAL(lconst, p, 0);
					if (op == EQMOD)
						tp->t_op = ASSIGN;
					else
						tp = rtp;
					continue;
				}
			}
			if ((p = constant(ltp, &tlc)) != NULL && CONSTZERO(lconst, p))
			{
				tp = ltp;
				continue;
			}
			if (multop(tpp))
			{
				tp = *tpp;
				continue;
			}
			break;

		case MULT:
		case EQMULT:
			if (p)
			{
				/*  Don't do this optimization !!! case: 0 * f()
				   if( !cval ) {
				   if (op == EQMULT)
				   tp->t_op = ASSIGN;
				   else
				   tp = rtp;
				   continue;
				   }    Problem: side effects of object multiplied with !!!
				 */
				if (cval == 1L)
				{
					tp = ltp;
					continue;
				}
				if (cval == -1L)
				{
					tp->t_op = (tp->t_op == MULT ? UMINUS : EQNEG);
					continue;
				}
			}
			if (multop(tpp))
			{
				tp = *tpp;
				continue;
			}
			break;

		case EQUALS:
		case NEQUALS:
			if (p && ((i = onebit(cval)) >= 0) && ltp->t_op == AND && (rtp = constant(ltp->t_right, &tlc)))
				if ((!tlc && i == onebit((int32_t) rtp->t_value)) || (tlc && i == onebit(rtp->t_lvalue)))
				{
					tp->t_op = invrel[tp->t_op - EQUALS];
					p->t_value = 0;
					p->t_op = CINT;		/* assure const short on bit value... */
					continue;
				}
			break;

		case GREATEQ:
			if (p && !cval && UNSIGN(ltp->t_type))
			{
				SETVAL(lconst, p, 1);
				tp = p;
				continue;
			}
			break;

		case LESS:
			if (p && !cval && UNSIGN(ltp->t_type))
			{
				tp = p;
				continue;
			}
			break;

		case AND:
			if (p)
			{
				/*  Don't do this optimization !!! case: 0 & f()
				   if( !cval ) {
				   tp = rtp;
				   continue;
				   }    Problem: side effects of object anded with !!!
				 */
				if (cval == -1L)
				{
					tp = ltp;
					continue;
				}
			}
			break;

		case OR:
			if (p)
			{
				if (!cval)
				{
					tp = ltp;
					continue;
				}
				if (cval == -1)
				{
					tp = rtp;
					continue;
				}
			}
			break;

		case EQXOR:
		case XOR:
			if (p)
			{
				if (!cval)
				{
					tp = ltp;
					continue;
				}
				if (cval == -1)
				{
					tp->t_op = (tp->t_op == XOR ? COMPL : EQNOT);
					continue;
				}
				if (ltp->t_type == CHAR)
					goto ctrunc;
			}
			break;

		case LSH:
		case EQLSH:
			if (tp->t_type == LONG && rtp->t_op == INT2L && UNSIGN(rtp->t_left->t_type))
			{
				tp->t_right = rtp->t_left;
				continue;
			}
			/* fall through */
		case RSH:
		case EQRSH:
			if (p)
			{
				if (!cval)
				{
					tp = ltp;
					continue;
				}
				if (p != rtp)
				{
					tp->t_right = p;
					continue;
				}
			}
			break;

		case ADDR:
			if (ltp->t_op == INDR)
			{
				tp = ltp->t_left;
				continue;
			}
			if (ltp->t_op == SYMBOL && ltp->t_sc == REGOFF)
			{
				tp->t_op = ADD;
				tp->t_right = cnalloc(INT, (short) ltp->t_offset);
				ltp->t_type = tp->t_type;
				ltp->t_sc = REGISTER;
				ltp->t_offset = 0;
				continue;
			}
			break;

		case INDR:
			p = ltp->t_left;
			switch (ltp->t_op)
			{
			case ADDR:
				tp = p;
				continue;

			case CINT:
				tp = snalloc(tp->t_type, CINDR, (int32_t) ltp->t_value, 0, 0);
				continue;

			case CLONG:
				tp = snalloc(tp->t_type, CLINDR, ltp->t_lvalue, 0, ltp->v.w.loword);
				continue;

			case CFLOAT:
				tp = snalloc(tp->t_type, CFINDR, ltp->t_lvalue, 0, ltp->v.w.loword);
				continue;

			case SYMBOL:
				if (indexreg(ltp))
				{
					ltp->t_sc = REGOFF;
					ltp->t_type = tp->t_type;
					ltp->t_offset = 0;
					tp = ltp;
					continue;
				}
				break;

			case ADD:
				if (p->t_op == SYMBOL && p->t_sc == REGOFF &&
					(rtp = constant(ltp->t_right, &tlc)) && !tlc && !ISPOINTER(p->t_type))
				{
					p->t_offset += rtp->t_value;
					tp = p;
					continue;
				}						/* long constant not valid offset !!!! */
				if (indexreg(p))
				{
					if ((rtp = constant(ltp->t_right, &tlc)) != NULL && !tlc)
					{
						/*
						 * This combines an address register and a constant into a register
						 * offset.  This relies on 68000 Addressing scheme somewhat.
						 */
						p->t_sc = REGOFF;
						p->t_type = tp->t_type;
						p->t_offset += rtp->t_value;
						tp = p;
						continue;
					}
					if (!lflag && ltp->t_right->t_op == ADDR)
					{
						/*
						 * We can fold *(An+&expr) into *(&expr(An)), but note that &expr
						 * must be 16 bits for 68000, hence we can only do this if short
						 * addresses are enabled.  Note that the storage classes are mapped:
						 * EXTERNAL->EXTOFF, STATIC->STATOFF, REGISTER->REGOFF
						 */
						ltp = ltp->t_right->t_left;
						ltp->t_sc += (EXTOFF - EXTERNAL);
						ltp->t_type = tp->t_type;
						tp = ltp;
						continue;
					}
				}
				break;
				/*
				 * --X and X++ can be folded into -(An) and (An)+ 68000 instructions
				 */
			case PREDEC:
			case POSTINC:
				if (indexreg(p) && ltp->t_type == p->t_type)
				{
					p->t_op = (ltp->t_op == PREDEC ? AUTODEC : AUTOINC);
					p->t_type = tp->t_type;
					tp = p;
					continue;
				}
			}
			break;

		case NOT:
			if (RELOP(ltp->t_op))
			{							/* !(X>Y)->X<=Y */
				tp = ltp;
				tp->t_op = invrel[tp->t_op - EQUALS];
				continue;
			}
			break;

		case UMINUS:
		case COMPL:
			if (tp->t_type == CHAR)
				tp->t_type = INT;
			if (tp->t_op == ltp->t_op)
			{
				tp = ltp->t_left;
				continue;
			}
			if (ltp->t_op == INT2L)
			{							/* ~(INT2L X)->INT2L (~X) */
				ltp->t_op = tp->t_op;
				tp->t_op = INT2L;
				ltp->t_type = INT;
				continue;
			}
			if (ltp->t_op == LONG2I)
			{							/* ~(LONG2I X)->LONG2I (~X) */
				ltp->t_op = tp->t_op;
				tp->t_op = LONG2I;
				ltp->t_type = LONG;
				continue;
			}
			break;

		case INT2L:
			if (ltp->t_op == MULT)
			{							/* INT2L (X*Y)->X*Y */
				ltp->t_type = (ltp->t_type & (~TYPE)) | LONG;
				tp = ltp;
				continue;
			}
			break;
		}
		if (!ccexpr(tpp))
			break;
		tp = *tpp;
	}
	PUTEXPR(oflag, "optim exit", tp);
	*tpp = tp;
	return changes;
}


/*
 * fixbfield - fix bit field operators
 * Fixes bit field assignment and normal usage
 */
static struct tnode *fixbfield(P(struct tnode *) tp)						/* returns pointer to fixed tree */
PP(struct tnode *tp;)						/* pointer to tree */
{
	register struct tnode *btp, *stp;
	struct tnode *rhstp, *asgtp;
	register short foff, fmask, op, flen;

	if (LEAFOP(op = tp->t_op))
		return tp;
	if (ISASGOP(op) && tp->t_left->t_op == BFIELD)
	{
		tp->t_right = fixbfield(tp->t_right);
		btp = tp->t_left;				/* pointer to BFIELD node */
		stp = btp->t_left;				/* pointer to son of BFIELD node */
		foff = BFOFFS(btp->t_su);
		flen = BFLEN(btp->t_su);
		fmask = (1 << flen) - 1;
		if (tp->t_right->t_op == CINT && (op == ASSIGN || op == EQXOR))
		{
			if (op == EQXOR)
			{
				tp->t_left = stp;
				tp->t_right->t_value <<= foff;
				return tp;
			}
			if (!tp->t_right->t_value)
			{
				tp->t_op = EQAND;
				tp->t_left = stp;
				tp->t_right->t_value = ~(fmask << foff);
				return tp;
			}
			if ((tp->t_right->t_value & fmask) == fmask)
			{
				tp->t_op = EQOR;
				tp->t_left = stp;
				tp->t_right->t_value = fmask << foff;
				return tp;
			}
		}
		if (fmask == -1)
		{
			tp->t_left = stp;
		} else
		{
			asgtp = tcopy(stp, A_DOPOST);	/* for lhs of assign */
			op = tp->t_op;
			tp = tp->t_right;				/* pointer to expression */
			if (op != ASSIGN)
			{								/* =op operator? */
				rhstp = tcopy(stp, 0);
				btp = fixbfield(btp);
				tp = tnalloc(op - (EQADD - ADD), INT, 0, 0, tcopy(btp, A_DOPRE), tp);
			} else
				rhstp = tcopy(stp, A_DOPRE);	/* for rhs of assign */
			tp = tnalloc(AND, INT, 0, 0, tp, cnalloc(INT, fmask));
			tp = tnalloc(LSH, INT, 0, 0, tp, cnalloc(INT, foff));
			btp = tnalloc(AND, INT, 0, 0, rhstp, cnalloc(INT, ~(fmask << foff)));
			tp = tnalloc(OR, INT, 0, 0, btp, tp);
			tp = tnalloc(ASSIGN, INT, 0, 0, asgtp, tp);
		}
	} else if (op == BFIELD)
	{
		foff = BFOFFS(tp->t_su);
		fmask = (1 << BFLEN(tp->t_su)) - 1;
		tp = tnalloc(RSH, INT, 0, 0, tp->t_left, cnalloc(INT, foff));
		tp = tnalloc(AND, INT, 0, 0, tp, cnalloc(INT, fmask));
	} else
	{
		tp->t_left = fixbfield(tp->t_left);
		if (BINOP(op))
			tp->t_right = fixbfield(tp->t_right);
	}
	return tp;
}


/*
 * harder - test one sub-expression for being "harder" than another
 *      This requires some special finagling for registers.  The reason
 *      for this is that the code skeletons produce better code if the
 *      register is on the left.  Also note that allowing an address
 *      register on the right can have disastrous effects for AND and OR.
 *      The basic poshort is: don't mess with this routine unless you're
 *      100% sure you understand the ramifications...
 */
static int harder(P(struct tnode *) tp, P(struct tnode *) ntp)							/* returns 1 if tp > ntp, else 0 */
PP(struct tnode *tp;)
PP(struct tnode *ntp;)
{
	short lconst;

	if (ntp->t_su == SU_VHARD)
		return 0;
	if (tp->t_su == SU_VHARD)
		return 1;
	if (ISREG(ntp))
		return 0;
	if (ISREG(tp))
		return 1;
	if (constant(ntp, &lconst))
		return 1;
	if (constant(tp, &lconst))
		return 0;
	return tp->t_su > ntp->t_su;
}


/*
 * addtree - collect commutable sub-trees for commute
 *      This recurses down the sub-trees looking for groups of
 *      commutable operators.  It collects the sub-trees and their
 *      parent nodes for commute.
 */
static VOID addtree(P(struct tnode *) tp, P(struct tnode ***) clist, P(struct tnode ***) plist)
PP(struct tnode *tp;)						/* pointer to tree */
PP(struct tnode ***clist;)					/* commutable sub-trees */
PP(struct tnode ***plist;)					/* parent nodes of sub-trees */
{
	register struct tnode ***p,	***c;

	c = clist;
	p = plist;
	if (tp->t_op == tp->t_left->t_op)
		addtree(tp->t_left, c, p);
	else
		*(*c)++ = tp->t_left;
	if (tp->t_op == tp->t_right->t_op)
		addtree(tp->t_right, c, p);
	else
		*(*c)++ = tp->t_right;
	*(*p)++ = tp;
}


/*
 * commute - commutes expression tree to canonical form
 *      This sorts commutable expression trees so that the most
 *      difficult expression is the left-most operand.  Note that
 *      canon assumes that commute has placed constant operands in
 *      the right sub-tree.  This also swaps relationals so that the
 *      most difficult expression is on the left.
 */
static struct tnode *commute(P(struct tnode *) tp)						/* returns commuted expression tree */
PP(struct tnode *tp;)
{
	struct tnode *clist[20], *plist[19];
	register struct tnode **p, **q, *s;
	struct tnode **plp, **clp;
	register short op;

	if (RELOP(op = tp->t_op))
	{
		s = tp->t_left;
		if (harder(tp->t_right, s))
		{
			PUTEXPR(oflag, "swaprel", tp);
			tp->t_op = swaprel[op - EQUALS];
			tp->t_left = tp->t_right;
			tp->t_right = s;
		}
	}
	if (COMMOP(op))
	{
		PUTEXPR(oflag, "commute", tp);
		clp = clist;
		plp = plist;
		addtree(tp, &clp, &plp);		/* collect comm. expressions */
		/*
		 * see if any sub-trees can also be commuted (with different operator)
		 */
		clp--;
		plp--;
		for (p = clist; p <= clp; p++)
			*p = commute(*p);
		/*
		 * this sorts the expressions in decreasing order of Sethy-Ullman
		 * values.
		 */
		for (p = clist; p <= clp; p++)
		{
			for (q = p; q <= clp; q++)
			{
				if (harder(*q, *p))
				{
					s = *q;
					*q = *p;
					*p = s;
				}
			}
		}
		/*
		 * Now, we start at the end of the list and collect any constants
		 * if possible.
		 */
		for (q = clp, p = plp; p > plist; p--)
		{
			s = *p;
			s->t_right = *q;
			s->t_left = *--q;
			if (ccexpr(p))
			{
				clp--;
				plp--;
				*q = *p;
			}
		}
		/*
		 * this takes the sorted sub-expression pointers and the pointers
		 * to the commutable operator nodes (plist) and structures the
		 * tree so that the left sub-expression is the most complex.  The
		 * code generation scheme is very sensitive to this...
		 */
		q = clist;
		p = plist;
		s = *q++;
		while (p <= plp)
		{
			(*p)->t_left = s;
			s = *p++;
			s->t_right = *q++;
			if (!LONGORPTR(s->t_type))
			{
				if (LONGORPTR(s->t_right->t_type))
					s->t_type = s->t_right->t_type;
				else if (LONGORPTR(s->t_left->t_type))
					s->t_type = s->t_left->t_type;
			}
		}
		tp = s;
		PUTEXPR(oflag, "after commute", tp);
	} else if (BINOP(op))
	{
		tp->t_left = commute(tp->t_left);
		tp->t_right = commute(tp->t_right);
	} else if (UNARYOP(op))
		tp->t_left = commute(tp->t_left);
	return tp;
}


/* constant - test for tree being a constant node */
struct tnode *constant(P(struct tnode *) tp, P(short *) lconst)				/* returns 0 or ptr to const node */
PP(struct tnode *tp;)						/* pointer to tree */
PP(short *lconst;)
{
	if (tp->t_op == CLONG)
	{
		*lconst = 1;
		return tp;
	}
	*lconst = 0;
	if (tp->t_op == CINT)
		return tp;
	if (tp->t_op == INT2L && (tp->t_left->t_op == CINT || tp->t_left->t_op == CLONG))
	{
		tp->t_op = CLONG;
		tp->t_type = LONG;
		if (tp->t_left->t_op == CLONG)
			tp->t_lvalue = tp->t_left->t_lvalue;
		else
			tp->t_lvalue = tp->t_left->t_value;
		*lconst = 1;
		return tp;
	}
	if (tp->t_op == LONG2I && (tp->t_left->t_op == CINT || tp->t_left->t_op == CLONG))
	{
		tp->t_op = CINT;
		tp->t_type = INT;
		if (tp->t_left->t_op == CLONG)
			tp->t_value = tp->t_left->t_lvalue;
		else
			tp->t_value = tp->t_left->t_value;
		return tp;
	}
	return 0;
}


/*
 * indexreg - returns whether node is an address register
 * For 68000, must be an A register
 */
int indexreg(P(struct tnode *) tp)							/* returns whether node is A reg */
PP(struct tnode *tp;)
{
	if (tp->t_op == SYMBOL && tp->t_sc == REGISTER && !(ISDREG(tp->t_reg)))
		return 1;
	return 0;
}


/*
 * onebit - returns whether constant is power of two (one bit on)
 * returns bit number or -1
 */
int onebit(P(int32_t) val)
PP(int32_t val;)
{
	register short i;

	for (i = 31; val != 0; val <<= 1, i--)
		if (val & 0x80000000)
			break;
	if (val != (int32_t)0x80000000L)
		return -1;
	return i;
}


/*
 * canon - expression canonicalization
 *      Top level tree canonicalization.  This fixes any bit field
 *      accesses, then loops on commute and optim until no more
 *      optimizations are done.
 */
struct tnode *canon(P(struct tnode *) tp)							/* returns pointer to tree */
PP(struct tnode *tp;)						/* pointer to tree to canonicalize */
{
	tp = fixbfield(tp);
	do
	{
		sucomp(tp, 0, 1);				/* need Sethy-Ullman's for commute */
		tp = commute(tp);				/* commute the tree */
	} while (optim(&tp));				/* keep trying for optimizations */
	PUTEXPR((cflag > 1), "canon return", tp);
	return tp;
}
