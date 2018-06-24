/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "cgen.h"




/* talloc - allocate expression tree area */
struct tnode *talloc(P(int) size)
PP(int size;)
{
	register char *p;

	p = opap;
	if (p + size >= &exprarea[EXPSIZE])
		fatal(_("expression too complex"));
	opap = p + size;
	return (struct tnode *)p;
}


/*
 * snalloc - code generator symbol node allocation
 * This might be coalesced into parser snalloc.
 */
struct tnode *snalloc(P(int) type, P(int) sc, P(int32_t) offset, P(int) dp, P(int) ssp)
PP(int type;)								/* type of symbol */
PP(int sc;)									/* storage class */
PP(int32_t offset;)							/* offset from Local Environment Ptr */
PP(int dp;)									/* for compatability with parser */
PP(int ssp;)								/* for compatability with parser */
{
	register struct tnode *sp;

	sp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(struct xlsym));
	sp->t_op = SYMBOL;
	sp->t_type = type;
	sp->t_su = dp;
	sp->t_ssp = ssp;
	sp->t_sc = sc;
	switch (sc)
	{
	case STATIC:
		sp->t_offset = 0;
		sp->t_reg = 0;
		sp->t_label = offset;
		break;

	case REGISTER:
		sp->t_offset = 0;
		sp->t_reg = offset;
		sp->t_label = 0;
		break;

	case AUTO:
		sp->t_sc = REGOFF;
		sp->t_offset = offset;
		sp->t_reg = LEP;
		sp->t_label = 0;
		break;

	default:
		sp->t_offset = offset;
		sp->t_reg = 0;
		sp->t_label = 0;
		break;
	}
	return sp;
}


/*
 * tnalloc - allocate binary expression tree node
 * returns ptr to node made.
 */
struct tnode *tnalloc(P(int) op, P(int) type, P(int) info, P(int) dummy, P(struct tnode *) left, P(struct tnode *) right)
PP(int op;)									/* operator */
PP(int type;)								/* resultant node type */
PP(int info;)								/* info field */
PP(int dummy;)								/* dummy field - used to match pass1 args */
PP(struct tnode *left;)						/* left sub-tree */
PP(struct tnode *right;)					/* righst sub-tree */
{
	register struct tnode *tp;

	tp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(tp->v.t));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_su = info;						/* info for bit-field & condbr's */
	tp->t_left = left;
	tp->t_right = right;
	return tp;
}


/* cnalloc - allocate constant expression tree node */
struct tnode *cnalloc(P(int) type, P(int) value)
PP(int type;)								/* type of constant */
PP(int value;)								/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(cp->t_value));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_value = value;
	return cp;
}


/* lcnalloc - allocate constant expression tree node */
struct tnode *lcnalloc(P(int) type, P(int32_t) value)
PP(int type;)								/* type of constant */
PP(int32_t value;)							/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(cp->t_lvalue));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_lvalue = value;
	return cp;
}


/* fpcnalloc - allocate constant expression tree node */
struct tnode *fpcnalloc(P(int) type, P(int32_t) value)
PP(int type;)								/* type of constant */
PP(int32_t value;)							/* value of constant */
{
	register struct tnode *cp;

	cp = talloc(sizeof(*cp) - sizeof(union tval) + sizeof(cp->t_lvalue));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_lvalue = value;
	return cp;
}


/* xnalloc - allocate address-indexed node */
struct tnode *xnalloc(P(int) type, P(int) ar, P(int32_t) off, P(int) xr, P(int) xt)
PP(int type;)								/* data type */
PP(int ar;)									/* address register */
PP(int32_t off;)							/* 8-bit offset */
PP(int xr;)									/* index register */
PP(int xt;)									/* index register type */
{
	register struct tnode *xp;

	xp = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(struct xxsym));
	xp->t_op = SYMBOL;
	xp->t_type = type;
	xp->t_sc = INDEXED;
	xp->t_reg = ar;
	xp->t_su = SU_ADDR;
	xp->t_offset = off;
	xp->t_xreg = xr;
	xp->t_xtype = xt;
	return xp;
}


/* symcopy - copy symbol */
static VOID symcopy(P(const char *) from, P(char *) to)
PP(const char *from;)						/* from symbol */
PP(char *to;)								/* to symbol */
{
	register const char *p;
	register char *q;
	register short i;

	for (p = from, q = to, i = SSIZE; --i >= 0;)
	{
		*q++ = (*p ? *p++ : '\0');
	}
}


/* tcopy - expression tree copy */
struct tnode *tcopy(P(struct tnode *) tp, P(int) autof)
PP(struct tnode *tp;)
PP(int autof;)								/* {A_DOPRE,A_DOPOST} */
{
	register struct tnode *p;
	register short op;

	op = tp->t_op;
	switch (op)
	{
	case SYMBOL:
		if (tp->t_sc == EXTERNAL || tp->t_sc == EXTOFF)
			p = cenalloc(tp->t_type, tp->t_sc, tp->t_symbol);
		else
		{
			p = snalloc(tp->t_type, tp->t_sc, tp->t_offset, 0, 0);
			p->t_label = tp->t_label;
		}
		p->t_offset = tp->t_offset;
		p->t_reg = tp->t_reg;
		return p;

	case CINT:
		return cnalloc(tp->t_type, tp->t_value);

	case CLONG:
		return lcnalloc(tp->t_type, tp->t_lvalue);

	case CFLOAT:
		return fpcnalloc(tp->t_type, tp->t_lvalue);

	case DCLONG:
		p = lcnalloc(tp->t_type, tp->t_lvalue);
		p->t_op = DCLONG;
		return p;

	case AUTOINC:
	case AUTODEC:
		p = snalloc(tp->t_type, AUTO, 0L, 0, 0);
		if (op == AUTODEC)
		{
			if ((autof & A_DOPRE) != 0)
				p->t_op = AUTODEC;
		} else if (op == AUTOINC)
		{
			if ((autof & A_DOPOST) != 0)
				p->t_op = AUTOINC;
		}
		p->t_reg = tp->t_reg;
		return p;

	case POSTINC:
	case POSTDEC:
		if ((autof & A_DOPOST) == 0)
			return tcopy(tp->t_left, autof);
		goto copyop;
	case PREINC:
	case PREDEC:
		if ((autof & A_DOPRE) == 0)
			return tcopy(tp->t_left, autof);
	  copyop:
	default:
		if (ISASGOP(op))
		{
			if ((autof & A_DOPRE) == 0)
				return tcopy(tp->t_left, autof);
			autof |= A_DOPOST;			/* We'll come this way only once */
		}
		p = tnalloc(op, tp->t_type, 0, 0, &null, &null);
		if (BINOP(op))
			p->t_right = tcopy(tp->t_right, autof);
		p->t_left = tcopy(tp->t_left, autof);
		return p;
	}
}


/*
 * outlval - output long value
 * This is a big pain because the PDP-11 doesn't do long divides
 * in hardware.
 */
static VOID outlval(P(int32_t) lval)
PP(int32_t lval;)
{
	char digs[8];
	register short i, c;

	i = 0;
	do
	{
		digs[i++] = lval & 0xf;
		lval >>= 4;
		lval &= 0xfffffff;
	} while (lval);
	oputchar('$');
	while (--i >= 0)
	{
		c = digs[i];
		oputchar(c >= 10 ? c + ('a' - 10) : c + '0');
	}
}


/* outaexpr - output address expression */
VOID outaexpr(P(struct tnode *) tp, P(int) flags)
PP(struct tnode *tp;)
PP(int flags;)								/* flags (IMMED,LOFFSET,...) */
{
	register short reg, lab;
	register int32_t off;
	int32_t l;

	if (tp->t_op == ADDR)
	{
		tp = tp->t_left;
		oputchar('#');
	}
	off = tp->t_offset;
	reg = tp->t_reg;
	lab = tp->t_label;
	switch (tp->t_op)
	{
	case AUTOINC:
		if ((flags & A_DOPOST) != 0)
			oprintf("(R%d)+", reg);
		else
			oprintf("(R%d)", reg);
		break;

	case AUTODEC:
		if ((flags & A_DOPRE) != 0)
			oprintf("-(R%d)", reg);
		else
			oprintf("(R%d)", reg);
		break;

	case CINT:
		if ((flags & A_DOIMMED) != 0)
			oputchar('#');
		oprintf("%d", tp->t_value);
		break;

	case DCLONG:
	case CLONG:
	case CFLOAT:
		if ((flags & A_DOIMMED) != 0)
			oputchar('#');
		outlval(tp->t_lvalue);
		break;

	case SYMBOL:
		if (off)
		{
			switch (tp->t_sc)
			{
			default:
				oprintf("%ld+", (long)off);
				break;

			case REGOFF:
				oprintf("%ld", (long)off);
			case CINDR:
			case CLINDR:
			case CFINDR:
			case INDEXED:
				break;

			case REGISTER:
				error("invalid register expression");
				break;
			}
		}
		switch (tp->t_sc)
		{
		case REGISTER:
			oprintf("R%d", reg);
			break;

		case REGOFF:
			oprintf("(R%d)", reg);
			break;

		case EXTERNAL:
			oprintf("_%.8s", tp->t_symbol);
			break;

		case EXTOFF:
			oprintf("_%.8s(R%d)", tp->t_symbol, reg);
			break;

		case STATIC:
			oprintf("L%d", lab);
			break;

		case STATOFF:
			oprintf("L%d(R%d)", lab, reg);
			break;

		case INDEXED:
			oprintf("%ld(R", (long)off);
			oprintf("%d,R%d", reg, tp->t_xreg);
			outatype(tp->t_xtype);
			oputchar(')');
			break;

		case CINDR:
			oprintf("%ld", (long)off);
			break;
			/*
			 * the following will work on: PDP-11, 68000, IBM-360, VAX, etc.
			 * it will not work on word machines or on machines where either
			 * longs two ints or two shorts.
			 */
		case CLINDR:
		case CFINDR:
			l = tp->t_offset;
			outlval(l);
			break;

		default:
			error("invalid storage class %d\n", tp->t_sc);
			break;
		}
		break;

	default:
		error("invalid operator %s\n", opname[tp->t_op]);
		break;
	}
}


/* outtype - output 68000 type (null, .b, .l) depending on data type */
VOID outtype(P(int) type)
PP(int type;)
{
	if (ISFLOAT(type) || LONGORPTR(type))
		oprintf(".l");
	else if (type == CHAR)
		oprintf(".b");
}


/* outatype - output address type (.l or null) depending on data type */
VOID outatype(P(int) type)
PP(int type;)
{
	if (LONGORPTR(type) || ISFLOAT(type))
		oprintf(".l");
}


/* outextend - output register extension to long depending on type */
VOID outextend(P(struct tnode *) tp, P(int) type, P(int) reg)
PP(struct tnode *tp;)
PP(int type;)								/* type to convert to */
PP(int reg;)								/* register to convert */
{
	if ((ISDREG(reg)) && !(LONGORPTR(tp->t_type)) && (LONGORPTR(type)))
	{
		if (UNSIGN(tp->t_type))
			OUTUEXT(reg);
		else
			OUTEXT(reg);
	}
}


/* outrr - output register to register instruction */
VOID outrr(P(const char *) ins, P(int) r1, P(int) r2, P(struct tnode *) tp)
PP(const char *ins;)
PP(int r1;)
PP(int r2;)
PP(struct tnode *tp;)
{
	oprintf("%s", ins);
	if (ISAREG(r1) || ISAREG(r2))
		outatype(tp->t_type);
	else
		outtype(tp->t_type);
	oprintf(" R%d,R%d\n", r1, r2);
}


/* outmovr - output "move[type] R1,R2" instruction */
VOID outmovr(P(int) r1, P(int) r2, P(struct tnode *) tp)
PP(int r1;)
PP(int r2;)
PP(struct tnode *tp;)
{
	if (r1 != r2)
		outrr("move", r1, r2, tp);
}


/* fakecmpm - output 68010 fake cmpm for bad microcode */
static VOID fakecmpm(P(int) type, P(int) lreg, P(int) rreg)
PP(int type;)
PP(int lreg;)
PP(int rreg;)
{
	oprintf("move");
	outtype(type);
	oprintf(" (R%d),R0\n", rreg);
	oprintf("cmp");
	outtype(type);
	oprintf(" (R%d),R0\n", lreg);
	if (ISFLOAT(type) || LONGORPTR(type))
		oprintf("addq #4,R%d\naddq #4,R%d\n", lreg, rreg);
	else if (type == CHAR)
		oprintf("addq #1,R%d\naddq #1,R%d\n", lreg, rreg);
	else
		oprintf("addq #2,R%d\naddq #2,R%d\n", lreg, rreg);
}


/* outcmpm - output "cmpm[type] (R1)+,(R2)+" */
VOID outcmpm(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	if (!m68010)
	{									/* 68000 version cmpm ok !!! */
		oprintf("cmpm");
		outtype(tp->t_left->t_type);
		oprintf(" (R%d)+,(R%d)+\n", tp->t_left->t_reg, tp->t_right->t_reg);
	} else								/* generating code for 68010, micro code is bad for cmpm */
	{
		fakecmpm(tp->t_left->t_type, tp->t_left->t_reg, tp->t_right->t_reg);
	}
}


/* outcreg - output reference to compiler temp register */
VOID outcreg(P(int) reg)
PP(int reg;)
{
	if ((DREG(reg)) > HICREG)
		error("expression too complex");
	oprintf("R%d", reg);
}


/* outcmp0 - output a compare with 0, special for address register */
VOID outcmp0(P(int) reg, P(struct tnode *) tp)
PP(int reg;)
PP(struct tnode *tp;)
{
	if (ISAREG(reg))
	{
		oprintf("cmp");
		outatype(tp->t_type);
		oprintf(" #0,R%d\n", reg);
	} else
	{
		oprintf("tst");
		outtype(tp->t_type);
		oprintf(" R%d\n", reg);
	}
}


/* outrpush - output "move[type] R1,[-](sp)" */
VOID outrpush(P(int) reg, P(struct tnode *) tp, P(int) pflag)
PP(int reg;)
PP(struct tnode *tp;)
PP(int pflag;)
{
	oprintf("move");
	outatype(tp->t_type);
	oprintf(" R%d,%s\n", reg, pflag ? "-(sp)" : "(sp)");
}


int outdbra(P(int) dir, P(int) op, P(struct tnode *) ltp, P(struct tnode *) rtp, P(int) lab)
PP(int dir;)
PP(int op;)
PP(struct tnode *ltp;)
PP(struct tnode *rtp;)
PP(int lab;)
{
	if (!((dir != 0 && op == NEQUALS) || (dir == 0 && op == EQUALS)))
		return 0;
	if (!(ltp->t_op == PREDEC && rtp->t_op == CINT && rtp->t_value == -1 &&
		  ISDREG(ltp->t_left->t_reg) && ltp->t_left->t_type == INT && ltp->t_left->t_sc == REGISTER))
		return 0;

	oprintf("dbra R%d,L%d\n", ltp->t_left->t_reg, lab);
	return 1;
}


/*
 * cenalloc - code generator external node allocation
 * This may be coalesced into enalloc in parser.
 */
struct tnode *cenalloc(P(int) type, P(int) sc, P(const char *) sym)
PP(int type;)								/* type of symbol */
PP(int sc;)									/* storage class */
PP(const char *sym;)						/* symbol name */
{
	register struct tnode *ep;

	ep = talloc(sizeof(struct tnode) - sizeof(union tval) + sizeof(struct xesym));
	ep->t_op = SYMBOL;
	ep->t_type = type;
	ep->t_sc = sc;
	ep->t_su = 0;
	ep->t_offset = 0;
	symcopy(sym, ep->t_symbol);
	return ep;
}


/* popstack - clear off the stack after a call if necessary */
VOID popstack(P(int) nb)
{
	if (nb > 0 && nb <= 8)
		oprintf("addq.l #%d,sp\n", nb);
	else if (nb > 0)
		oprintf("adda.l #%d,sp\n", nb);
}
