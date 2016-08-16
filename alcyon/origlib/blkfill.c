/***************************************************************************
*									    
*			b l k f i l l   F u n c t i o n			    
*			-------------------------------			    
*	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
*									    
*	The blkfill function sets a region of memory to a given value.	    
*									    
*	Calling Sequence:						    
*									    
*		blkfill(addr,fc,num);					    
*									    
*	Where:								    
*		addr	Is a pointer to region of memory to blkfill	    
*		fc	Is a character to blkfill with			    
*		num	Is the number of chars to blkfill with		    
*									    
****************************************************************************/

#include "lib.h"

#if 0 /* not used; optimized version in startup code */

VOID blkfill(P(VOIDPTR) addr, P(int) fc, P(size_t) num)
PP(VOIDPTR addr;)
PP(register int fc;)
PP(register size_t num;)
{
	register char *ptr = addr;
	
	if (num > 0)
	{
		do
		{
			*ptr++ = fc;
		} while (--num);
	}
}

#endif
