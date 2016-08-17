/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "lib.h"

/*
 *	FFP Floating Point Representation to Internal Representation :
 *		FFP Standard Single Precision Representation Floating Point
 *
 *	float
 *	ffptof(lf)
 *	long lf;
 *
 *	Largest positive number is 3.4 * 10^18 and the smallest positive
 *	number is 1.2 * 10^-20.
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

double ffptof(P(long) lf)
PP(long lf;)
{
	register int exp, fsign;
	double f;

	if (lf == 0L)
		return (0.0);
	fsign = (lf & 0x80);
	exp = (lf & 0x7f) - 0x40;
	lf = (lf >> 8) & 0xffffff;			/* 24 bits of fraction */
	f = lf;
	f = f / 16777216.0;					/* 2 ^ 24 */
	while (exp < 0)
	{									/* negative exp : 2^-? */
		f = f / 2.0;
		exp++;
	}
	while (exp > 0)
	{									/* positive exp : 2^+? */
		f = f * 2.0;
		exp--;
	}
	if (fsign)
		f = -f;
	return f;
}
