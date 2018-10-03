#include "aesalt.h"

WORD min(WORD a, WORD b)
{
	if (a < b)
		return a;
	return b;
}


WORD max(WORD a, WORD b)
{
	if (a > b)
		return a;
	return b;
}
