/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "preproc.h"

#define OPPRI   077
#define OPBIN   0100
#define STKLEN  64
int oprstk[STKLEN];						/*operator stack */

int opnstk[STKLEN];						/*operand stack */

int pristk[STKLEN];						/*operator priority stack */

int *oprptr;							/*pointer to operator stack */

int *opnptr;							/*pointer to operand stack */

int *priptr;							/*pointer to priority stack */

int cvalue;

int opinfo[] = {
	0,									/*CEOF=0 */
	16 | OPBIN,							/*SUB=1 */
	16 | OPBIN,							/*ADD=2 */
	20,									/*NOT=3 */
	20,									/*NEG=4 */
	22,									/*LPAREN=5 */
	2,									/*RPAREN=6 */
	6 | OPBIN,							/*QMARK=7 */
	6 | OPBIN,							/*COLON=8 */
	8 | OPBIN,							/*OR=9 */
	10 | OPBIN,							/*AND=10 */
	8 | OPBIN,							/*XOR=11 */
	12 | OPBIN,							/*EQUAL=12 */
	12 | OPBIN,							/*NEQUAL=13 */
	14 | OPBIN,							/*LESS=14 */
	14 | OPBIN,							/*LSEQUAL=15 */
	14 | OPBIN,							/*GREAT=16 */
	14 | OPBIN,							/*GREQUAL=17 */
	4 | OPBIN,							/*LSHIFT=18 */
	4 | OPBIN,							/*RSHIFT=19 */
	18 | OPBIN,							/*MULT=20 */
	18 | OPBIN,							/*DIV=21 */
	18 | OPBIN,							/*MOD=22 */
	20,									/*COMPL=23 */
};

/* cexpr - constant expression evaluation*/
/*      Does priority-driven operator/operand stack evaluation of*/
/*      constant expressions.*/
int cexpr(void)									/* returns constant evaluated */
{
	register int lop, type;

	oprptr = &oprstk[0];
	opnptr = &opnstk[0];
	priptr = &pristk[0];
	*priptr = 0;
	lop = -1;
	while (1)
	{
		switch (type = getctok())
		{
		case CONST:
			if (!lop)					/*last was not operator */
				goto syntax;
			if (opnptr >= &opnstk[STKLEN])
			{
				error("expression stack overflow");
				cexit();
			}
			lop = FALSE;
			*++opnptr = cvalue;
			continue;

		case SUB:
			if (lop)
				type = NEG;				/*unary minus */
			break;

		case ADD:
			if (lop)
				continue;				/*ignore unary + */
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
			type = opnstk[1];
			putback('\n');
			return type;

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
	error("expression syntax");
	if (type == NEWL)
		putback('\n');
	return 0;
}


/* getctok - get a constant expression token*/
/*      Handles conversion of quoted character strings and numbers.*/
int getctok(void)
{
	register int type, c, count;
	register struct symbol *sp;
	register char *p;
	char token[TOKSIZE];

	while (1)
	{
		switch (type = getntok(token))
		{

		case DIGIT:
			cvalue = constexpr(token);
			return CONST;

		case SQUOTE:
			for (cvalue = 0, p = &token[1], count = 2; --count >= 0;)
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
						switch (c = *p++)
						{
						case 'n':
							c = '\n';
							break;

						case 't':
							c = '\t';
							break;

						case 'b':
							c = '\b';
							break;

						case 'r':
							c = '\r';
							break;

						case 'f':
							c = '\f';
							break;
						}
				}
				cvalue = (cvalue << 8) | c;
			}
			return CONST;

		case ALPHA:
			if ((sp = lookup(token)) != NULL)
				expand(sp);
			else
				return ALPHA;
			break;

		default:
			return type;
		}
	}
}


/* stkop - stack an operator on the operand stack */
/*      Unstack all operators of lower priority, evaluating them as*/
/*      they are unstacked.*/
int stkop(int opr)
{
	register int op1, op2 = 0, pri;

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
		{								/*operator */
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
			op1 = (op1 == op2);
			break;

		case NEQUAL:
			op1 = (op1 != op2);
			break;

		case LESS:
			op1 = (op1 < op2);
			break;

		case LSEQUAL:
			op1 = (op1 <= op2);
			break;

		case GREAT:
			op1 = (op1 > op2);
			break;

		case GREQUAL:
			op1 = (op1 >= op2);
			break;

		case LSHIFT:
			op1 = (op1 << op2);
			break;

		case RSHIFT:
			op1 = (op1 >> op2);
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
		error("expression operator stack overflow");
		cexit();
	}
	*++oprptr = opr;					/*operator value */
	*++priptr = pri;					/*operator priority */
	return 1;
}


#define toupper(c)  ((c) & ~32)
/* constexpr - alpha to int conversion, handles octal and hexidecimal */
/*      Uses Horner's method to evaluate number.*/
int constexpr(const char *str)
{
	register int c, ch, i, radix;

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
		if ((ch = toupper(c)) >= 'A' && ch <= 'F')
			c = ch - ('A' - 10);
		else if (c >= '0' && c <= '9')
			c -= '0';
		else
			break;
		if (c >= radix)
			break;
		i = i * radix + c;
	}
	return i;
}
