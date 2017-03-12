/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

#include "lib.h"

char *__prtld(P(register long) n, P(char **) pbuf, P(int) base, P(int) issigned, P(char *) digs)
PP(register long n;)
PP(char **pbuf;)
PP(int base;)
PP(int issigned;)
PP(char *digs;)
{
	register long b;
	register char *p;
	register int i;

	p = digs;
	b = base;
	if (base == 16)
	{									/* special because of negatives */
		i = 8;
		while (n && i)
		{
			*p++ = n & 0xf;
			n >>= 4;
			i--;
		}
	} else if (base == 8)
	{
		i = 11;
		while (n && i)
		{
			*p++ = n & 7;
			n >>= 3;
			i--;
		}
		if (i == 0)
		{
			*(p - 1) &= 3;				/* only 2 bits in upper octal digit */
		}
	} else
	{
		if (issigned && n < 0)
		{
			*(*pbuf)++ = '-';
			n = -n;
		}
		while (n)
		{
			*p++ = ulrem(n, b);
			n = uldiv(n, b);
		}
	}
	return p;
}
