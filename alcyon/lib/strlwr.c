#include <string.h>
#include <portab.h>
#include <ctype.h>


char *strlwr(P(char *) str)
PP(char *str;)
{
	register char *p = str;
	do {
		*p = tolower((UBYTE) *p);
	} while (*p++ != 0);
	return str;
}

