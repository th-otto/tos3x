/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) __prtint.c - Sep 12, 1983  REGULUS 4.1
*/

#include "lib.h"


char *__prtshort(P(register long) n, P(char **) pbuf, P(int) base, P(int) issigned, P(char *) digs)
PP(long n;)
PP(char **pbuf;)
PP(int base;)
PP(int issigned;)
PP(char *digs;)
{
	register char *p;
	register long b;

	p = digs;
	b = base;
	if (issigned && n < 0)
	{
		n = -n;
		*(*pbuf)++ = '-';
	} else
	{
		n &= 0xffffL;					/* clear upper half */
	}
	
	while (n != 0)
	{
		*p++ = n % b;
		n /= b;
	}
	return p;
}
