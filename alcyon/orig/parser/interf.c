/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
short bol;


VOID outinit(P(struct tnode *) tp, P(int) type)
PP(struct tnode *tp;)
PP(int type;)
{
#ifdef __ALCYON__
	/* BUG: missing argument to tnalloc */
	outexpr(tnalloc(INIT, type, 0, 0, tp));
#else
	outexpr(tnalloc(INIT, type, 0, 0, tp, NULL));
#endif
}


VOID outcforreg(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
#ifdef __ALCYON__
	/* BUG: missing argument to tnalloc */
	outexpr(tnalloc(CFORREG, tp->t_type, 0, 0, tp));
#else
	outexpr(tnalloc(CFORREG, tp->t_type, 0, 0, tp, NULL));
#endif
}


VOID outifgoto(P(struct tnode *) tp, P(int) dir, P(int) lab)
PP(struct tnode *tp;)
PP(int dir;)
PP(int lab;)
{
#ifdef __ALCYON__
	/* BUG: missing argument to tnalloc */
	outexpr(tnalloc(IFGOTO, dir, lab, 0, tp));
#else
	outexpr(tnalloc(IFGOTO, dir, lab, 0, tp, NULL));
#endif
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
	putchar('\t');
	for (p = cstr; *p; p++)
	{
		putchar(*p);
		if (*p == '\n')
			putchar('\t');
	}
	putchar('\n');
}


VOID outexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	if (!tp)
		return;
	outline();
	outtree(tp);
}


/* interprets and prints the parse tree */
VOID outtree(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	short w1, w2;

	if (!tp)
		return;

	printf("%x.%x", tp->t_op, tp->t_type);

	switch (tp->t_op)
	{
	case CINT:
		printf(".%x\n", ((struct conode *) tp)->t_value);
		break;

	case CLONG:
		w1 = ((struct lconode *) tp)->_l.w.hiword;
		w2 = ((struct lconode *) tp)->_l.w.loword;
		printf(".%x.%x\n", w1, w2);
		break;

	case CFLOAT:
		w1 = ((struct lconode *) tp)->_l.w.hiword;
		w2 = ((struct lconode *) tp)->_l.w.loword;
		printf(".%x.%x\n", w1, w2);
		break;

	case SYMBOL:
		printf(".%x", ((struct symnode *) tp)->t_sc);
		if (((struct symnode *) tp)->t_sc == EXTERNAL)
			printf(".%.8s\n", (char *)((struct extnode *) tp)->t_symbol);
		else
			printf(".%x\n", ((struct symnode *) tp)->t_offset);
		break;

	case 0:
		putchar('\n');
		break;

	case IFGOTO:
	case BFIELD:
		printf(".%x\n", tp->t_dp);
		outtree(tp->t_left);
		break;

	default:
		putchar('\n');
		outtree(tp->t_left);
		if (BINOP(tp->t_op))
		{
			outtree(tp->t_right);
		}
		break;
	}
}
