/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) libget.c - Sep 12, 1983  REGULUS 4.1
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */

#include <stdio.h>
#include <cout.h>
#include "../util/util.h"

int lgetw(P(unsigned short *) lp, P(FILE *) f)
PP(unsigned short *lp;)
PP(FILE *f;)
{
	register unsigned int w1;
	w1 = getc(f);
	w1 <<= 8;
	*lp = w1 | getc(f);
	return feof(f) ? EOF : 0;
}

int lgetl(P(int32_t *) lp, P(FILE *) f)
PP(register int32_t *lp;)		/* 32 bits */
PP(FILE *f;)
{
	unsigned short w1, w2;
	lgetw(&w1, f);
	lgetw(&w2, f);
	*lp = ((int32_t)w1 << 16) | w2;
	return feof(f) ? EOF : 0;
}
