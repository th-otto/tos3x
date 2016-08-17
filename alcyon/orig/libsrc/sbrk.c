/***************************************************************************
 *									    
 *			     s b r k   F u n c t i o n			    
 *			     -------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.       
 *									    
 *	The "sbrk" function is used to allocate memory dynamically.	    
 *									    
 *	Calling Sequence:						    
 *									    
 *		addr = sbrk(incr);					    
 *									    
 *	Where:								    
 *		incr	Is the incremental number of bytes to be added to   
 *			the program heap area.				    
 *									    
 *		addr	Is the beginning address of the allocated area.	    
 *			-1 is returned if allocation failed		    
 *									    
 *	Last modified:
 *	1/84 whf	refresh include files for 68K (sbrk in asm for 8086)
 ****************************************************************************/
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <stdlib.h>


VOIDPTR sbrk(P(int) incr)
PP(int incr;)								/* Incremental storage      */
{
	register char *t1;
	register char *t2;
	register ptrdiff_t inc;						/* Temp size increment      */

	/* Un sign-extend */
	inc = ((ptrdiff_t) incr) & 0xffffL;
	/* Disallow odd incr's */
	if (inc & 1)
		inc++;
	
	/* Save the old break */
	t1 = _break;
	/* New break value */
	t2 = _break + inc;
	
	/* Allocate */
	if (brk(t2) == -1)
		RETERR((VOIDPTR)-1, ENOMEM);
	
	blkfill(t1, 0, incr); /* BUG: incr can be negative */
	
	return t1;
}
