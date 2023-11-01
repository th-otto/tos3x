/*********************************************************************
*    STRCMP  -  compares strings
*	
*	Special version which is case - insensitive.
*
*	WORD strcmp(s1,s2)
*	BYTE *s1, *s2;
*
*	'strcmp' compares null terminated strings s1 and s2.
*	Returns:
*		strcmp < 0  if  s1<s2
*		strcmp = 0  if  s1=s2
*		strcmp > 0  if  s1>s2
*********************************************************************/

#include <string.h>
#include "klib.h"

static char _toupper PROTO((char c));

int xstrcmp(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)
PP(register const char *s2;)
{
	register char a, b;

	while (*s1 && *s2)
	{
		a = _toupper(*s1++);
		b = _toupper(*s2++);
		if (a > b)
			return 1;
		if (a < b)
			return -1;
	}
	return 0;
}


static char _toupper(P(char) c)
PP(register char c;)
{
	if (c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	return c;
}
