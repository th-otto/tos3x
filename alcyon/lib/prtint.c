/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) __prtint.c - Sep 12, 1983  REGULUS 4.1
*/

#include "lib.h"


char *__prtint(P(long, n), P(char *, buf), P(int, base), P(int, issigned), P(printfunc, f), P(int, upper))
PP(long, n;)
PP(char *, buf;)
PP(int, base;)
PP(int, issigned;)
PP(printfunc, f;)
PP(int, upper;)
{
	char digs[15];
	register char *dp;
	register int k;
	register char *p;

	dp = (*f) (n, &buf, base, issigned, digs);

	if (dp == digs)
		*dp++ = 0;
	p = buf;
	while (dp != digs)
	{
		k = *--dp;
		if (k < 10)
			k += '0';
		else
			k += upper ? 'A' - 10 : 'a' - 10;
		*p++ = k;
	}
	*p = 0;
	return p;
}
