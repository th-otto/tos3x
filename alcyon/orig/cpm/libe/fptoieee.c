#include "lib.h"
#include <math.h>

#define BIAS    127L

long fptoieee(P(double) f)				/* convert current machine float to ieee rep */
PP(double f;)							/* unsigned float... */
{
	register long exp, l;

	if (f == 0.0)
		return 0;
	exp = 0L;
	for (; f >= 2.0; f = f / 2.0)
		exp++;
	for (; f < 1.0; f = f * 2.0)
		exp--;
	f = f - 1.0;						/* implicit 1, eg. 1.F */
	if (f != 0.0)
		f = f * 8388608.0;				/* 2 ^ 23 */
	l = f;
	exp += BIAS;
	l |= ((exp << 23) & 0x7f800000);
	return l;
}
