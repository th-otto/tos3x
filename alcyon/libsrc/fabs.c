/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include <math.h>

/* 
 *	Floating Point Absolute :
 *		Fast Floating Point Package
 *
 *		double
 *		fabs(farg)
 *		double farg;
 *
 *	Returns : absolute Floating point number
 */

double fabs(P(double) f)
PP(double f;)
{
	union {
		double f;
		long l;
	} u;
	
	u.f = f;
	u.l &= 0xffffff7f;					/* turn off sign bit */
	return u.f;
}
