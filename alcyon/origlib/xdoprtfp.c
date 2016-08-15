#include "lib.h"
#include <math.h>

char *pftoa(P(double, fp), P(char *, buf), P(int, prec))
PP(double, fp;)
PP(char *, buf;)
PP(int, prec;)
{
	if (prec < 0)
		prec = 6;
	return ftoa(fp, buf, prec);
}

char *petoa(P(double, fp), P(char *, buf), P(int, prec))
PP(double, fp;)
PP(char *, buf;)
PP(int, prec;)
{
	if (prec < 0)
		prec = 6;
	return etoa(fp, buf, prec);
}
