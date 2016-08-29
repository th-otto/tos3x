/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)interf.c	1.5	12/14/83
*/

#include "cgen.h"

short bol;

short gflag;

short onepass;

int exprok PROTO((struct tnode *tp));



/* outexpr - output expression */
VOID outexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	outline();
	if (exprok(tp))
	{
#ifdef DEBUG
		if (cflag)
			printf("outexpr 1: type is %d\n", tp->t_type);
#endif
		scodegen(canon(tp), FOREFF, 0);
#ifdef DEBUG
		if (cflag)
			printf("outexpr 2: type is %d\n", tp->t_type);
#endif
	}
}


VOID outifgoto(P(struct tnode *) tp, P(int) dir, P(int) lab)
PP(struct tnode *tp;)
PP(int dir;)
PP(int lab;)
{
	outline();
	if (exprok(tp))
		condbr(canon(tp), dir, lab, 0);
}


VOID outcforreg(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	outline();
	if (exprok(tp))
		outmovr(scodegen(canon(tp), FORREG, 0), 0, tp);
}


VOID outinit(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	register short typeout;

	outline();
	if (exprok(tp))
	{
		typeout = tp->t_type;
		tp = canon(tp);
		if (tp->t_op == ADDR)
			tp = tp->t_left;
		if (tp->t_op == CINT || tp->t_op == CLONG || tp->t_op == SYMBOL)
		{
			if (tp->t_op != CINT)
				printf(".dc.l ");
			else
			{
				printf(".dc");
				outtype(typeout);
				putchar(' ');
			}
			outaexpr(tp, A_NOIMMED);	/* not immed... */
		} else
			error("invalid initialization");
		putchar('\n');
	}
}


/* snalloc - code generator symbol node allocation */
/*	This might be coalesced into parser snalloc. */
struct tnode *snalloc(P(int) type, P(int) sc, P(long) offset, P(int) dp, P(int) ssp)
PP(int type;)								/* type of symbol */
PP(int sc;)									/* storage class */
PP(long offset;)							/* offset from Local Environment Ptr */
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


int exprok(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	if ((char *)tp < exprarea || (char *)tp > &exprarea[EXPSIZE])
		return 0;
	if (LEAFOP(tp->t_op))
		return 1;
	if (BINOP(tp->t_op) && !exprok(tp->t_right))
		return 0;
	return exprok(tp->t_left);
}


VOID outline(NOTHING)
{
	if (onepass && !bol)
		putchar('\n');
	if (!gflag)
		printf("*line %d\n", lineno);
	else
		printf("~_lN%d:\n", lineno);
}
