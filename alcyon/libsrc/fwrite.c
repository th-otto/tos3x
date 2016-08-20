/**************************************************************************
*
*			f w r i t e   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fwrite" writes NI items of size SZ from memory at BUFF into stream
*	SP.
*
*	Calling sequence:
*		nitems = fwrite(buff,size,nitems,stream)
*	Where:
*		buff -> where to write from
*		size = number bytes in each item
*		nitems = number bytes written/to write
*		stream -> file
*
*	nitems is set to 0 if an error occurs (including EOF).
*
*****************************************************************************/

#include "lib.h"

size_t fwrite(P(const VOIDPTR) pbuff, P(size_t) size, P(size_t) nmemb, P(FILE *) sp)
PP(VOIDPTR pbuff;)
PP(size_t size;)
PP(size_t nmemb;)
PP(register FILE *sp;)
{
	register const char *buff = pbuff;
	register long j, k;

	k = (long)size * (long)nmemb;
	for (j = 0; j < k; j++)
		if (fputc(*buff++, sp) < 0)	/* used for side effects */
			return 0;
	return nmemb;
}
