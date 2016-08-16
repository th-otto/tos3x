/**********************************************************************
*   STRCPY  -  copies from one string to another
*
*	char *strcpy(s1,s2)
*	char *s1, *s2;
*
*	Copies bytes from s2 to s1, stopping after null has been moved.
*	Returns s1.
*	No check for overflow of s1.
***********************************************************************/

#include <string.h>

char *strcpy(P(char *) s1, P(const char *) s2)
PP(char *s1;)
PP(register char *s2;)
{
	register char *cp;

	cp = s1;							/* save for return.     */
	while ((*cp++ = *s2++) != '\0')
		;
	return s1;
}
