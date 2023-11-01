/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) __prtint.c - Sep 12, 1983  REGULUS 4.1
*/

#include "lib.h"


char *__prtint(P(char *) pobj, P(char *) buf, P(int) base, P(int) issigned, P(printfunc) f)
PP(char *pobj;)
PP(char *buf;)
PP(int base;)
PP(int issigned;)
PP(printfunc f;)
{
	char digs[15];
	register char *dp;
	register int k;
	register char *p;

	dp = (*f) (pobj, &buf, base, issigned, digs);

	if (dp == digs)
		*dp++ = 0;
	p = buf;
	while (dp != digs)
	{
		k = *--dp + '0';
		if (k > '9')
			k += 'A' - 10 - '0';
		*p++ = k;
	}
	*p = 0;
	return p;
}
