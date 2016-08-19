#include "lib.h"

long ldivr;

long ldiv(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
	register long l1, l2, q, b;
	register int sign;

#ifdef __ALCYON__
	asm("  xdef ldiv");
	asm("ldiv equ _ldiv");
#endif
	sign = 0;
	l1 = al1;
	l2 = al2;
	if (l2 == 0)
	{
		ldivr = 0x80000000;
		return 0x80000000;
	}
	if (l2 > l1)
	{
		ldivr = l1;
		return 0;
	}
	if (l1 < 0)
	{
		l1 = -l1;
		sign++;
	}
	if (l2 < 0)
	{
		l2 = -l2;
		sign++;
	}
	if (l1 == l2)
	{
		q = 1;
		l1 = 0;
		goto doret;
	}
	b = 1;								/* bit value */
	while (l1 >= l2)
	{
		l2 <<= 1;
		b <<= 1;
	}
	q = 0;
	while (b)
	{									/* now do shifts and subtracts */
		if (l1 >= l2)
		{
			q |= b;
			l1 -= l2;
		}
		b >>= 1;
		l2 >>= 1;
	}
  doret:
	if (sign == 1)
	{
		ldivr = -l1;
		return -q;
	}
	ldivr = l1;
	return q;
}
