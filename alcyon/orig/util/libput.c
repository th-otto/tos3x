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
#include "util.h"

#define doputc(byte,fp) putc(byte,fp)

struct mlbytes { char b1; char b2; char b3; char b4; };
struct mwbytes { char wb1; char wb2; };

int lputl(P(long *) llp, P(FILE *) f)			/* returns 0 for success, -1 for failure */
PP(long *llp;)			/* 32 bits */
PP(FILE *f;)
{
#ifndef __ALCYON__
	struct mlbytes *lp = (struct mlbytes *)llp;
#else
#define lp llp
#endif
	doputc(lp->b1, f);
	doputc(lp->b2, f);
	doputc(lp->b3, f);
	doputc(lp->b4, f);
	return ferror(f) ? -1 : 0;
}

int lputw(P(unsigned short *) llp, P(FILE *) f)
PP(unsigned short *llp;)
PP(FILE *f;)
{
#ifndef __ALCYON__
	struct mwbytes *lp = (struct mwbytes *)llp;
#else
#define lp llp
#endif
	doputc(lp->wb1, f);
	doputc(lp->wb2, f);
	return ferror(f) ? -1 : 0;
}
