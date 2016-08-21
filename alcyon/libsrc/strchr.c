/*********************************************************************
*   STRCHR -  returns a pointer to first occurrence of char in string.
*	(formerly known as "index()")
*
*	char *strchr(s,c)
*	char *s, c;
*
*	Returns pointer to first c in s, or zero if c not in s.
*
*	Edits:
*	1/84 whf	changed from 'index()' to 'strchr()'
**********************************************************************/

#include "lib.h"
#include <string.h>

#if 0 /* not used; optimized assembler version */

char *strchr(P(const char *) str, P(int) ch)
PP(register const char *str;)
PP(register char ch;)
{
	for (; ch != *str; str++)			/* look for c in s.     */
		if (*str == '\0')				/* if we get to eos, we've gone */
			return NULL;					/*   too far.           */
	return NO_CONST(str);
}

#endif
