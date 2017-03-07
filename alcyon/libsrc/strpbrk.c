#include "lib.h"
#include <string.h>

/*
 *	ptr = pointer to the first occurrence in s1 of any character
 *	      from s2.  ptr is NULL if no character from s2 exists in s1.
 */

char *strpbrk(P(const char *) s1, P(const char *) s2)
PP(const char *s1;)
PP(const char *s2;)
{
	register const char *ts1;					/* temp s1 */
	register const char *ts2;					/* temp s2 */

	/* compare each character in s1 to all characters in s2 */
	/* if match is found return pointer to position in s1 */
	for (ts1 = s1; *ts1; ts1++)
		for (ts2 = s2; *ts2; ts2++)
			if (*ts2 == *ts1)
				return NO_CONST(ts1);

	return NULL;
}
