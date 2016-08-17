/**************************************************************************
*
*			g e t l   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"getl" gets an LONG (4 byte) value from the input stream.
*	Note that what's written (binary) by a PDP 11 (UNIX: lo byte, hi byte)
*	system will NOT be compatible with what's written by a
*	68K (NUXI: hi byte, lo byte) system.
*
*	Calling sequence:
*		l = getl(stream)
*	Where:
*		l     = a 32-bit long value
*		stream-> a (FILE *) input stream
*
*****************************************************************************/

#include "lib.h"

long getl(P(FILE *) sp)
PP(register FILE *sp;)					/* the stream to get from   */
{
	long l;									/* place to get to      */
	register char *p;						/* make ptr to l        */

	p = (char *) &l;
	*p++ = getc(sp);
	*p++ = getc(sp);
	*p++ = getc(sp);
	*p++ = getc(sp);
	return l;
}
