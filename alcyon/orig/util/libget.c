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

#define dogetc(byte,i,fp) byte = getc(fp)

struct mlbytes { char b1; char b2; char b3; char b4; };
struct mwbytes { char wb1; char wb2; };

int lgetl(P(long *) llp, P(FILE *) f)
PP(struct mlbytes *llp;)		/* 32 bits */
PP(FILE *f;)
{
#ifndef __ALCYON__
	struct mlbytes *lp = (struct mlbytes *)llp;
#else
#define lp llp
#endif
	register int i;

	dogetc(lp->b1, i, f);
	dogetc(lp->b2, i, f);
	dogetc(lp->b3, i, f);
	dogetc(lp->b4, i, f);
	UNUSED(i);
	return ferror(f) ? -1 : 0;
}

int lgetw(P(unsigned short *) llp, P(FILE *) f)
PP(struct mwbytes *llp;)
PP(FILE *f;)
{
#ifndef __ALCYON__
	struct mwbytes *lp = (struct mwbytes *)llp;
#else
#define lp llp
#endif
	register int i;

	dogetc(lp->wb1, i, f);
	dogetc(lp->wb2, i, f);
#if 0
	*llp &= 0xffff;
#endif
	UNUSED(i);
	return ferror(f) ? -1 : 0;
}
