#include "lib.h"
#include <string.h>

/* already in startup code */

VOID blkfill(P(char *) dest, P(char) val, P(int) size)
PP(register char *dest;)
PP(register char val;)
PP(register int size;)
{
	while (size-- > 0)
		*dest++ = val;
}
