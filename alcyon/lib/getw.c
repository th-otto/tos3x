/**************************************************************************
*
*			g e t w   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"getw" gets an WORD (2 byte) value from the input stream.
*	Note that what's written (binary) by a PDP 11 (UNIX: lo byte, hi byte)
*	system will NOT be compatible with what's written by a
*	68K (NUXI: hi byte, lo byte) system.
*
*	Calling sequence:
*		w = getw(stream)
*	Where:
*		w     = a 16-bit word value
*		stream-> a (FILE *) input stream
*
*****************************************************************************/

#include "lib.h"

int getw(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	int w;								/* place to get to      */
	register char *p;					/* make ptr to w        */

	p = (char *) &w;
	*p++ = getc(sp);
	*p++ = getc(sp);
	return w;
}
