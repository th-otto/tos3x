/****************************************************************************
*  STRNCAT  -  concatenate strings (limited)
*

*	char *strncat(s1,s2,n)
*	char *s1, *s2; int n;
*
*	'strncat' copies at most n bytes of s2 onto the end of s1.
*
*	Assumes null terminated strings. No check is made for string area
*		overflow.
****************************************************************************/

#include <string.h>

char *strncat(P(char *) s1, P(const char *) s2, P(size_t) num)
PP(char *s1;)
PP(register const char *s2;)
PP(size_t num;)
{
	register char *cp;

	for (cp = s1; *cp; cp++)			/* save s1 for return.      */
		;
	while (*s2 && num-- > 0)			/* copy until eos(s2) or n==0:  */
		*cp++ = *s2++;					/*  no copy if n==0.        */
	*cp = '\0';							/* make sure string gets eos.   */
	return s1;
}
