/*********************************************************************
*   RINDEX  -  returns a pointer to last occurrence of char in string.
*
*	char *rindex(s,c)
*	char *s, c;
*
*	Like 'index', only returns pointer to last c in s (instead of first),
*	  or zero if c not in s.
**********************************************************************/

#include "lib.h"
#include <string.h>

#undef rindex
#undef strrchr

char *rindex(P(const char *) s, P(int) c)
PP(const char *s;)
PP(char c;)
{
	register const char *t;

	for (t = s; *t; t++)				/* look for eos.        */
		;
	for (; c != *t; t--)				/* look for c in s.     */
		if (t == s)						/* if we get to start of string, */
			return 0;					/*   too far.           */
	return NO_CONST(t);							/* found c. note that 'rindex'  */
}
