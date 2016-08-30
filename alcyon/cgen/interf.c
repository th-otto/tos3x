/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)interf.c	1.5	12/14/83
*/

#include "cgen.h"




static int exprok(P(struct tnode *) tp)
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


/* outexpr - output expression */
VOID outexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	outline();
	if (exprok(tp))
	{
#ifdef DEBUG
		if (cflag)
			oprintf("outexpr 1: type is %d\n", tp->t_type);
#endif
		scodegen(canon(tp), FOREFF, 0);
#ifdef DEBUG
		if (cflag)
			oprintf("outexpr 2: type is %d\n", tp->t_type);
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
				oprintf(".dc.l ");
			else
			{
				oprintf(".dc");
				outtype(typeout);
				oputchar(' ');
			}
			outaexpr(tp, A_NOIMMED);	/* not immed... */
		} else
		{
			error("invalid initialization");
		}
		oputchar('\n');
	}
}


VOID outline(NOTHING)
{
	if (onepass && !bol)
		oputchar('\n');
	if (!gflag)
		oprintf("*line %d\n", lineno);
	else
		oprintf("~_lN%d:\n", lineno);
}
