/*
 *	El-Kludg-o Dup routine.  Takes advantage of the fact that
 *	stdout is not closed by Bill Allen's stuff.
 */

#include "lib.h"
#include <stdlib.h>
#include "klib.h"

int xdup(P(int) n)
PP(register int n;)
{
#ifndef __ALCYON__
	return n;
#else
	asm("move.w d7,d0");
	asm("nop");
#endif
}
