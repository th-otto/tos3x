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
PP(register VOIDPTR pbuff;)
PP(size_t size;)
PP(size_t nmemb;)
PP(register FILE *sp;)
{
#ifdef __ALCYON__
#define buff pbuff
#else
	register const char *buff = pbuff;
#endif
	register size_t jj,  kk;

	for (jj = 0; (int)jj < (int)nmemb; jj++)
		for (kk = 0; (int)kk < (int)size; kk++)
			if (fputc(*buff++, sp) == EOF)	/* used for side effects */
				return 0; /* BUG: should be return jj */
	return (int)nmemb;
#undef buff
}
