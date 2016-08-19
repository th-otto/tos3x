/**********************************************************************
*    STRLEN  -  finds the number of non-null characters in s.
*
*	int strlen(s)
*	char *s;
**********************************************************************/

#include <string.h>

size_t strlen(P(const char *) str)
PP(register const char *str;)
{
	register const char *p;

	p = str;
	while (*p++ != '\0')				/* advance *p until NULL.   */
		;
	--p;
	return (long)p - (long)str;
}
