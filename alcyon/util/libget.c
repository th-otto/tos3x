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
#include "util.h"

int lgetw(P(unsigned short *) lp, P(FILE *) f)
PP(unsigned short *lp;)
PP(FILE *f;)
{
	unsigned int c1 = getc(f);
	unsigned int c2 = getc(f);
	*lp = (c1 << 8) | c2;
	return feof(f) ? EOF : 0;
}

int lgetl(P(long *) lp, P(FILE *) f)
PP(register long *lp;)		/* 32 bits */
PP(FILE *f;)
{
	unsigned short w1, w2;
	lgetw(&w1, f);
	lgetw(&w2, f);
	*lp = ((long)w1 << 16) | w2;
	return feof(f) ? EOF : 0;
}
