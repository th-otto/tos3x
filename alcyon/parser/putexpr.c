/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "parser.h"
#ifdef DEBUG
#include <ctype.h>

static char const invalid[] = "INVALID";

static const char *const opname[] = {
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
	"ltof",								/* 51 */
	"ftol",								/* 52 */
	"itof",								/* 53 */
	"ftoi",								/* 54 */
	"tochar",							/* 55 */
	invalid,							/* 56 */
	invalid,							/* 57 */
	invalid,							/* 58 */
	invalid,							/* 59 */
	"U&",								/* 60 */
	"U*",								/* 61 */
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

static const char *const types[] = {
	"typeless",							/*  0=TYPELESS */
	"char",								/*  1=CHAR */
	"short",							/*  2=SHORT */
	"int",								/*  3=INT */
	"long",								/*  4=LONG */
	"uchar",							/*  5=UCHAR */
	"ushort",							/*  6=USHORT */
	"uint",								/*  7=UINT */
	"ulong",							/*  8=ULONG */
	"float",							/*  9=FLOAT */
	"double",							/* 10=DOUBLE */
	"struct",							/* 11=STRUCT */
	invalid,							/* 12=undefined */
	invalid,							/* 13=undefined */
	invalid,							/* 14=undefined */
	invalid								/* 15=undefined */
};

static const char *const suvals[] = {
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

static short level;


static VOID outlevel(NOTHING)
{
	register short i;

	for (i = 0; i < level; i++)
		fprintf(stderr, "     ");
}


static VOID puttsu(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	switch (SUPTYPE(tp->t_type))
	{
	case FUNCTION:
		fprintf(stderr, "()");
		break;

	case ARRAY:
		fprintf(stderr, "[]");
		break;

	case POINTER:
		fprintf(stderr, "*");
		break;
	}
	fprintf(stderr, "%s ", types[BTYPE(tp->t_type)]);
}


static VOID putsexpr(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	register struct tnode *ltp;
	register short op;

	level++;
	ltp = tp->t_left;
	op = tp->t_op;
	outlevel();
	if (op < 0 || op > CFLOAT)
	{
		error(_("INVALID op"));
		return;
	}
	fprintf(stderr, "%s ", opname[op]);
	puttsu(tp);
	switch (op)
	{
	case DCLONG:
	case CLONG:
	case CFLOAT:
		fprintf(stderr, " %x.%x\n", ((struct lconode *) tp)->_l.w.hiword, ((struct lconode *) tp)->_l.w.loword);
		break;

	case CINT:
		fprintf(stderr, " %d [0x%x]\n", ((struct conode *) tp)->t_value, ((struct conode *) tp)->t_value);
		break;

	case AUTODEC:
	case AUTOINC:
		fprintf(stderr, "Autodec or Autoinc");
		break;

	case SYMBOL:
		switch (((struct symnode *) tp)->t_sc)
		{
		case REGISTER:
			fprintf(stderr, " Register");
			break;

		case CINDR:
			fprintf(stderr, " %d\n", ((struct symnode *) tp)->t_offset);
			break;

		case CLINDR:
		case CFINDR:
			fprintf(stderr, " %x.%x\n", ((struct symnode *) tp)->t_offset, tp->t_ssp);
			break;

		case REGOFF:
			fprintf(stderr, " Regoffset");
			break;

		case EXTERNAL:
		case EXTOFF:
			fprintf(stderr, " %s+%d", ((struct extnode *) tp)->t_symbol, ((struct extnode *) tp)->t_offset);
			if (((struct symnode *) tp)->t_sc == EXTOFF)
				fprintf(stderr, "Ext offset");
			break;

		case STATIC:
		case STATOFF:
			fprintf(stderr, " STATIC or STATOFF");
			if (((struct symnode *) tp)->t_sc == STATOFF)
				fprintf(stderr, "STATOFF");
			break;

		case INDEXED:
			fprintf(stderr, " %d indexed", ((struct symnode *) tp)->t_offset);
			break;
		}
		oputchar('\n');
		break;

	case IFGOTO:
		putsexpr(tp->t_left);
		break;

	default:
		oputchar('\n');
		putsexpr(tp->t_left);
		if (BINOP(tp->t_op))
			putsexpr(tp->t_right);
		break;
	}
	level--;
}


VOID putexpr(P(const char *) name, P(struct tnode *) tp)
PP(const char *name;)
PP(struct tnode *tp;)
{
	oprintf("%s\n", name);
	if (tp != 0)
		putsexpr(tp);
}
#endif
