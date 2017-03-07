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


/*
 * getarhd - fills the archive header structure from the buffer in
 *		the manner which will be understood on the current machine.
 */
int getarhd(P(FILE *) fp, P(struct libhdr *) arptr)
PP(register FILE *fp;)
PP(struct libhdr *arptr;)
{
	register int i;
	char *lp;
	int c;
	
	for (i = 0, lp = arptr->lfname; i < LIBNSIZE; i++)
	{
		if ((c = getc(fp)) == EOF)
			return EOF;
		lp[i] = c;
	}
	if (lgetl(&(arptr->lmodti), fp) != 0)
		return EOF;
	if ((c = getc(fp)) == EOF)
		return EOF;
	arptr->luserid = c;
	if ((c = getc(fp)) == EOF)
		return EOF;
	arptr->lgid = c;
	if (lgetw(&arptr->lfimode, fp) != 0)
		return EOF;
	if (lgetl(&arptr->lfsize, fp) != 0)
		return EOF;
	if (lgetw(&arptr->junk, fp) != 0)
		return EOF;
	return 0;
}
