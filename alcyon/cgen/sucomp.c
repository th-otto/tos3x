/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)sucomp.c	1.3	11/2/83
*/

#include "cgen.h"


/*
 * sucomp - Sethy-Ullman expression complexity measure computation
 * This is a heuristic computation of the Sethy-Ullman numbers
 * for expressions.  This gives an approximation of the complexity
 * of the expression.  The code generation scheme works best if
 * the most complex expressions are done first.
 */
short sucomp(P(struct tnode *) tp, P(int) nregs, P(int) flag)
PP(struct tnode *tp;)
PP(int nregs;)								/* number of registers left */
PP(int flag;)								/* 1=>set values in tree, 0=>return */
{
	register short su, sur, op, i;
	register struct tnode *ltp,	*rtp;

	nregs = DREG(nregs);
	ltp = NULL;
	rtp = NULL;
	if (BINOP(op = tp->t_op))
	{
		ltp = tp->t_left;
		rtp = tp->t_right;
	} else if (UNARYOP(op))
	{
		ltp = tp->t_left;
	}
	switch (op)
	{
	case CLONG:
		if (tp->t_lvalue >= 0x8000L || tp->t_lvalue <= 0xffff8000L)
		{
			su = SU_ADDR;
			break;
		}
		i = tp->t_lvalue;
	case CINT:
		if (op == CINT)
			i = tp->t_value;
		if (i == 0)
			su = SU_ZERO;
		else if (i == 1)
			su = SU_ONE;
		else if (i >= 1 && i <= QUICKVAL)
			su = SU_SMALL;
		else if (i >= -128 && i <= 127)
			su = SU_QUICK;
		else
			su = SU_CONST;
		break;

	case COMMA:
		su = sucomp(rtp, nregs, flag);
		i = sucomp(ltp, nregs, flag);
		su = MAX(su, i);
		su = MAX(su, SU_EASY);
		break;

	case ADDR:
		su = sucomp(ltp, nregs, flag);
		break;

	case CFLOAT:
	case DCLONG:
	case AUTOINC:
	case AUTODEC:
		su = SU_ADDR;
		break;

	case SYMBOL:
		if (tp->t_sc != REGISTER)
			su = SU_ADDR;
		else if (ISDREG(tp->t_reg))
			su = SU_REG;
		else
			su = SU_AREG;
		break;

	case LDIV:
	case LMOD:
	case LMULT:
	case CALL:
		sucomp(rtp, nregs, flag);
	case NACALL:
		sucomp(ltp, nregs, flag);
		su = SU_VHARD;					/* very hard */
		break;

	default:
		su = sucomp(ltp, nregs, flag);
		if (BINOP(op))
		{
			if (su <= SU_ADDR)
			{
				i = sucomp(rtp, nregs, flag);
				su = MAX(su, i);
			} else
			{
				sur = sucomp(rtp, nregs + 1, flag);
				if (sur > SU_ADDR && nregs > HICREG)
					su = MAX(su, SU_HARD);
			}
			su = MAX(SU_EASY, su);
		} else if (su <= SU_XREG)
			su = MAX(SU_EASY, su);
		if (ISFLOAT(tp->t_type))
			su = SU_VHARD;
		break;
	}
	if (flag)
		tp->t_su = su;
	return su;
}
