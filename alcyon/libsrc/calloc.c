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

VOIDPTR calloc(P(size_t) nelem, P(size_t) sizelem)
PP(size_t nelem;)								/* number of elements */
PP(size_t sizelem;)							/* size of element */
{
	char *rp;						/* pointer to region */

	nelem *= sizelem;
	if ((rp = malloc(nelem)) != NULL)
		memset(rp, 0, nelem);
	return rp;
}
