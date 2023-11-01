#include "lib.h"
#include "klib.h"

/**
 **	put a single character
 **/

int	f_log = 0;

VOID cputc(P(char) c, P(int) fn)
PP(char c;)
PP(int fn;)
{
	write(fn, &c, 1);
}
