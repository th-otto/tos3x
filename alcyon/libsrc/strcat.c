/****************************************************************************
*    STRCAT  -  concatenate strings
*
*	char *strcat(s1,s2)	copies s2 to end of s1
*	char *s1, *s2;
*
*	Assumes null terminated strings. No check is made for string area
*		overflow.
****************************************************************************/

#include <string.h>

char *strcat(P(char *) s1, P(const char *) s2)
PP(char *s1;)
PP(register char *s2;)
{
	register char *cp;

	for (cp = s1; *cp; cp++)			/* save s1 for return.      */
		;
	while ((*cp++ = *s2++) != '\0')		/* copy until eos(s2).      */
		;
	return s1;
}
