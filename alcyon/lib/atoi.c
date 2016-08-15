/* atoi - convert decimal number in ascii to integer */
#include "lib.h"
#include <ctype.h>
#include <stdlib.h>
#include <portab.h>

int atoi(P(const char *, s))
PP(register const char *, s;)
{
	register int val;
	register int isneg;

	val = 0;
	isneg = 0;
	while (isspace((UBYTE)*s))
		s++;
	if (*s == '+')
		s++;
	else if (*s == '-')
	{
		s++;
		isneg++;
	}
	while (*s >= '0' && *s <= '9')
		val = 10 * val + (*s++ - '0');
	if (isneg)
		val = -val;
	return val;
}
