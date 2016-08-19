#include "lib.h"

extern long ldivr;

long lrem(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
#ifdef __ALCYON__
	asm("  xdef lrem");
	asm("lrem equ _lrem");
#endif
	ldiv(al1, al2);
	return ldivr;
}
