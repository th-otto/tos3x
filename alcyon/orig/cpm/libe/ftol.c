/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "lib.h"

/* char *version "@(#)ftol.c	1.2    10/19/83"; */

/* 
 *	Floating Point Float to Long Routine :
 *		Front End to IEEE Floating Point Package.
 *
 *	long
 *	fpftol(fparg)
 *	double fparg;
 *
 *	Return : Fixed Point representation of Floating Point Number
 */

#define BIAS	127L

long fpftol(P(long) f)
PP(long f;)
{
	register long l;
	register int exp, sign;

	l = (f & 0x7f800000) >> 23;
	exp = l - BIAS;
	if (f == 0L || exp < 0)		/* underflow or 0 */
		return 0;
	sign = (f < 0L);
	if (exp > 31)				/* overflow */
		return sign ? 0x80000000 : 0x7fffffff;
	exp -= 23;
	l = (f & 0x7fffff) | 0x800000;	/* 1.F */
	for( ; exp < 0 ; exp++)
		l >>= 1;
	for( ; exp > 0; exp--)
		l <<= 1;
	if (sign)
		l = -l;
	return l;
}

