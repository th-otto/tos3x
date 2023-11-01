/************************************************************************
*
*			r e w i n d   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"rewind" sets the rdad/write pointer of a stream file to the
*	beginning of the stream.
*
*	Calling sequence:
*		ret = rewind(sp)
*	Where:
*		sp -> a stream file (FILE *)
*		ret = 0 for success, -1 for failure
*
*************************************************************************/

#include "lib.h"

int rewind(P(FILE *) sp)
PP(FILE *sp;)
{
	return fseek(sp, 0L, SEEK_SET);
}
