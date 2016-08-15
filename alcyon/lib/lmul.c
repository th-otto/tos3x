/*
 * long multiply routine without floating point
 *  call with:
 *		two long values on stack
 *  returns:
 *		long value in R0 and R1
 *
 * warning:  no overflow checking or indication!!!!
 */

#include "lib.h"

long lmul(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
	register union ll t1;
	register union ll l1;
	register union ll l2;
	register int sign;
	register short t2;

	l1.l = al1;
	l2.l = al2;
	sign = 0;
	if (l1.l < 0)
	{
		l1.l = -l1.l;					/* make it positive */
		sign++;
	}
	if (l2.l < 0)
	{
		l2.l = -l2.l;					/* make it positive */
		sign++;
	}
	t1.l = l1.w.loword * l2.w.loword;
	t2 = l1.w.hiword * l2.w.loword + l2.w.hiword * l1.w.loword;
	t1.w.hiword = t1.w.hiword + t2;
	if (sign & 1)
		t1.l = -t1.l;					/* negate results */
	return t1.l;
}
