/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)putexpr.c	1.4	12/15/83
*/

#include "cgen.h"

char invalid[] = "INVALID";

char *opname[] = {
	invalid,							/*  0 */
	"+",								/*  1 */
	"-",								/*  2 */
	"*",								/*  3 */
	"/",								/*  4 */
	"%",								/*  5 */
	">>",								/*  6 */
	"<<",								/*  7 */
	"&",								/*  8 */
	"|",								/*  9 */
	"^",								/* 10 */
	"!",								/* 11 */
	"U-",								/* 12 */
	"~",								/* 13 */
	"--p",								/* 14 */
	"++p",								/* 15 */
	"p--",								/* 16 */
	"p++",								/* 17 */
	"=",								/* 18 */
	"+=",								/* 19 */
	"-=",								/* 20 */
	"*=",								/* 21 */
	"/=",								/* 22 */
	"%=",								/* 23 */
	">>=",								/* 24 */
	"<<=",								/* 25 */
	"&=",								/* 26 */
	"|=",								/* 27 */
	"^=",								/* 28 */
	"jsr",								/* 29 */
	"==",								/* 30 */
	"!=",								/* 31 */
	">",								/* 32 */
	">=",								/* 33 */
	"<",								/* 34 */
	"<=",								/* 35 */
	"int->long",						/* 36 */
	"long->int",						/* 37 */
	"btst",								/* 38 */
	"load",								/* 39 */
	"long*",							/* 40 */
	"long/",							/* 41 */
	"long%",							/* 42 */
	"long*=",							/* 43 */
	"long/=",							/* 44 */
	"long%=",							/* 45 */
	"=addr",							/* 46 */
	"=not",								/* 47 */
	"=neg",								/* 48 */
	"docast",							/* 49 */
	"st=",								/* 50 */
	"long->float",						/* 51 */
	"float->long",						/* 52 */
	"int->float",						/* 53 */
	"float->int",						/* 54 */
	"tochar",							/* 55 */
	invalid,							/* 56 */
	invalid,							/* 57 */
	invalid,							/* 58 */
	invalid,							/* 59 */
	"U&",								/* 60 ADDR */
	"U*",								/* 61 INDR */
	"&&",								/* 62 */
	"||",								/* 63 */
	"?",								/* 64 */
	":",								/* 65 */
	",",								/* 66 */
	"cint",								/* 67 */
	"clong",							/* 68 */
	"symbol",							/* 69 */
	"++a",								/* 70 */
	"a--",								/* 71 */
	"call",								/* 72 */
	"call()",							/* 73 */
	"bitfield",							/* 74 */
	"if",								/* 75 */
	"init",								/* 76 */
	"loadR0",							/* 77 */
	"divlong",							/* 78 */
};

#ifdef	DEBUG

VOID outlevel PROTO((NOTHING));
VOID putsexpr PROTO((struct tnode *tp));

char *types[] = {
	"typeless-invalid",					/* 0=TYPELESS */
	"char",								/* 1=CHAR */
	"short",							/* 2=SHORT */
	"int",								/* 3=INT */
	"long",								/* 4=LONG */
	"uchar-invalid",					/* 5=UCHAR */
	"ushort-invalid",					/* 6=USHORT */
	"uint",								/* 7=UINT */
	"ulong-invalid",					/* 8=ULONG */
	"float",							/* 9=FLOAT */
	"double",							/* 10=DOUBLE */
	"struct",							/* 11=STRUCT */
	invalid,							/* 12=undefined */
	invalid,							/* 13=undefined */
	invalid,							/* 14=undefined */
	invalid,							/* 15=undefined */
};

char *suvals[] = {
	"zero",
	"one",
	"quick",
	"small",
	"constant",
	"Areg",
	"Dreg",
	"addressable",
	"loadable",
	"easy",
	"hard",
	"veryhard",
};

short level;

VOID putexpr(P(const char *) name, P(struct tnode *) tp)
PP(const char *name;)
PP(struct tnode *tp;)
{
	printf("%s\n", name);
	putsexpr(tp);
}


VOID putsexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	level++;
	outlevel();
	printf("%s ", opname[tp->t_op]);
	if (tp->t_op == BFIELD || tp->t_op == IFGOTO)
	{
		if (tp->t_op == BFIELD)
			printf("off=%d len=%d\n", (tp->t_su >> 8) & 0377, tp->t_su & 0377);
		else
			printf("%s goto L%d\n", tp->t_type ? "TRUE" : "FALSE", tp->t_su);
		putsexpr(tp->t_left);
		level--;
		return;
	}
	puttsu(tp);
	switch (tp->t_op)
	{
	case DCLONG:
	case CLONG:
	case CFLOAT:
		printf(" %x.%x\n", tp->v.w.hiword, tp->v.w.loword);
		break;

	case CINT:
		printf(" %d\n", tp->t_value);
		break;

	case AUTODEC:
	case AUTOINC:
		printf(" R%d\n", tp->t_reg);
		break;

	case SYMBOL:
		switch (tp->t_sc)
		{
		case REGISTER:
			printf(" R%d", tp->t_reg);
			break;

		case CINDR:
			printf(" %ld\n", tp->t_offset);
			break;

		case CLINDR:
		case CFINDR:
			printf(" %lx.", tp->t_offset);
			printf("%x\n", tp->t_ssp);
			break;

		case REGOFF:
			printf(" %ld", tp->t_offset);
			printf("(R%d)", tp->t_reg);
			break;

		case EXTERNAL:
		case EXTOFF:
			printf(" %s+%ld", tp->t_symbol, tp->t_offset);
			if (tp->t_sc == EXTOFF)
				printf("(R%d)", tp->t_reg);
			break;

		case STATIC:
		case STATOFF:
			printf(" L%d+%ld", tp->t_label, tp->t_offset);
			if (tp->t_sc == STATOFF)
				printf("(R%d)", tp->t_reg);
			break;

		case INDEXED:
			printf(" %ld", tp->t_offset);
			printf("(R%d,R%d)", tp->t_reg, tp->t_xreg);
			break;
		}
		putchar('\n');
		break;

	case IFGOTO:
		putsexpr(tp->t_left);
		break;

	default:
		putchar('\n');
		putsexpr(tp->t_left);
		if (BINOP(tp->t_op))
			putsexpr(tp->t_right);
		break;
	}
	level--;
}


VOID outlevel(NOTHING)
{
	register short i;

	for (i = 0; i < level; i++)
	{
		putchar(' ');
		putchar(' ');
		putchar(' ');
		putchar(' ');
	}
}


VOID puttsu(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	register short i;

	if (SUPTYPE(tp->t_type))
		putchar('*');
	printf("%s ", types[BTYPE(tp->t_type)]);
	if (tp->t_su != 0 || (tp->t_op == CINT && tp->t_value == 0))
	{
		i = tp->t_su >> 8;
		if (i > 15 || i < 0)
			printf("INVALID");
		else
			printf("%s", suvals[tp->t_su >> 8]);
	}
}
#endif
