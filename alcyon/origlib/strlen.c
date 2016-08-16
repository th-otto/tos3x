/**********************************************************************
*    STRLEN  -  finds the number of non-null characters in s.
*
*	int strlen(s)
*	char *s;
**********************************************************************/

#include <string.h>

int strlen(P(const char *) str)
PP(register char *str;)
{
	register const char *p;

	for (p = str; *p; p++)				/* advance *p until NULL.   */
		;
	return (int) p - (long)str;	/* BUG: wrong cast */
}
