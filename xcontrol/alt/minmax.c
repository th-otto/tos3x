#include "aesalt.h"

WORD mymin(WORD a, WORD b)
{
	if (a < b)
		return a;
	return b;
}


WORD mymax(WORD a, WORD b)
{
	if (a > b)
		return a;
	return b;
}
