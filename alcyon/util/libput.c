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
#include "../util/util.h"

int lputl(P(int32_t *) lp, P(FILE *) f)			/* returns 0 for success, -1 for failure */
PP(register int32_t *lp;)
PP(register FILE *f;)
{
	register long l;
	
	l = *lp;
	putc((int)((l >> 24) & 0xff), f);
	putc((int)((l >> 16) & 0xff), f);
	putc((int)((l >>  8) & 0xff), f);
	putc((int)(l) & 0xff, f);
	return ferror(f) ? EOF : 0;
}

int lputw(P(unsigned short *) wp, P(FILE *) f)			/* returns 0 for success, -1 for failure */
PP(register unsigned short *wp;)
PP(register FILE *f;)
{
	register unsigned short w;
	
	w = *wp;
	putc((int)((w >>  8) & 0xff), f);
	putc((int)(w) & 0xff, f);
	return ferror(f) ? EOF : 0;
}
