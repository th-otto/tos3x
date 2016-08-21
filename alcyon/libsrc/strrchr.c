/*********************************************************************
*   STRRCHAR -  returns a pointer to last occurrence of char in string.
*
*	char *strrchr(s,c)
*	char *s, c;
*
*	Like 'strchr', only returns pointer to last c in s (instead of first),
*	  or zero if c not in s.
*
*	Edits:
*	1/84 whf	change from 'rindex()' to 'strchr()'
**********************************************************************/

#include "lib.h"
#include <string.h>

#undef rindex
#undef strrchr

char *strrchr(P(const char *) str, P(int) ch)
PP(register const char *str;)
PP(register char ch;)
{
	register const char *t;

	for (t = str; *t; t++)				/* look for eos.        */
		;
	for (; ch != *t; t--)				/* look for c in s.     */
		if (t == str)					/* if we get to start of string, */
			return NULL;				/*   too far.           */
	return NO_CONST(t);							/* found c. note that 'rindex'  */
}
