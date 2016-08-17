#include <string.h>
#include <portab.h>
#include <ctype.h>


char *strupr(P(char *) str)
PP(char *str;)
{
	register char *p = str;
	do {
		*p = toupper((UBYTE) *p);
	} while (*p++ != 0);
	return str;
}
