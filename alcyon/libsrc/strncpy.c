/**********************************************************************
*   STRNCPY  -  copies at most n chars from one string to another
*
*	char *strcpy(s1,s2,n)
*	char *s1, *s2;
*	UWORD n;
*
*	Copies at most n bytes from s2 to s1, stopping after null 
*	  has been moved.
*	Truncates or null-pads s2, depending on n.
*	Returns s1.
*	No check for overflow of s1.
***********************************************************************/

#include <string.h>

char *strncpy(P(char *) s1, P(const char *) s2, P(size_t) num)
PP(char *s1;)
PP(register char *s2;)
PP(size_t num;)
{
	register char *cp;

	for (cp = s1; num-- > 0 && (*cp = *s2) != '\0'; cp++, s2++)	/* isn't C fun?   */
		;
	num++;								/* bump n back up.      */
	while (num--)						/* while #chars != 0        */
		*cp++ = '\0';					/*   null pad.          */
	return s1;
}
