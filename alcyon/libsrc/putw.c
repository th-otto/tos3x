/**************************************************************************
*
*			p u t w   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"putw" puts a word (2 byte) value into the output stream.
*	Note that what's written (binary) by a UNIX (lo byte, hi byte)
*	system (PDP 11) will NOT be compatible with what's written by a
*	NUXI (hi byte, lo byte) system (68K).
*
*	Calling sequence:
*		ret = putw(w, stream)
*	Where:
*		ret   = w or FAILURE on error
*		w     = an int word value
*		stream-> a (FILE *) output stream
*
******************************************************************************/

#include "lib.h"

int putw(P(int) wrd, P(FILE *) sp)
PP(int wrd;)								/* the word to be put       */
PP(FILE *sp;)								/* the stream to put to     */
{
	if (putc(wrd >> 8, sp) < 0)
		return EOF;
	if (putc(wrd, sp) < 0)
		return EOF;
	return 0;
}
