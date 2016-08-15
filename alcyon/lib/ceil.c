/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include <math.h>

/* ceil - returns the smallest integer (as a double precision
		  number) not greater than x. */

double ceil(P(double, x))
PP(double, x;)
{
	register long i;
	double retval;

	if (x > 0.0)
		x += 0.999999999999;
	i = x;
	retval = i;
	return (retval);
}
