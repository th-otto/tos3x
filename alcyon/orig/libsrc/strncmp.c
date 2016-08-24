/*********************************************************************
*    STRNCMP  -  compares strings up to n chars
*
*	int strncmp(s1,s2,n)
*	char *s1, *s2;
*	UWORD n;
*
*	'strncmp' compares null terminated strings s1 and s2, and examines
*	  at most n chars.
*	Always compares at least 1 char.
*	n < 0 compares many, many characters.
*	Returns:
*		strncmp < 0  if  s1<s2  (within n chars)
*		strncmp = 0  if  s1=s2	   "    "   "
*		strncmp > 0  if  s1>s2     "    "   "
*********************************************************************/

#include <string.h>

int strncmp(P(const char *) s1, P(const char *) s2, P(size_t) num)
PP(register const char *s1;)
PP(register const char *s2;)
PP(register int num;)
{
	for (; --num > 0 && (*s1 == *s2); s1++, s2++)
		if (*s1 == '\0')
			return 0;
	return *s1 - *s2;
}
