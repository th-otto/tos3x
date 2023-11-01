/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

#include "lib.h"
#include "osif.h"

extern long uldiv PROTO((long, long));

extern struct long_struct uldivr;


char *__prtld(P(char *) pobj, P(char **) pbuf, P(int) base, P(int) issigned, P(char *) digs)
PP(char *pobj;)
PP(char **pbuf;)
PP(int base;)
PP(int issigned;)
PP(char *digs;)
{
	register long n;
	register long b;
	register char *p;
	register int ii;

	struct long_struct *p_uldivr;

	p_uldivr = &uldivr;
	p = digs;
	b = base;
	n = *((long *)pobj);
	if (base == 16)
	{									/* special because of negatives */
		ii = 8;
		while (n && ii)
		{
			*p++ = (int) n & 0xf;
			n >>= 4;
			ii--;
		}
	} else if (base == 8)
	{
		ii = 11;
		while (n && ii)
		{
			*p++ = (int) n & 7;
			n >>= 3;
			ii--;
		}
		if (ii == 0)
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
			n = uldiv(n, b);			/* n>0 (or unsigned), do unsigned div */
			*p++ = p_uldivr->lblolo;
		}
	}
	return p;
}
