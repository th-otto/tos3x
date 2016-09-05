/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) libput.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include <stdio.h>
#include <cout.h>
#include "../libsrc/klib.h"

long xputl(P(long) l, P(FILE *) f)			/* returns 0 for success, -1 for failure */
PP(register long l;)			/* 32 bits */
PP(register FILE *f;)
{
	putc((int)((l >> 24) & 0xff), f);
	putc((int)((l >> 16) & 0xff), f);
	putc((int)((l >>  8) & 0xff), f);
	putc((int)(l) & 0xff, f);
#ifdef __ALCYON__
	asm("move.l d7,d0");
	asm("nop");
#else
	return l;
#endif
}

long xputlp(P(long *) lp, P(FILE *) f)			/* returns 0 for success, -1 for failure */
PP(long *lp;)			/* 32 bits */
PP(FILE *f;)
{
#ifdef __ALCYON__
	xputl(*lp, f);
	asm("nop");
#else
	return xputl(*lp, f);
#endif
}
