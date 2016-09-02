/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
*/

#include "parser.h"
#ifdef DEBUG
#include <ctype.h>

static char const invalid[] = "INVALID";

static const char *const opname[] = {
	invalid,							/*  0 */
	"+",								/*  1=ADD */
	"-",								/*  2=SUB */
	"*",								/*  3=MULT */
	"/",								/*  4=DIV */
	"%",								/*  5=MOD */
	">>",								/*  6=RSH */
	"<<",								/*  7=LSH */
	"&",								/*  8=AND */
	"|",								/*  9=OR */
	"^",								/* 10=XOR */
	"!",								/* 11=NOT */
	"U-",								/* 12=UMINUS */
	"~",								/* 13=COMPL */
	"--p",								/* 14=PREDEC */
	"++p",								/* 15=PREINC */
	"p--",								/* 16=POSTDEC */
	"p++",								/* 17=POSTINC */
	"=",								/* 18=ASSIGN */
	"+=",								/* 19=EQADD */
	"-=",								/* 20=EQSUB */
	"*=",								/* 21=EQMULT */
	"/=",								/* 22=EQDIV */
	"%=",								/* 23=EQMOD */
	">>=",								/* 24=EQRSH */
	"<<=",								/* 25=EQLSH */
	"&=",								/* 26=EQAND */
	"|=",								/* 27=EQOR */
	"^=",								/* 28=EQXOR */
	"jsr",								/* 29=FJSR */
	"==",								/* 30=EQUALS */
	"!=",								/* 31=NEQUALS */
	">",								/* 32=GREAT */
	">=",								/* 33=GREATEQ */
	"<",								/* 34=LESS */
	"<=",								/* 35=LESSEQ */
	"int->long",						/* 36=INT2L */
	"long->int",						/* 37=LONG2I */
	"btst",								/* 38=BTST */
	"load",								/* 39=LOAD */
	"long*",							/* 40=LMULT */
	"long/",							/* 41=LDIV */
	"long%",							/* 42=LMOD */
	"long*=",							/* 43=LEQMULT */
	"long/=",							/* 44=LEQDIV */
	"long%=",							/* 45=LEQMOD */
	"=addr",							/* 46=EQADDR */
	"=not",								/* 47=EQNOT */
	"=neg",								/* 48=EQNEG */
	"docast",							/* 49=DOCAST */
	"st=",								/* 50=STASSIGN */
	"long->float",						/* 51=LONG2F */
	"float->long",						/* 52=FLOAT2L */
	"int->float",						/* 53=INT2F */
	"float->int",						/* 54=FLOAT2I */
	"tochar",							/* 55=TOCHAR */
	invalid,							/* 56 */
	invalid,							/* 57 */
	invalid,							/* 58 */
	invalid,							/* 59 */
	"U&",								/* 60=ADDR */
	"U*",								/* 61=INDR */
	"&&",								/* 62=LAND */
	"||",								/* 63=LOR */
	"?",								/* 64=QMARK */
	":",								/* 65=COLON */
	",",								/* 66=COMMA */
	"cint",								/* 67=CINT */
	"clong",							/* 68=CLONG */
	"symbol",							/* 69=SYMBOL */
	"++a",								/* 70=AUTOINC */
	"a--",								/* 71=AUTODEC */
	"call",								/* 72=CALL */
	"call()",							/* 73=NACALL */
	"bitfield",							/* 74=BFIELD */
	"if",								/* 75=IFGOTO */
	"init",								/* 76=INIT */
	"loadR0",							/* 77=CFORREG */
	"divlong",							/* 78=DCLONG */
	"cfloat",							/* 79=CFLOAT */
};

static const char *const types[] = {
	"typeless-invalid",					/* 0=TYPELESS */
	"char",								/* 1=CHAR */
	"short",							/* 2=SHORT */
	"int",								/* 3=INT */
	"long",								/* 4=LONG */
	"uchar",							/* 5=UCHAR */
	"ushort",							/* 6=USHORT */
	"uint",								/* 7=UINT */
	"ulong",							/* 8=ULONG */
	"float",							/* 9=FLOAT */
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
	{
		fputs("     ", stderr);
	}
}


static VOID puttsu(P(struct tnode *) tp)
PP(struct tnode *tp;)
{
	switch (SUPTYPE(tp->t_type))
	{
	case FUNCTION:
		fputs("()", stderr);
		break;

	case ARRAY:
		fputs("[]", stderr);
		break;

	case POINTER:
		fputs("*", stderr);
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
		error("INVALID op");
		return;
	}
	fprintf(stderr, "%s ", opname[op]);
	puttsu(tp);
	switch (op)
	{
	case DCLONG:
	case CLONG:
	case CFLOAT:
		fprintf(stderr, " %ld %x.%x\n", ((struct lconode *) tp)->t_lvalue, ((struct lconode *) tp)->_l.w.hiword, ((struct lconode *) tp)->_l.w.loword);
		break;

	case CINT:
		fprintf(stderr, " %d [0x%x]\n", ((struct conode *) tp)->t_value, ((struct conode *) tp)->t_value);
		break;

	case AUTODEC:
	case AUTOINC:
		fprintf(stderr, "Autodec or Autoinc\n");
		break;

	case SYMBOL:
		switch (((struct symnode *) tp)->t_sc)
		{
		case REGISTER:
			fprintf(stderr, " Register");
			break;

		case CINDR:
			fprintf(stderr, " %d", ((struct symnode *) tp)->t_offset);
			break;

		case CLINDR:
		case CFINDR:
			fprintf(stderr, " %x.%x", ((struct symnode *) tp)->t_offset, tp->t_ssp);
			break;

		case REGOFF:
			fprintf(stderr, " Regoffset");
			break;

		case EXTERNAL:
		case EXTOFF:
			fprintf(stderr, " %s+%d", ((struct extnode *) tp)->t_symbol, ((struct extnode *) tp)->t_offset);
			if (((struct symnode *) tp)->t_sc == EXTOFF)
				fprintf(stderr, " Ext offset");
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
		fputc('\n', stderr);
		break;

	case IFGOTO:
		putsexpr(tp->t_left);
		break;

	default:
		fputc('\n', stderr);
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
