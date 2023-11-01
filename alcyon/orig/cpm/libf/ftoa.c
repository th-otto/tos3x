/*
 *	Ftoa routine with rounding
 */
#include "lib.h"
#include <math.h>

char *ftoa(P(double) x, P(char *) str, P(int) prec)
PP(double x;)								/* Arg to convert   */
PP(char *str;)								/* -> Output area   */
PP(int prec;)								/* # digits right of dp */
{
	register int ie;
	register int i;
	register int k;
	register int ndig;						/* Temps        */

	char *savstr;						/* Copy of str to return */
	double y;							/* Temp for rounding    */

	savstr = str;						/* Preserve for later   */
	ndig = (prec <= 0) ? 1 : ((prec > 22) ? 23 : prec + 1);
	ie = 0;
	if (x < 0.0)						/* Fix for negative */
	{
		*str++ = '-';
		x = -x;							/* Negate       */
	}
	/*
	 *	Normalize x to the range 0.0 <= x < 10.0
	 */
	if (x > 0.0)
	{
		while (x < 1.0)
		{
			x *= 10.0;
			ie--;
		}
	}
	while (x >= 10.0)
	{
		x /= 10.0;
		ie++;
	}
	ndig += ie;							/* Adjust digit count for size  */
	/*
	 *	Now round.
	 */
	for (y = i = 1; i < ndig; i++)
		y = y / 10.0;					/* Compute round amount */
	x += (y / 2.0);						/* Round by 1/2 lsb */
	if (x >= 10.0)						/* Did we push it over 10? */
	{
		x = 1.0;
		ie++;
	}
	/*
	 *	Now convert result
	 */
	if (ie < 0)							/* Leading zeros are special */
	{
		*str++ = '0';
		*str++ = '.';
		if (ndig < 0)
			ie = ie - ndig;				/* For underflow */
		for (i = -1; i > ie; i--)		/* Out the zeros */
			*str++ = '0';
	}

	for (i = 0; i < ndig; i++)
	{
		k = x;							/* Truncate */
		*str++ = k + '0';				/* ASCIIfy  */
		if (i == ie)					/* Locate decimal point */
			*str++ = '.';
		x -= (y = k);
		x *= 10.0;
	}
	*str++ = '\0';
	return savstr;
}
