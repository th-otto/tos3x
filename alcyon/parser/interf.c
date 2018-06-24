/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"



VOID outinit(P(struct tnode *) tp, P(int) type)
PP(struct tnode *tp;)
PP(int type;)
{
	outexpr(tnalloc(INIT, type, 0, 0, tp, NULL));
}


VOID outcforreg(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	outexpr(tnalloc(CFORREG, tp->t_type, 0, 0, tp, NULL));
}


VOID outifgoto(P(struct tnode *) tp, P(int) dir, P(int) lab)
PP(struct tnode *tp;)
PP(int dir;)
PP(int lab;)
{
	outexpr(tnalloc(IFGOTO, dir, lab, 0, tp, NULL));
}


/* outasm - outputs asm literal. */
VOID outasm(NOTHING)
{
	register const char *p;

	if (lineno != lst_ln_id)
	{
		outline();
		OUTNULL();
	}
	oputchar('\t');
	for (p = cstr; *p; p++)
	{
		if (*p == '\t')
			oputchar(' ');
		else
			oputchar(*p);
		if (*p == '\n')
			oputchar('\t');
	}
	oputchar('\n');
}


/* interprets and prints the parse tree */
static VOID outtree(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	unsigned short w1, w2;

	if (!tp)
		return;

	oprintf("%X.%X", (unsigned short)tp->t_op, (unsigned short)tp->t_type);

	switch (tp->t_op)
	{
	case CINT:
		oprintf(".%X\n", (unsigned short)((struct conode *) tp)->t_value);
		break;

	case CLONG:
		w1 = ((struct lconode *) tp)->_l.w.hiword;
		w2 = ((struct lconode *) tp)->_l.w.loword;
		oprintf(".%X.%X\n", w1, w2);
		break;

	case CFLOAT:
		w1 = ((struct lconode *) tp)->_l.w.hiword;
		w2 = ((struct lconode *) tp)->_l.w.loword;
		oprintf(".%X.%X\n", w1, w2);
		break;

	case SYMBOL:
		oprintf(".%X", (unsigned short)((struct symnode *) tp)->t_sc);
		if (((struct symnode *) tp)->t_sc == EXTERNAL)
			oprintf(".%.*s\n", SSIZE, ((struct extnode *) tp)->t_symbol);
		else
			oprintf(".%X\n", (unsigned short)((struct symnode *) tp)->t_offset);
		break;

	case 0:
		oputchar('\n');
		break;

	case IFGOTO:
	case BFIELD:
		oprintf(".%X\n", (unsigned short)tp->t_dp);
		outtree(tp->t_left);
		break;

	default:
		oputchar('\n');
		outtree(tp->t_left);
		if (BINOP(tp->t_op))
		{
			outtree(tp->t_right);
		}
		break;
	}
}


VOID outexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	if (!tp)
		return;
	outline();
	outtree(tp);
}
