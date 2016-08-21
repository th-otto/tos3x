/********************************************************************
*
*	calloc.c - memory allocator for sets of elements
*	zalloc	 - memory allocator like malloc only zeros storage.
*
*	char *calloc(nelem,sizelem)
*		int nelem, sizelem;
*
*	Returns a pointer to a region of (zero filled) memory large
*	enough to hold 'nelem' items each of size 'sizelem'.
*	Returns NULL if not enough memory.
*
*	char	*zalloc(nbytes)
*		UWORD	nbytes;
*
*	Returns a pointer to a region of zero filled memory nbytes long.
*	Returns NULL if not enough memory.
*
*********************************************************************/

#include "lib.h"
#include <string.h>
#include <stdlib.h>

VOIDPTR zalloc PROTO((size_t nbytes));

VOIDPTR zalloc(P(size_t) nbytes)
PP(size_t nbytes;)							/* number of bytes */
{
	register char *rp;						/* pointer to region */

	if ((rp = malloc(nbytes)) == NULL)
		return NULL;
	memset(rp, 0, nbytes);
	return rp;
}

VOIDPTR calloc(P(size_t) nelem, P(size_t) sizelem)
PP(size_t nelem;)								/* number of elements */
PP(size_t sizelem;)							/* size of element */
{
	return zalloc(sizelem * nelem);
}
