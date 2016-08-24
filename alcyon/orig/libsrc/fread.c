/**************************************************************************
*
*			f r e a d   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fread" reads NI items of size SZ from stream SP and deposits them
*	at BUFF.
*
*	Calling sequence:
*		nitems = fread(buff,size,nitems,stream)
*	Where:
*		buff -> where to read to
*		size = number bytes in each item
*		nitems = number bytes read/to read
*		stream -> file
*
*	nitems is set to 0 if an error occurs (including EOF).
*
*****************************************************************************/

#include "lib.h"

size_t fread(P(VOIDPTR) pbuff, P(size_t) size, P(size_t) nmemb, P(FILE *) sp)
PP(register VOIDPTR pbuff;)
PP(size_t size;)
PP(size_t nmemb;)
PP(register FILE *sp;)
{
#ifdef __ALCYON__
#define buff pbuff
#else
	register char *buff = pbuff;
#endif
	register size_t jj, kk;
	register int ch;

	for (jj = 0; (int)jj < (int)nmemb; jj++)
		for (kk = 0; (int)kk < (int)size; kk++)
		{
			if ((ch = getc(sp)) == EOF)
				return (int)jj;
			else
				*buff++ = ch;
		}
	return (int)nmemb;
#undef buff
}
