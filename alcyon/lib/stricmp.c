/*********************************************************************
*    STRCMP  -  compares strings
*	
*
*	Special version which is case - insensitive.
*
*	int strcasecmp(s1,s2)
*	char *s1, *s2;
*
*	'strcmp' compares null terminated strings s1 and s2.
*	Returns:
*		strcasecmp < 0  if  s1<s2
*		strcasecmp = 0  if  s1=s2
*		strcasecmp > 0  if  s1>s2
*********************************************************************/

#include "lib.h"
#include <string.h>
#include <ctype.h>
#include <portab.h>

int strcasecmp(P(const char *) s1, P(const char *) s2)
PP(register const char *s1;)
PP(register const char *s2;)
{
	register unsigned int c1, c2;

	do
    {
		c1 = *s1++;
		c2 = *s2++;
		c1 = toupper((UBYTE) c1);
		c2 = toupper((UBYTE) c2);
    } while (c1 && c1 == c2);
	return c1 - c2;
}
