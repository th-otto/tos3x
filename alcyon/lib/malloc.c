/********************************************************************
*	malloc, free - memory allocator mechanism
*
*	9/83: fixed free bug, added check word   whf
*	10/83: added debugging features: DEBUGMEM and MALLOCND hooks   whf
*
* char *malloc(numbytes) 
*		int numbytes
*	Returns a pointer to an area of memory at least 'numbytes'
*	in length.  Returns NULLPTR if out of memory or corrupted freelist.
*	Warning: the size of the area is right below the region: 
*	do not overwrite!
*
*
* int free(mptr)
*		char *mptr
*	Frees the region pointed to by 'mptr'.  Returns 0 (SUCCESS) if ok,
*	FAILURE (-1) if corrupted freelist (or if mptr was not previously
*	allocated by malloc).
*
*
* char *realloc(mptr,numbytes)
*		char *mptr
*		int numbytes
*	Returns a pointer to a (probably different) region of memory
*	(numbytes long) containing the same data as the old region 
*	pointed to by mptr. Returns NULLPTR if not enough memory, or
*	if freelist was corrupted.
*
************************************************************************/

#include "lib.h"
#include <stdlib.h>

/************************* #include "malloc.h"  *****************************/
#define FB_HDR struct hdr				/* free block header type   */
#define NULLFBH ((FB_HDR *)0)			/* Null of above struct     */

FB_HDR
{										/* mem_block header     */
	struct hdr *ptr;					/* ptr next blk (when freed) */
	size_t size;						/* block size (always)      */
	size_t chksize;						/* 1's complement of size   */
};


static FB_HDR _afreebase = { &_afreebase, 0, ~0 };	/* initial (empty) block    */

static FB_HDR *_aflistptr = &_afreebase;		/* ptr into ring of freeblks */

#define AOFFS 1L						/* alignment offset: 0=byte, 1=word, 3=quad */
#define AMASK(c) ((char *)((long)(c) & ~AOFFS))	/* mask alignment bits      */
#define AFUDGE 4						/* leeway for passing block as is */
#define ACHUNKS 64						/* chunks to alloc from O.S. */

/*** end of "malloc.h" ******/

/****************************************************************************/
/* getmemory - gets memory from O.S. 					    */
/* This function requests memory from the O.S. in multiples (ACHUNKS)	    */
/*	of the requested number of units (numu), in order to minimize	    */
/*	the number of calls to the O.S.					    */
/*	Function cleans up pointer alignment and adds to free list.	    */
/*************************************************			    */
static FB_HDR *getmemory(P(size_t) numu)
PP(size_t numu;)								/* number of units */
{
	register char *mmp;						/* more memory pointer */
	register FB_HDR *fbp;					/* free block pointer */
	register int utg;						/* units to get */

	utg = ((numu + (ACHUNKS - 1)) / ACHUNKS) * ACHUNKS;

	mmp = sbrk(utg * sizeof(FB_HDR));	/* sbrk wants number of bytes */

	if (mmp == (char *) -1)				/* sbrk = -1 means no memory available */
		return NULLFBH;
	mmp = AMASK(mmp + AOFFS);			/* alignment handling: nop if good sbrk */
	fbp = (FB_HDR *) mmp;
	fbp->size = utg;
	fbp->chksize = ~fbp->size;
	free((char *) (fbp + 1));			/* add to free list */
	return _aflistptr;					/* set in 'free' */
}


/****************************************************************************/
/* findblock - local subr to find a free block that's big enough.	    */
/*	It returns a pointer to the freeblock BEFORE the desired freeblock, */
/*	in order to fix the pointer of the this freeblock.		    */
/*************************************************			    */
static FB_HDR *findblock(P(size_t) units)
PP(size_t units;)
{
	register FB_HDR *cp;						/* ptr to current block     */
	register FB_HDR *pp;						/* ptr to previous block    */

	pp = _aflistptr;					/* start search here        */
	cp = pp->ptr;						/* init current ptr     */
	for (;;)
	{
		if (~(cp->size ^ cp->chksize))	/* does cp pt to valid blk? */
		{
			_errmalloc(ERR_FINDBLOCK);	/* handle this error        */
			return NULLFBH;
		}
		if (cp->size >= units)			/* is this block big enough? */
			return pp;					/* yes! NOTE: return prevptr */
		if (cp == _aflistptr)			/* end of list?         */
			if ((cp = getmemory(units)) == NULLFBH)	/* is there more?    */
			{
				_errmalloc(ERR_GETMEM);
				return NULLFBH;			/*  no more memory...   */
			}
		pp = cp;
		cp = cp->ptr;					/* move on down the list    */
	}
}


/****************************************************************************/
/* cutup - a local fn to cut up the free block (if its much bigger than the */
/*	number of units requested), and to convert blk ptr to byte ptr.     */
/*************************************************			    */
static char *cutup(P(FB_HDR *) pp, P(size_t) units)
PP(FB_HDR *pp;)								/* ptr to ptr to block      */
PP(size_t units;)							/* num units to cut to      */
{
	register FB_HDR *cp;						/* cur ptr          */
	register FB_HDR *np;						/* new ptr (if needed)      */

	cp = pp->ptr;						/* get ptr to big block     */
	if (cp->size <= units + AFUDGE)		/* is size close enough?    */
		pp->ptr = cp->ptr;				/*   yes: cut cp from list  */
	else
	{									/* o.w. cut block up        */
		np = cp + units;				/* where it starts      */
		np->size = cp->size - units;	/* how big it is        */
		np->chksize = ~np->size;		/* it's a valid blk     */
		np->ptr = cp->ptr;				/* it's linked into freelist */
		pp->ptr = np;					/* from both sides      */
		cp->size = units;				/* new size for cur block   */
		cp->chksize = ~cp->size;		/* and make valid       */
	}
	_aflistptr = pp;					/* search from here next tim */
	return (char *)(cp + 1);			/* point to after header    */
}


/****************************************************************************/
/* malloc - general purpose memory allocator
* This function allocates the requested number of bytes (nbytes) and returns
*	a pointer to this space to the calling function.
*	The memory is requested from the O.S. in larger chunks, and
*	free space is managed as a ring of free blocks.  Each block
*	contains a pointer to the next block (s.ptr), a block size (s.size),
*	a check word (1's complement of size), and the space itself.
*	Alignment is handled by assuming that sizeof(FB_HDR) is an aligned
*	quantity, and allocating in multiples of that size.
**************************************************			    */
VOIDPTR malloc(P(size_t) nbytes)
PP(size_t nbytes;)
{
	register size_t nmults;					/* multiples of FB_HDR size */
	register FB_HDR *pp;					/* temporary ptr        */

	if (nbytes == 0)
		return NULL;
	nmults = (nbytes + sizeof(FB_HDR) - 1) / sizeof(FB_HDR) + 1;
	if ((pp = findblock(nmults)) == NULLFBH)	/* find ptr to ptr to block */
		return NULL;					/*  no luck...      */
	return cutup(pp, nmults);			/* cut block to fit & return */
}


/****************************************************************************/
/* free - adds memory back to free block list.
* This function assumes freed memory pointer (fmp) points to free space
*	preceeded by a FB_HDR structure.
*	Note that free block is inserted in memory address order,
*	to facilitate compaction.
*	Fails (-1 return) if you link in a random ptr.
*************************************************			    */
VOID free(P(VOIDPTR) fmp)
PP(VOIDPTR fmp;)
{
	register FB_HDR *cp;				/* current freeblk ptr      */
	register FB_HDR *pp;				/* previous freeblk ptr     */

	if (fmp == NULL)
		return;
	/* ptr to (hdr of) freed blk */
	cp = (FB_HDR *) fmp - 1;
	/* is blk valid? */
	if (~(cp->size ^ cp->chksize))
	{
		_errmalloc(ERR_FREE);
		return;
	}
	/* start through the list   */
	for (pp = _aflistptr;; pp = pp->ptr)
	{
		/* are we at correct place? */
		if (cp >= pp && cp < pp->ptr)
			break;
		/* at end of list? and blk is off either end? */
		if (pp >= pp->ptr && (cp <= pp->ptr || cp >= pp))
			break;
	}
	if (cp >= pp && cp + cp->size <= pp + pp->size)	/* block already freed? */
	{
		/* search from here next */
		_aflistptr = pp;
		return;
	}

	/*
	 * Insert freed block back into list.  Try to coalesce (merge) adjacent
	 * regions.
	 */
	if (cp + cp->size == pp->ptr)		/* end freed blk==start nxt? */
	{									/*  then merge with nxt */
		cp->size += pp->ptr->size;
		cp->chksize = ~cp->size;
		cp->ptr = pp->ptr->ptr;
	} else
	{
		/* else just point to nxt */
		cp->ptr = pp->ptr;
	}
	
	if (pp + pp->size == cp)			/* end prev plk==start cur? */
	{
		/* then merge with prev */
		pp->size += cp->size;
		pp->chksize = ~pp->size;
		pp->ptr = cp->ptr;
	} else
	{
		/* else point prev to cur */
		pp->ptr = cp;
	}
	
	/* search from here next time */
	_aflistptr = pp;
}


/****************************************************************************/
/* realloc - free memory, allocate again (with changed size maybe),	    */
/*		preserve contents.					    */
/*************************************************			    */
VOIDPTR realloc(P(VOIDPTR) _ptr, P(size_t) siz)
PP(VOIDPTR _ptr;)
PP(register size_t siz;)
{
	register char *ptr = _ptr;
	register char *np;						/* ptr to new allocation    */
	register size_t nmults;					/* multiples if FB_HDR size */
	FB_HDR *pp;
	
	/* stuff back into free list: any coalesce will not */
	free(ptr); /* WTF; bad idea */
	if (siz == NULL)
		return NULL;
	/* affect original data region */
	nmults = (siz + sizeof(FB_HDR) - 1) / sizeof(FB_HDR) + 1;
	/* find ptr to ptr to block */
	if ((pp = findblock(nmults)) == NULLFBH)
		return NULL;
	/* convert to char ptr */
	np = (char *)(pp->ptr + 1);
	if (ptr != NULL && ptr != np && siz != 0)
	{
		/* if ptr changed */
		if (np < (char *)ptr)
		{
			/* if new ptr in lower mem, copy up */
			/* BUG: should copy old size only */
			do
			{
				*np++ = *ptr++;
			} while (--siz);
		} else
		{
			/* if new ptr in higher mem copy down */
			/* BUG: should copy old size only */
			np += siz;
			ptr += siz;
			do
			{
				*--np = *--ptr;
			} while (--siz);
		}
	}
	/* cut block to fit & return */
	return cutup(pp, nmults);
}
