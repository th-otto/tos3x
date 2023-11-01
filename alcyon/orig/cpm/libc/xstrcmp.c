/*********************************************************************
*    STRCMP  -  compares strings
*	
*
*	int strcmp(s1,s2)
*	char *s1, *s2;
*
*	'strcmp' compares null terminated strings s1 and s2.
*	Returns:
*		strcmp < 0  if  s1<s2
*		strcmp = 0  if  s1=s2
*		strcmp > 0  if  s1>s2
*********************************************************************/

#include "lib.h"
#include <string.h>

static char _toupper PROTO((int c));

int _strcmp(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)
PP(register const char *s2;)
{
	register char a;
	register char b;

	while (*s1 || *s2) /* BUG: || */
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


static char _toupper(P(int) c)
PP(register int c;)
{
	if (c >= 'a' && c <= 'z')
		c += 'A' - 'a';
	return c;
}
