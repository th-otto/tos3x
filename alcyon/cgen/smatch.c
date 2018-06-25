/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* Code Skeleton expansion and matching */

#include "cgen.h"

#define	SK_TYPE(x)	(x&017)


/* op_expand - printf out the subroutine being called */
static VOID op_expand(P(int) op, P(int) type, P(int) ltype)
PP(int op;)
PP(int type;)
PP(int ltype;)
{
	if (ISFLOAT(type) || ISFLOAT(ltype))
	{
		switch (op)
		{
		case ADD:
		case EQADD:
		case PREINC:
		case POSTINC:
			oprintf("_fpadd");
			break;

		case SUB:
		case EQSUB:
		case PREDEC:
		case POSTDEC:
			oprintf("_fpsub");
			break;

		case MULT:
		case EQMULT:
			oprintf("_fpmult");
			break;

		case DIV:
		case EQDIV:
			oprintf("_fpdiv");
			break;

		case UMINUS:
		case EQNEG:
			oprintf("_fpneg");
			break;

		case FLOAT2L:
		case FLOAT2I:
			oprintf("_fpftol");
			break;

		case LONG2F:
		case INT2F:
			oprintf("_fpltof");
			break;

		case EQUALS:
		case NEQUALS:
		case GREAT:
		case GREATEQ:
		case LESS:
		case LESSEQ:
			oprintf("_fpcmp");
			break;

		default:
			error("invalid floating point op %d\n", op);
			break;
		}
	} else
	{
		switch (op)
		{
		case MULT:
		case LMULT:
			oprintf("lmul");
			break;

		case DIV:
		case LDIV:
			oprintf("ldiv");
			break;

		case MOD:
		case LMOD:
			oprintf("lrem");
			break;

		default:
			error("opcall bad op %d", op);
			break;
		}
	}
}


/*
 * expand - code skeleton expansion
 * Handles the expansion of code skeleton macros.
 * returns register result is in.
 */
int expand(P(struct tnode *) tp, P(int) cookie, P(int) freg, P(const struct skeleton *) skp)
PP(struct tnode *tp;)						/* pointer to expression tree */
PP(int cookie;)								/* goal of expression tree */
PP(int freg;)								/* register to leave results in */
PP(const struct skeleton *skp;)				/* pointer to code skeleton */
{
	register short op, nreg, reg;
	register short c;
	register short extf, i2f;
	register struct tnode *ltp, *rtp;
	register struct tnode *p;
	register short i, sreg, flag, subtrees, scookie;
	register const char *macro;

	/*
	 *	This is a kludge because not all of the arithmetic operators
	 *	will work on address registers - the 68000 clone strikes again
	 */
	op = tp->t_op;
#ifdef DEBUG
	if (eflag)
		oprintf("expand op=%d left=0x%x right=0x%x skp=%p\n", op, skp->sk_left, skp->sk_right, skp);
#endif
	if (((op >= MULT && op <= COMPL) || (op >= LMULT && op <= LMOD)) || tp->t_type == CHAR)
		freg = DREG(freg);
	macro = skp->sk_def;
	i2f = extf = 0;
	rtp = ltp = tp->t_left;
	subtrees = 1;
	if (BINOP(op))
	{
		subtrees++;
		rtp = tp->t_right;
		if ((LONGORPTR(tp->t_type)) && (op == DIV || op == MOD ||
			(op != MULT && (ISDREG(freg)) &&
			 !(LONGORPTR(ltp->t_type)) && !(LONGORPTR(rtp->t_type)))))
			extf++;
		switch (op)
		{
		case RSH:
		case LSH:
		case EQLSH:
		case EQRSH:
			if (UNSIGN(ltp->t_type))
				i2f++;
			break;

		case MULT:
		case EQMULT:
		case DIV:
		case MOD:
		case EQDIV:
		case EQMOD:
			if (UNSIGN(ltp->t_type) || UNSIGN(rtp->t_type))
				i2f++;
			break;
		}
	}
	nreg = freg + 1;
	while ((c = *macro++) != 0)
	{
		c &= 0xff;
		switch (c)
		{
		default:
			oputchar(c);
			break;

		case POP:
			stacksize--;
			oprintf("(sp)+");
			break;

		case POP4:
			stacksize--;
			popstack(4);
			break;

		case POP8:
			stacksize -= 2;
			popstack(8);
			break;

		case PSH:
			if (cookie == FORSP)		/* don't affect sp */
				oprintf("(sp)");
			else
				oprintf("-(sp)");
			stacksize++;
			break;

		case MOV:
		case MOVL:
		case JSR:
		case CLR:
		case CLRL:
		case EXTW:
		case EXTL:
		case LEA:
		case STK:
			oprintf("%s", strtab[c - 128]);
			break;

		case OPCALL:
			op_expand(op, tp->t_type, ltp->t_type);
			break;

		case TLEFT:
			outtype(LEAFOP(op) ? tp->t_type : ltp->t_type);
			break;

		case TLEFTL:
			outatype(LEAFOP(op) ? tp->t_type : ltp->t_type);
			break;

		case TEITHER:
			if (LONGORPTR(rtp->t_type) || LONGORPTR(ltp->t_type))
				outtype(LONG);
			break;

		case TRIGHT:
			outtype(rtp->t_type);
			break;

		case OP:
		case AOP:
			if (c == AOP || i2f)
				i = optab[op][1];
			else
				i = optab[op][0];
			oprintf(mnemonics[i]);
			break;

		case LADDR:
		case RADDR:
			p = ((c == RADDR) ? rtp : ltp);
			if (*macro == '+')
			{
				macro++;
				outaexpr(p, A_DOPOST | A_DOIMMED);
			} else if (*macro == '-')
			{
				macro++;
				outaexpr(p, A_DOPRE | A_DOIMMED);
			} else
			{
				outaexpr(p, A_DOIMMED | A_DOPRE | A_DOPOST);
			}
			break;

		case CR:
			outcreg(freg);
			break;

		case NR:
			outcreg(nreg);
			break;

		case CAR:
			outcreg(AREG(freg));
			break;

		case NAR:
			outcreg(AREG(nreg));
			break;

		case EXL:
			outextend(ltp, LONG, freg);
			break;

		case EXRL:
		case EXRLN:
			outextend(rtp, ltp->t_type, c == EXRL ? freg : nreg);
			break;

		case EXLR:
		case EXLRN:
			outextend(ltp, rtp->t_type, c == EXLR ? freg : nreg);
			break;

		case LEFT:
		case RIGHT:
			subtrees--;
			/* fall through */
		case TREE:
			p = (c == LEFT ? ltp : c == RIGHT ? rtp : tp);
			flag = *macro++;
			scookie = FORREG;
			if (flag & S_STACK)
			{
				if (cookie == FORSP)
					scookie = FORSP;
				else
					scookie = FORSTACK;
			} else if (flag & S_FORCC)
			{
				scookie = FORCC;
			}
			if (flag & S_NEXT)
				reg = nreg;
			else
				reg = freg;
			if (flag & S_INDR)
			{
				if (p->t_op != INDR)
					error("code skeleton error: %d\n", op);
				p = p->t_left;			/* skip INDR */
				if (coffset(p))
				{
					p = p->t_left;
					if (LONGORPTR(p->t_type) == 0 && (flag & S_STACK) != 0)
					{
						p = tnalloc(INT2L, LONG, 0, 0, p, NULL);
					}
				}
				reg = AREG(reg);		/* no qualifications before... */
#ifdef DEBUG
				if (eflag)
					oprintf("reg = %d, nreg = %d, freg = %d\n", reg, nreg, freg);
#endif
			}
			sreg = codegen(p, scookie, reg);	/* code for subtree */
			if (scookie == FORREG)
			{
				if (flag & S_INDR)
				{
					if (ISDREG(sreg))
						outmovr(sreg, AREG(reg), p);
				} else if (flag & S_NEXT)
					nreg = sreg;
				else if (sreg != reg)
				{
					/*
					 * result was not in expected register, if remaining sub-tree can be
					 * compiled using the remaining registers, update current and next
					 * registers, saving us the trouble of moving the register.
					 */
					if (c == TREE || ((ISDREG(sreg)) && subtrees > 0 &&
									  ((c == LEFT &&
										sucomp(rtp, sreg, 0) <= skp->sk_right &&
										sucomp(rtp, sreg, 1) <= SU_ANY) ||
									   (c == RIGHT &&
										sucomp(ltp, sreg, 0) <= skp->sk_left && sucomp(ltp, sreg, 1) <= SU_ANY))))
					{
						freg = DREG(sreg);
						nreg = freg + 1;
					} else
					{
						outmovr(sreg, DREG(freg), p);
					}
				}
			}
			break;

		case LOFFSET:
		case ROFFSET:
			p = (c == LOFFSET) ? ltp->t_left : rtp->t_left;
			if ((p = coffset(p)) != 0 && (p->t_op != CINT || p->t_value != 0))
				outaexpr(p, A_NOIMMED);
			break;

		case MODSWAP:
			switch (op)
			{
			case LMOD:
			case LEQMOD:
				extf++;
				/* fall through */
			case MOD:
			case EQMOD:
				OUTSWAP(freg);
				break;
			}
			break;
		}
	}
	if (extf && cookie == FORREG && (ISDREG(freg)))
	{
		if (UNSIGN(ltp->t_type) || UNSIGN(rtp->t_type))
			OUTUEXT(freg);
		else
			OUTEXT(freg);
	}
#ifdef DEBUG
	if (eflag)
		oprintf("ending expand skp=%p\n", skp);
#endif
	return freg;
}


/*
 * skelmatch - sub-tree type matching for match
 * This checks a subtree for type compatability in match.
 */
static int skelmatch(P(struct tnode *) tp, P(int) skinfo)
PP(struct tnode *tp;)
PP(int skinfo;)
{
	register short type, unsignf;
	register short isconstant, stype;

	if (tp->t_su > skinfo || ((skinfo & T_INDR) && tp->t_op != INDR))
		return 0;
	stype = SK_TYPE(skinfo);
	type = tp->t_type;
	if (ISFUNCTION(type))
		type = BTYPE(type);
	if ((unsignf = UNSIGN(type)) != 0)
		type = BASETYPE(type);
	isconstant = 0;

	switch (tp->t_op)
	{
	case CFLOAT:
	case CLONG:
		if (tp->t_su > SU_CONST)
			break;
		/* fall through */
	case CINT:
		isconstant++;
		break;
	}
	switch (stype)
	{
	case T_CHAR:
		return type == CHAR;

	case T_ANY:						/* either short or char */
		if (type == CHAR)
			return 1;
		/* fall through */
	case T_INT:
		return type == INT || isconstant;

	case T_UNSN:
	case T_UANY:
	case T_UCHAR:
	case T_ULONG:
		return unsignf;

	case T_LONG:
		return LONGORPTR(type);

	case T_FLOAT:
		return ISFLOAT(type);

	default:
		error("skelmatch type: %x", stype);
		return 0;
	}
}


/*
 * match - try to match expression tree with code skeleton
 *		Given the expression tree, tries to match the given tree with
 *		the appropriate code skeleton.  The code skeleton list is
 *		gotten from the root operator and the cookie value.  The code
 *		skeleton list is then searched, checking the Sethy-Ullman numbers
 *		of the sub-trees against the Sethy-Ullman numbers in the code
 *		skeleton list.  If the Sethy-Ullman numbers are OK, then the
 *		left and right sub-trees are checked for compatability, e.g.
 *		integer pointers, etc.  If a match is found, the code skeleton
 *		list pointer is returned.
 * returns ptr to code skeleton, or 0 if no skeleton
 */
const struct skeleton *match(P(struct tnode *) tp, P(int) cookie, P(int) reg)
PP(struct tnode *tp;)						/* pointer to tree */
PP(int cookie;)								/* goal for code expansion */
PP(int reg;)								/* register to use */
{
	register const struct skeleton *skp;
	register short op, bop;
	short i;
	register struct tnode *ltp, *rtp;

#ifdef DEBUG
	if (mflag)
		oprintf("match op=%d cookie=%d reg=%d\n", tp->t_op, cookie, reg);
#endif
	rtp = 0;
	PUTEXPR(mflag, "match", tp);
	if ((op = tp->t_op) >= LCGENOP)
		return 0;
	if (LEAFOP(op))
		ltp = tp;
	else
		ltp = tp->t_left;
	if ((bop = BINOP(op)) != 0)
	{
		rtp = tp->t_right;
		if (CONVOP(ltp->t_op))
		{
			if (op != LSH && NOTCONVOP(rtp->t_op) && cookie != FORSTACK && cookie != FORSP && cookie != FORCREG)
			{
				if (!(UNSIGN(ltp->t_left->t_type)) || op == ASSIGN)
				{
					tp->t_left = ltp->t_left;
					if ((skp = match(tp, cookie, reg)) != NULL)
						return skp;
					tp->t_left = ltp;
				}
			}
		} else if (CONVOP(rtp->t_op))
		{
			if (!(UNSIGN(rtp->t_left->t_type)) || op == ASSIGN)
			{
				tp->t_right = rtp->t_left;
				if ((skp = match(tp, cookie, reg)) != 0)
					return skp;
				tp->t_right = rtp;
			}
		}
	}

	switch (cookie)
	{
	case FORCC:
		i = 3;
		break;

	case FOREFF:
		i = 2;
		break;

	case FORSTACK:
	case FORSP:
		i = 4;
		break;

	case FORREG:
		i = 5;
		break;

	default:
		error("match cookie=%d\n", cookie);
		return 0;
	}
#ifdef DEBUG
	if (mflag)
		oprintf("match op=%d i=%d ", op, i);
#endif
	if (!(i = optab[op][i]))
		return 0;
	skp = codeskels[i];
#ifdef DEBUG
	if (mflag)
		oprintf("codeskels[%d]=%p\n", i, skp);
#endif
#ifdef DEBUG
	if (mflag)
	{
		oprintf("match LEFT ");
		puttsu(ltp);
		if (bop)
		{
			oprintf(" RIGHT ");
			puttsu(rtp);
		}
		oputchar('\n');
	}
#endif
	for (; skp->sk_left != 0; skp++)
	{
#ifdef DEBUG
		if (mflag > 1)
			oprintf("sk_left=0x%x sk_right=0x%x\n", skp->sk_left, skp->sk_right);
#endif
		if (!(skelmatch(ltp, skp->sk_left)))
			continue;
		if (bop && !(skelmatch(rtp, skp->sk_right)))
			continue;
#ifdef DEBUG
		if (mflag)
			oprintf("match found skp=%p left=0x%x right=0x%x\n", skp, skp->sk_left, skp->sk_right);
#endif
		return skp;
	}
	return NULL;
}
