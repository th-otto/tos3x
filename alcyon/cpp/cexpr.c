/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

#define OPPRI   077
#define OPBIN   0100
#define STKLEN  64
static int oprstk[STKLEN];					/* operator stack */
static long opnstk[STKLEN];					/* operand stack */
static int pristk[STKLEN];					/* operator priority stack */
static int *oprptr;							/* pointer to operator stack */
static long *opnptr;						/* pointer to operand stack */
static int *priptr;							/* pointer to priority stack */
static long cvalue;

static int const opinfo[] = {
	0,									/* CEOF=0 */
	16 | OPBIN,							/* SUB=1 */
	16 | OPBIN,							/* ADD=2 */
	20,									/* NOT=3 */
	20,									/* NEG=4 */
	22,									/* LPAREN=5 */
	2,									/* RPAREN=6 */
	6 | OPBIN,							/* QMARK=7 */
	6 | OPBIN,							/* COLON=8 */
	8 | OPBIN,							/* OR=9 */
	10 | OPBIN,							/* AND=10 */
	8 | OPBIN,							/* XOR=11 */
	12 | OPBIN,							/* EQUAL=12 */
	12 | OPBIN,							/* NEQUAL=13 */
	14 | OPBIN,							/* LESS=14 */
	14 | OPBIN,							/* LSEQUAL=15 */
	14 | OPBIN,							/* GREAT=16 */
	14 | OPBIN,							/* GREQUAL=17 */
	4 | OPBIN,							/* LSHIFT=18 */
	4 | OPBIN,							/* RSHIFT=19 */
	18 | OPBIN,							/* MULT=20 */
	18 | OPBIN,							/* DIV=21 */
	18 | OPBIN,							/* MOD=22 */
	20,									/* COMPL=23 */
};


/*
 * stkop - stack an operator on the operand stack
 *
 * Unstack all operators of lower priority, evaluating them as
 * they are unstacked.
 * returns 1 if ok, 0 otherwise
 */
static int stkop(P(int) opr)
PP(int opr;)
{
	register long op1;
	register long op2;
	register int pri;

	op2 = 0; /* avoid compiler warning */
	for (pri = opinfo[opr] & OPPRI; pri < *priptr;)
	{
		if (*oprptr == LPAREN)
		{
			if (opr == RPAREN)
			{
				oprptr--;
				priptr--;
				return 1;
			}
			break;
		}
		op1 = *opnptr;
		if (opinfo[*oprptr] & OPBIN)
		{
			op2 = op1;
			op1 = *--opnptr;
		}
		switch (*oprptr)
		{								/* operator */
		case ADD:
			op1 += op2;
			break;

		case SUB:
			op1 -= op2;
			break;

		case COLON:
			priptr--;
			if (*--oprptr != QMARK)
				return 0;
			op1 = (*--opnptr ? op1 : op2);
			break;

		case QMARK:
			return 0;

		case XOR:
			op1 ^= op2;
			break;

		case OR:
			op1 |= op2;
			break;

		case AND:
			op1 &= op2;
			break;

		case EQUAL:
			op1 = op1 == op2;
			break;

		case NEQUAL:
			op1 = op1 != op2;
			break;

		case LESS:
			op1 = op1 < op2;
			break;

		case LSEQUAL:
			op1 = op1 <= op2;
			break;

		case GREAT:
			op1 = op1 > op2;
			break;

		case GREQUAL:
			op1 = op1 >= op2;
			break;

		case LSHIFT:
			op1 = op1 << op2;
			break;

		case RSHIFT:
			op1 = op1 >> op2;
			break;

		case NEG:
			op1 = -op1;
			break;

		case NOT:
			op1 = !op1;
			break;

		case COMPL:
			op1 = ~op1;
			break;

		case MULT:
			op1 *= op2;
			break;

		case DIV:
			op1 /= op2;
			break;

		case MOD:
			op1 %= op2;
			break;

		}
		*opnptr = op1;
		priptr--;
		oprptr--;
	}
	if (priptr >= &pristk[STKLEN - 1])
	{
		error(_("expression operator stack overflow"));
		cexit();
	}
	*++oprptr = opr;					/* operator value */
	*++priptr = pri;					/* operator priority */
	return 1;
}


/*
 * getctok - get a constant expression token
 * Handles conversion of quoted character strings and numbers.
 */
static int getctok(NOTHING)
{
	register int type;
	register int c;
	register int count;
	register struct symbol *sp;
	register char *p;
	char token[TOKSIZE];

	for (;;)
	{
		switch (type = getntok(token))
		{
		case DIGIT:
			cvalue = constexpr(token);
			return CONST;

		case SQUOTE:
			for (cvalue = 0, p = &token[1], count = sizeof(cvalue); --count >= 0;)
			{
				if ((c = *p++) == '\'')
					break;
				if (c == '\\')
				{
					if (*p >= '0' && *p <= '7')
					{
						for (c = 0; *p >= '0' && *p <= '7';)
							c = (c << 3) + (*p++ - '0');
					} else
					{
						switch (c = *p++)
						{
						case 'n':
							c = 0x0a;
							break;

						case 't':
							c = 0x09;
							break;

						case 'b':
							c = 0x08;
							break;

						case 'r':
							c = 0x0d;
							break;

						case 'f':
							c = 0x0c;
							break;

						case 'a':
							c = 0x07;
							break;

						case 'v':
							c = 0x0b;
							break;

						case 'e':
							c = 0x1b;
							break;
						
						case '\'':
						case '"':
						case '\\':
							break;
						
						case 'x':
							for (c = 0; ;)
							{
								if (*p >= '0' && *p <= '9')
									c = (c << 4) + (*p++ - '0');
								else if (*p >= 'a' && *p <= 'f')
									c = (c << 4) + (*p++ - 'a' + 10);
								else if (*p >= 'A' && *p <= 'F')
									c = (c << 4) + (*p++ - 'A' + 10);
								else
									break;
							}
							c = (char)c;
							break;
							
						default:
							warning(_("unknown escape sequence '\\%c'"), c);
							break;
						}
					}
				}
				cvalue = (cvalue << 8) | c;
			}
			if (count < (sizeof(cvalue) - 2))
				warning(_("multi-character character constant"));
			if (cvalue < -128 || cvalue > 255)
				warning(_("character constant out of range"));
			return CONST;

		case ALPHA:
			if ((sp = lookup(token)) != NULL)
			{
				expand(sp);
			} else
			{
				warning(_("\"%s\" is not defined"), token);
				cvalue = 0;
				return ALPHA;
			}
			break;

		default:
			return type;
		}
	}
}


/*
 * cexpr - constant expression evaluation
 *      Does priority-driven operator/operand stack evaluation of
 *      constant expressions.
 */
long cexpr(NOTHING)									/* returns constant evaluated */
{
	register int lop, type;
	register long v;
	
	oprptr = &oprstk[0];
	opnptr = &opnstk[0];
	priptr = &pristk[0];
	*priptr = 0;
	lop = -1;
	for (;;)
	{
		switch (type = getctok())
		{
		case ALPHA:
		case CONST:
			if (!lop)					/* last was not operator */
				goto syntax;
			if (opnptr >= &opnstk[STKLEN])
			{
				error(_("expression stack overflow"));
				cexit();
			}
			lop = FALSE;
			*++opnptr = cvalue;
			continue;

		case SUB:
			if (lop)
				type = NEG;				/* unary minus */
			break;

		case ADD:
			if (lop)
				continue;				/* ignore unary + */
			break;

		case COMPL:
		case LPAREN:
		case NOT:
			if (!lop)
				goto syntax;
			break;

		case RPAREN:
			if (lop)
				goto syntax;
			lop = FALSE;
			if (!stkop(type))
				goto syntax;
			continue;

		case NEWL:
		case CEOF:
			if (lop || !stkop(CEOF) || opnptr != &opnstk[1])
				goto syntax;
			v = opnstk[1];
			putback('\n');
			return v;

		default:
			if (lop || type > LASTOP)
				goto syntax;
			break;
		}
		lop = TRUE;
		if (!stkop(type))
			goto syntax;
	}
  syntax:
	error(_("expression syntax"));
	if (type == NEWL)
		putback('\n');
	return 0;
}


#undef toupper
#define toupper(c)  ((c) & ~32)
/*
 * constexpr - alpha to int conversion, handles octal and hexadecimal
 *      Uses Horner's method to evaluate number.
 * returns number evaluated
 */
long constexpr(P(const char *) str)
PP(const char *str;)
{
	register int c, radix;
	register long i;
	
	i = 0;
	radix = 10;
	if (*str == '0')
	{
		radix = 8;
		if (*++str == 'x' || *str == 'X')
		{
			radix = 16;
			str++;
		}
	}
	while ((c = *str++) != 0)
	{
		if (c >= '0' && c <= '9')
			c -= '0';
		else if ((c = toupper(c)) >= 'A' && c <= 'F')
			c = c - ('A' - 10);
		else
			break;
		if (c >= radix)
			break;
		i = i * radix + c;
	}
	return i;
}
