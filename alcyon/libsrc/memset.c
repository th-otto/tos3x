/***************************************************************************
*									    
*			b l k f i l l   F u n c t i o n			    
*			-------------------------------			    
*	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
*									    
*	The memset function sets a region of memory to a given value.	    
*									    
*	Calling Sequence:						    
*									    
*		memset(addr,fc,num);					    
*									    
*	Where:								    
*		addr	Is a pointer to region of memory to fill	    
*		fc	Is a character to fill with			    
*		num	Is the number of chars to fill with		    
*									    
****************************************************************************/

#include "lib.h"

#if 0 /* not used; optimized version in startup code */

VOIDPTR memset(P(VOIDPTR) addr, P(int) fc, P(size_t) num)
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
	return addr;
}

#endif
