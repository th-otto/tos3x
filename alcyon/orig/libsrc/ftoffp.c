/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "lib.h"

/*
 *	Floating Point to FFP Floating Point Routine :
 *		FFP Standard Single Precision Representation Floating Point
 *
 *	long
 *	fptoffp(f)
 *	float f;
 *
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

long fptoffp(P(double) f)							/* convert current machine float to ffp rep */
PP(double f;)								/* unsigned input, guaranteed positive */
{
	register int exp, sign;
	long l;

	if (f == 0.0)
		return (0L);
	if (f < 0.0)
	{
		f = -f;
		sign = 1;
	} else
		sign = 0;
	exp = 0L;
	for (; f >= 1.0; f = f / 2.0)
		exp++;
	for (; f < 0.5; f = f * 2.0)
		exp--;
	f = f * 16777216.0;					/* 2 ^ 24 */
	l = f;
	l <<= 8;
	exp += 0x40;
	l |= (exp & 0x7f);
	if (sign)
		l |= 0x80;
	return (l);
}
