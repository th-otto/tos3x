/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"

#define QCAST(l,r) \
	(l->t_type == CHAR && \
	((r->t_op == SYMBOL && ((struct symnode *)r)->t_sc != REGISTER && ((struct symnode *)r)->t_sc <= STATIC) || r->t_type != CHAR))
short strassign;
short strsize;						/* save structure size for assignment */

int specops PROTO((int op, struct tnode *ltp, struct tnode *rtp));
struct tnode *cvopgen PROTO((struct tnode *tp, int type, int conv, long len, int op));
VOID tadjust PROTO((struct tnode *tp, int type, int dp, int ssp));
struct tnode *funcref PROTO((struct tnode *tp));
struct tnode *arrayref PROTO((struct tnode *tp));
int ttoconv PROTO((int type));




/*
 * This indicates if a conversion operator is needed given the types
 * of the two operands.  The upper diagonal is applied ONLY if this
 * is an assignment operator and the indexes are swapped so the lower
 * diagonal is used otherwise.
 *   INT		UNSN		LONG		DOUB		PTR
 */
char const cvmap[5][5] = {
	{ 0, UNSN_INT, LONG_INT, DOUB_INT, PTR_INT },
	{ INT_UNSN, 0, LONG_UNSN, DOUB_UNSN, PTR_UNSN },
	{ INT_LONG, UNSN_LONG, 0, DOUB_LONG, PTR_LONG },
	{ INT_DOUB, UNSN_DOUB, LONG_DOUB, 0, BADCONV },
	{ INT_PTR, UNSN_PTR, LONG_PTR, BADCONV, PTR_PTR }
};

/*
 * maketree - given operator, takes operands and builds up tree
 *		This takes the given operator, allocates a node for it
 *		and links up the subtrees on the operand stack.  A lot of
 *		work is in inserting the proper conversions.
 */
int maketree(P(int) op)
PP(int op;)									/* new root operator */
{
	register struct tnode *ltp, *rtp, *p;
	register short type, ltype, rtype, lconv, conv, pconv, docast, left, right;

#ifdef DEBUG
	if (treedebug)
		printf("maketree op = %d\n", op);
#endif
#ifndef __ALCYON__
	rtp = NULL; /* quiet compiler */
	rtype = 0;
#endif
	if (BINOP(op))
	{
		if (!(rtp = (struct tnode *) popopd()))
			return 0;
		rtype = (rtp = funcref(arrayref(rtp)))->t_type;
		PUTEXPR(treedebug, "maketree r", rtp);
	}
	if (!(ltp = (struct tnode *) popopd()))
		return 0;
	PUTEXPR(treedebug, "maketree l", ltp);
	if (op == ASM)						/* asm internal function */
		return 1;
	if (op == SIZEOF)
	{
		pushopd(cnalloc(INT, dosizeof(ltp, 1)));
		return 1;
	}
	if (op != ADDR)
	{
		ltp = arrayref(ltp);
		if (op != CALL && op != NACALL)
			ltp = funcref(ltp);
	} else
	{
		if (ltp->t_op == ADD)			/* there must be a better way... */
			if (ltp->t_left->t_op == SYMBOL && ltp->t_right->t_op == CINT)
			{
				pushopd(ltp);
				return 1;
			}
	}
	if (specops(op, ltp, rtp))
		return 1;
	ltype = ltp->t_type;
	if (LINTEGRAL(op))
		integral(ltp, LONG);
	if (RINTEGRAL(op))
		integral(rtp, LONG);
	if (LVALOP(op) && ltp->t_op != SYMBOL && ltp->t_op != INDR && ltp->t_op != BFIELD)
		error("assignable operand required");
	if (UNARYOP(op))
	{
		if (!unopeval(op, (struct lconode *)ltp))
			pushopd(tnalloc(op, ltype, ltp->t_dp, ltp->t_ssp, ltp, 0L));
		return 1;
	}
	if (ltype == STRUCT || rtype == STRUCT)
	{
		if (op == ASSIGN)
		{
			if (ltype != STRUCT)
			{
				ltp->t_type = rtp->t_type;
				ltp->t_ssp = rtp->t_ssp;
				ltp->t_dp = rtp->t_dp;
				ltype = STRUCT;
			} else if (rtype != STRUCT)
			{
				rtp->t_type = ltp->t_type;
				rtp->t_ssp = ltp->t_ssp;
				rtp->t_dp = ltp->t_dp;
				rtype = STRUCT;
			}
			left = dosizeof(ltp, 0);
			strsize = right = dosizeof(rtp, 0);
			if (left < right)
			{							/* left side smaller than right */
				warning("left side of structure assignment smaller than right");
				strsize = left;
			}
			pushopd(ltp);
			maketree(ADDR);
			pushopd(rtp);
			maketree(ADDR);
			maketree(STASSIGN);
			strassign = 1;
			return 1;
		}
		ltype = rtype = INT;
		error("illegal structure operation");
	}
	type = ltype;
	if (rtype == TYPELESS)
	{
		rtp->t_type = rtype = INT;
		lconv = conv = 0;
	} else
	{
		lconv = ttoconv(ltype);
		conv = ttoconv(rtype);
		if (ISASGOP(op) || lconv >= conv || (COMOP(op) &&	/* 4.3 not RELOP */
											ltype == INT && rtype == LONG && rtp->t_op == CLONG))
		{
			conv = cvmap[lconv][conv];
			lconv = 0;
		} else
		{
			conv = cvmap[conv][lconv];
			lconv = 1;
			type = rtype;
		}
	}
	if (ISASGOP(op))
	{
		if ((op == ASSIGN || op == FRETURN) && rtp->t_op != CINT && (conv == INT_PTR || conv == UNSN_PTR))
		{
			conv = INT_LONG;
			warning("short assigned to pointer");
		} else if (op == ASSIGN || op == CAST)
		{
			switch (conv)
			{

			case INT_PTR:
			case UNSN_PTR:
			case PTR_PTR:
			case PTR_LONG:
			case LONG_PTR:
				conv = 0;
				break;
			}
		}
	} else if (op == COLON && SUPTYPE(ltype) != 0 && ltype == rtype)
	{
		conv = 0;
	} else if (RELOP(op) && (conv == PTR_PTR || conv == LONG_PTR || conv == PTR_LONG))
	{
		conv = 0;
	} else if (BINOP(op) && (ltp->t_op == CINT || ltp->t_op == CLONG) && (rtp->t_op == CINT || rtp->t_op == CLONG))
	{
		conv = 0;
	}
	pconv = 0;
	if (RELOP(op) && ISALLTYPE(ltype) == (STRUCT | POINTER) && (conv == INT_PTR || conv == LONG_PTR))
		conv = 0;						/* short compare to struct pointer, no conversion */
	if (op == FRETURN && conv == LONG_PTR)
		conv = 0;
	if (conv == PTR_PTR)
	{
		conv = 0;
		if (op == SUB)
		{
			type = LONG;
			pconv++;
		} else if (op != FRETURN && ((ISALLTYPE(ltype) != ISALLTYPE(rtype) || ISALLTYPE(ltype) != (POINTER | CHAR))))
		{
			warning("suspect conversion operation");
		}
	}
	docast = QCAST(ltp, rtp);
	if (conv)
	{
		if (conv == BADCONV)
			error("illegal type conversion");
		else if (lconv)
			ltp = cvopgen(ltp, type, conv, psize(rtp), op);
		else
			rtp = cvopgen(rtp, type, conv, psize(ltp), op);
	}
	if (op == CAST)
	{
		if (docast)						/* predefined to handle conv/cast ops */
		{
			rtp = tnalloc(TOCHAR, CHAR, 0, 0, rtp, 0L);
		} else if ((ltype & POINTER) && rtype == INT)
		{
			rtp = cvopgen(rtp, type, INT_LONG, psize(ltp), op);
		} else if (rtp->t_type != CHAR && !conv)
		{
			rtp->t_type = ltp->t_type;
			rtp->t_ssp = ltp->t_ssp;
			if (rtp->t_op != BFIELD)
				rtp->t_dp = ltp->t_dp;
		}
		pushopd(rtp);
	}
	if (RELOP(op))
		type = INT;
	if (op != CAST && !binopeval(op, (struct lconode *)ltp, (struct lconode *)rtp))
	{
		if (BTYPE(ltype) == STRUCT || BTYPE(rtype) != STRUCT)
			p = ltp;
		else
			p = rtp;
		p = tnalloc(op, type, p->t_dp, p->t_ssp, ltp, rtp);
		pushopd(p);
	}
	if (op == SUB && (ltype & POINTER) && (rtype & POINTER) && !lconv && !conv)
	{
		warning("pointer subtraction yields a long result");
#ifdef DEBUG
		if (symdebug)
			printf("pconv = %d\n", pconv);
#endif
	}
	if (pconv && ltype != (POINTER | CHAR))
	{
		if (!(ltp = (struct tnode *) popopd()))
			return 0;
		pushopd(cvopgen(ltp, LONG, PTR_LONG, psize(ltp->t_left), op));
	}
	return 1;
}


/*
 * specops - handle special operators in building tree
 * returns 1 if op special, 0 otherwise
 */
int specops(P(int) op, P(struct tnode *) ltp, P(struct tnode *) rtp)
PP(int op;)									/* operator */
PP(struct tnode *ltp;)						/* left subtree pointer */
PP(struct tnode *rtp;)						/* right subtree pointer */
{
	register short type;

	type = ltp->t_type;
#ifdef DEBUG
	if (treedebug)
		printf("specops: op = %d type = %d\n", op, type);
#endif
	switch (op)
	{
	case 0:
		break;

	default:
		return 0;

	case APTR:							/* expr -> name */
		integral(ltp, LONG);			/* we need to turn expr into a */
		ltp->t_type = POINTER | STRUCT;	/* pointer to a struct, then use */
		pushopd(ltp);					/* expr . name stuff */
		maketree(INDR);
		ltp = (struct tnode *) popopd();	/* ltp cannot be 0 */
	case PERIOD:						/* expr . name */
		if (!(ISSTEL(rtp)))
			error("invalid structure member name");
		type = rtp->t_type;
		if (ISARRAY(type))
		{
			type = delspchk(type);
			rtp->t_dp++;
		}
		tadjust(ltp, type, rtp->t_dp, rtp->t_ssp);
		pushopd(ltp);
		maketree(ADDR);
		pushopd(cnalloc(TYPELESS, ((struct symnode *)rtp)->t_offset));
		maketree(ADD);
		if (NOTARRAY(rtp->t_type))
			maketree(INDR);
		ltp = (struct tnode *) popopd();
		if (((struct symnode *)rtp)->t_sc == BFIELDCL)		/* ltp cannot be 0 */
			ltp = tnalloc(BFIELD, type, rtp->t_dp, rtp->t_ssp, ltp, 0L);
		break;

	case QMARK:
		if (rtp->t_op != COLON)
			error("invalid ?: operator syntax");
		if (ltp->t_op == CINT && rtp->t_left->t_op == CINT && rtp->t_right->t_op == CINT)
			((struct conode *)ltp)->t_value = (((struct conode *)ltp)->t_value ? ((struct conode *)rtp->t_left)->t_value : ((struct conode *)rtp->t_right)->t_value);
		else
			ltp = tnalloc(op, rtp->t_type, 0, 0, ltp, rtp);
		break;

	case LAND:
	case LOR:
	case COMMA:						/* don't need conversions here */
		ltp = tnalloc(op, INT, 0, 0, ltp, rtp);
		break;

	case INDR:
		if (ltp->t_op == ADDR)			/* *& is null op */
			ltp = ltp->t_left;
		else
		{
			if (ISFUNCTION(type))
				error("indirection on function invalid");
			ltp = tnalloc(INDR, delspchk(type), ltp->t_dp, ltp->t_ssp, ltp, 0L);
		}
		break;

	case STASSIGN:
		ltp = tnalloc(STASSIGN, strsize, 0, 0, ltp, rtp);
		break;

	case NACALL:
	case CALL:
		if (NOTFUNCTION(type))
		{
			error("illegal call");
#ifdef DEBUG
			if (symdebug)
				printf("illegal call...................\n");
#endif
		}
		ltp = tnalloc(op, delspchk(type), ltp->t_dp, ltp->t_ssp, ltp, rtp);
		break;

	case ADDR:
		if (ltp->t_op == INDR)			/* &* is null op */
			ltp = ltp->t_left;
		else if (ltp->t_op == SYMBOL)
		{
			if (((struct symnode *) ltp)->t_sc == REGISTER)
				error("address of register");
			ltp = tnalloc(ADDR, addsp(type, POINTER), ltp->t_dp, ltp->t_ssp, ltp, 0L);
		} else
			error("& operator illegal");
		break;
	}
	pushopd(ltp);
	return 1;
}


/*
 * cvopgen - generate a conversion operator
 * Generates conversions necessary for integers, pointers and longs.
 * returns pointer to conv node
 */
struct tnode *cvopgen(P(struct tnode *) tp, P(int) type, P(int) conv, P(long) len, P(int) op)
PP(struct tnode *tp;)						/* pointer to node to do conversion */
PP(int type;)								/* type to convert to */
PP(int conv;)								/* specified conversion */
PP(long len;)								/* object length */
PP(int op;)									/* for cast operator */
{
	register struct tnode *rtp;
	register short cop;

#ifdef DEBUG
	if (treedebug)
	{
		printf("cvopgen: type=%d conv=%d op=%d ", type, conv, op);
		printf("- type=%d op=%d\n", tp->t_type, tp->t_op);
	}
#endif

#ifndef __ALCYON__
	rtp = NULL; /* quiet compiler */
#endif
	switch (conv)
	{
	case INT_PTR:
	case UNSN_PTR:
		if ((type == (POINTER | CHAR) || ISALLTYPE(type) == (POINTER | STRUCT)) && (op == CAST || op == FRETURN))
		{
			cop = INT2L;
			break;
		} else if (op == CAST || op == FRETURN)
		{
			cop = INT2L;				/* of the ptd to objects length plus */
			if (len != 1L)
			{							/* an integer to long covnversion */
				rtp = (struct tnode *) cnalloc(INT, (int) len);
				if (binopeval(MULT, (struct lconode *)tp, (struct lconode *)rtp))
					tp = popopd();
				else
					tp = tnalloc(MULT, type, 0, 0, tp, rtp);
			}
			break;
		}
	case PTR_LONG:						/* need to generate mult or div */
	case LONG_PTR:						/* of the ptd to objects length */
		if (len == 1)
			return tp;
		cop = (conv == PTR_LONG ? DIV : MULT);
		rtp = (struct tnode *) cnalloc(INT, (int) len);
		if (binopeval(cop, (struct lconode *)tp, (struct lconode *)rtp))
			return popopd();
		break;

	case INT_LONG:
		if (tp->t_op == CINT)			/* constant conversion */
			return (struct tnode *)lcnalloc(type, (long) ((struct conode *) tp)->t_value);
	case UNSN_LONG:
		cop = INT2L;
		break;

	case INT_DOUB:
	case UNSN_DOUB:
		cop = INT2F;
		break;

	case LONG_DOUB:
		cop = LONG2F;
		break;

	case DOUB_LONG:
		cop = FLOAT2L;
		break;

	case DOUB_INT:
	case DOUB_UNSN:
		cop = FLOAT2I;
		break;

	case LONG_INT:
	case LONG_UNSN:
	case PTR_INT:
	case PTR_UNSN:
		cop = LONG2I;
		break;

	default:
		error("invalid conversion");
		return tp;
	}
	return tnalloc(cop, type, 0, 0, tp, rtp);
}


/*
 * tadjust - expression tree type adjustment
 * Adjusts the types of subtrees to agree with the top of the tree.
 */
VOID tadjust(P(struct tnode *) tp, P(int) type, P(int) dp, P(int) ssp)
PP(struct tnode *tp;)						/* pointer to tree */
PP(int type;)								/* type to adjust to */
PP(int dp;)									/* dimension pointer or info */
PP(int ssp;)								/* structure pointer */
{
	register short op;

	tp->t_type = type;
	if (dp >= 0)
	{
		tp->t_dp = dp;
		tp->t_ssp = ssp;
	}
	if ((op = tp->t_op) == ADDR)
		type = delspchk(type);
	else if (op == INDR)
		type = addsp(type, POINTER);
	else if (op != ADD && op != SUB)
		return;
	tadjust(tp->t_left, type, dp, ssp);
}


/*
 * funcref - handle tree function reference
 * Turns a reference to a function into the address of the function.
 */
struct tnode *funcref(P(struct tnode *) tp)
PP(struct tnode *tp;)						/* pointer to old node */
{
	if (ISFUNCTION(tp->t_type))
		tp = tnalloc(ADDR, addsp(tp->t_type, POINTER), tp->t_dp, tp->t_ssp, tp, 0L);
	return tp;
}


/*
 * arrayref - handle tree array reference
 * Turns a reference to an array into the address of the array.
 */
struct tnode *arrayref(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	register short dp;

	if (ISARRAY(tp->t_type) && !(ISSTEL(tp)))
	{
		tp->t_dp++;
		dp = tp->t_dp;
		pushopd(tp);
		tadjust(tp, delspchk(tp->t_type), -1, 0);
		maketree(ADDR);
		tp = (struct tnode *) popopd();	/* tp cannot be 0 */
		tp->t_dp = dp;
	}
	return tp;
}


/* ttoconv - maps normal type into conversion table type */
int ttoconv(P(int) type)					/* returns conversion type */
PP(int type;)								/* type to convert */
{
	switch (type)
	{
	case CHAR:
	case INT:
		return 0;

	case UCHAR:
	case USHORT:
	case UNSIGNED:
		return 1;

	case ULONG:
	case LONG:
		return 2;

	case FLOAT:
	case DOUBLE:
		return 3;

	default:
		return 4;
	}
}
