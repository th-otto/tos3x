/*********************************************************************
*   INDEX  -  returns a pointer to first occurrence of char in string.
*
*	char *index(s,c)
*	char *s, c;
*
*	Returns pointer to first c in s, or zero if c not in s.
**********************************************************************/

#include "lib.h"
#include <string.h>

#undef index
#undef strchr

char *index(P(const char *) str, P(int) ch)
PP(register const char *str;)
PP(register char ch;)
{
	for (; ch != *str; str++)			/* look for c in s.     */
		if (*str == '\0')				/* if we get to eos, we've gone */
			return (0);					/*   too far.           */
	return NO_CONST(str);						/* found c. note that 'index'   */
}
