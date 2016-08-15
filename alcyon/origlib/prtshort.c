/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) __prtint.c - Sep 12, 1983  REGULUS 4.1
*/

#include "lib.h"
#include "osif.h"


char *__prtshort(P(char *, pobj), P(char **, pbuf), P(int, base), P(int, issigned), P(char *, digs))
PP(char *, pobj;)
PP(char **, pbuf;)
PP(int, base;)
PP(int, issigned;)
PP(char *, digs;)
{
	register unsigned int n;
	register char *p;
	register int b;

#if HILO
	long ln;

	ln = *((int *)pobj);							/* assign arg to long       */
	n = ln & 0xFFFFL;					/* then to int         */
#else
	n = *((int *)pobj);
#endif
	p = digs;
	b = base;
	if (issigned)
	{
		if ((int) n < 0)
		{
			asm("clr.w d0"); /* WTF */
			n = -n;
			*(*pbuf)++ = '-';
		}
	}
	while (n != 0)
	{
		*p++ = n % b;
		n = n / b;
	}
	return p;
}
