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
#include <portab.h>

int strcmp(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)
PP(register const char *s2;)
{
	register UBYTE c1, c2;

	do
    {
		c1 = *s1++;
		c2 = *s2++;
    } while (c1 && c1 == c2);
	return c1 - c2;
}
