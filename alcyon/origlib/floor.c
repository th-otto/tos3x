/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include <math.h>

/* floor - returns the largest integer (as a double precision
		   number) not greater than x. */

double floor(P(double) x)
PP(double x;)
{
	register long i;
	double retval;

	if (x < 0.0)
		x -= 0.99999999999999;
	i = x;
	retval = i;
	return retval;
}
