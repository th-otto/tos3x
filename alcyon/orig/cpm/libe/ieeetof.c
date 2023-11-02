/*
        Copyright 1982
        Alcyon Corporation
        8716 Production Ave.
        San Diego, Ca.  92121
*/

/*char *version "@(#) _ieeetof - dec 29, 1982"; */

#include "lib.h"

/*
 *      IEEE Floating Point Representation to Internal Representation :
 *              IEEE Standard Single Precision Representation Floating Point
 *
 *      float
 *      _ieeetof(lf)
 *      long lf;
 *
 *      Largest positive number is 3.4 * 10^33 and the smallest positive
 *      number is 1.2 * 10^-38.
 *      Rely's on the fact that a long and a float are both 32 bits.
 */

#define BIAS    127L

double _ieeetof(P(long) lf)
PP(long lf;)
{
	register long exp;
	register int count, fsign;
	float f;

	if (lf == 0)
		return 0.0;
	if (lf < 0L)
	{
		fsign = 1;
		lf &= 0x7fffffff;		/* mask MSB (sign) */
	} else
	{
		fsign = 0;
	}
	exp = (lf >> 23) & 0xff;				/* biased ieee exponent */
	exp -= BIAS;
	lf &= 0x7fffff; 		/* 23 bits of fraction */
	f = lf;
	if (lf != 0)			/* don't need to do floating point test !!! */
		f = f / 8388608.0;		/* 2 ^ 23 */
	f = f + 1.0;			/* ieee fraction : 1.F */
	while (exp < 0)			/* negative exp : 2^-? */
	{
		f = f / 2.0;
		exp++;
	}
	while (exp > 0)		/* positive exp : 2^+? */
	{
		f = f * 2.0;
		exp--;
	}
	if (fsign)
		f = -f;
	return f;
}
