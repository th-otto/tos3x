/************************************************************************
* _errmalloc / MALLOCDB.C : prints error messages for malloc()
*	To be included in OPTION?.lib
*
*	10/83   whf
************************************************************************/

#include "lib.h"
#include <stdlib.h>

extern FB_HDR _afreebase;				/* initial (empty) block    */
extern FB_HDR *_aflistptr;				/* ptr into ring of freeblks */


VOID _errmalloc(P(int) etype)
PP(int etype;)								/* type of error        */
{
	switch (etype)
	{
	case ERR_FINDBLOCK:
		printf("malloc() error: corrupt arena\n");
		break;
	case ERR_GETMEM:
		printf("malloc() error: out of memory\n");
		break;
	case ERR_FREE:
		printf("free() error: pointer was not from malloc()\n");
		break;
	}
	exit(1);
}


/****************************************************************************/
/* malloc_debug(): checks the free list to see if everything is ok.	    */
/*************************************************			    */
int malloc_debug(NOTHING)
{
	register FB_HDR *cp;						/* ptr to current block     */

	cp = _aflistptr;					/* start search here        */
	for (;;)
	{
		/* does cp pt to valid blk? */
		if (~(cp->size ^ cp->chksize))
		{
			_errmalloc(ERR_FINDBLOCK);
			return -1;
		}
		/* move on down the list */
		cp = cp->ptr;
		if (cp == _aflistptr)
			return 0;
	}
}
