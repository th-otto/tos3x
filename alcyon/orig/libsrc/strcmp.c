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

#include <string.h>


int strcmp(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)
PP(register const char *s2;)
{
	register char a;
	register char b;

	while (*s1 && *s2)
	{
		a = *s1++;
		b = *s2++;
		if (a > b)
			return 1;
		if (a < b)
			return -1;
	}
	return *s1 - *s2;
}
