/*
 *	Ftoa routine with rounding
 */
#include "lib.h"
#include <math.h>
#include <string.h>

char *gtoa(P(double) x, P(char *) buf, P(int) prec)
PP(double x;)								/* Arg to convert   */
PP(char *buf;)							/* -> Output area   */
PP(int prec;)								/* # digits right of dp */
{
	char *sp;
	
	sp = pftoa(x, buf, prec);
	if (strlen(buf) > (7 + prec))	/* smallest fp string */
		sp = petoa(x, buf, prec);
	return sp;
}
