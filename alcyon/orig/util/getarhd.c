/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

/*
	I/O independent mapping routine.  Machine specific.  Independent
	of structure padding.  Buffer must contain at least as many
	characters as is required for structure.
 */


#include <stdio.h>
#include <ar68.h>
#include "util.h"

#ifdef __GNUC__
 #pragma GCC diagnostic ignored "-Warray-bounds"
#endif

/*
 * getarhd - fills the archive header structure from the buffer in
 *		the manner which will be understood on the current machine.
 */
int getarhd(P(FILE *) fp, P(struct libhdr *) arptr)
PP(FILE *fp;)
PP(struct libhdr *arptr;)
{
	register int i;
	int junk;
	register char *p, *lp;

	for (i = 0, lp = arptr->lfname; i < LIBNSIZE; i++)
		if ((*lp++ = getc(fp)) == EOF)
			return EOF;
	if (lgetl(&arptr->lmodti, fp) == EOF)
		return EOF;
	if ((arptr->luserid = getc(fp)) == EOF)
		return EOF;
	if ((arptr->lgid = getc(fp)) == EOF)
		return EOF;
	if (lgetw(&arptr->lfimode, fp) == EOF)
		return EOF;
	if (lgetl(&arptr->lfsize, fp) == EOF)
		return EOF;
	if (lgetw(&junk, fp) == EOF)
		return EOF;
	return 0;
}
