/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*
 *	Ascii String to FFP Floating Point Routine :
 *		FFP Standard Single Precision Representation Floating Point
 *
 *	float
 *	atof(buf)
 *	char *buf;
 *
 *	No more than 9 significant digits are allowed in single precision.
 *	Largest positive number is 3.4 * 10^18 and the smallest positive
 *	number is 1.2 * 10^-20.
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

#include "lib.h"
#include <math.h>

#define EXPSIZ	4
#define FRACSIZ	20



static double power10(P(int) pwr)						/* 10^pwr */
PP(int pwr;)
{
	double f;

	if (pwr < 0)						/* negative power */
		for (f = 1.0; pwr < 0; pwr++)
			f = f / 10.0;
	else								/* positive power */
		for (f = 1.0; pwr > 0; pwr--)
			f = f * 10.0;
	return f;
}


static double strbin(P(const char *) p)							/* decimal string => binary long */
PP(const char *p;)
{
	double f;

	for (f = 0.0; *p >= '0' && *p <= '9'; p++)
	{
		f = f * 10.0;
		f = f + (*p - '0');
	}
	return f;
}


double atof(P(const char *) buf)
PP(const char * buf;)
{
	char ibuf[FRACSIZ];
	char ebuf[EXPSIZ];
	register char *ip, *ep;
	int dp, esign, isign, ebin, places;
	double ibin, fp;

	ip = ibuf;
	ep = ebuf;
	dp = 0;
	places = 0L;
	while (*buf == ' ' || *buf == '\t')	/* ignore white spaces */
		buf++;
	isign = (*buf == '-');
	if (*buf == '-' || *buf == '+')
		buf++;
	while (*buf && *buf != 'e' && *buf != 'E')
	{
		if (*buf == '.')
			dp++;
		else
		{								/* digit seen */
			*ip++ = *buf;
			if (dp)
				places++;
		}
		buf++;
	}
	*ip = 0;
	esign = 0;
	if (*buf == 'e' || *buf == 'E')
	{									/* exponent string */
		buf++;
		esign = (*buf == '-');
		if (*buf == '-' || *buf == '+')
			buf++;
		while (*buf)					/* get exponent string */
			*ep++ = *buf++;
	}
	*ep = 0;
	ibin = strbin(ibuf);
	ebin = atoi(ebuf);
	places = (esign) ? -ebin - places : ebin - places;
	fp = ibin * power10(places);
	if (isign)
		fp = -fp;
	return fp;
}
