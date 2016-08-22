/***************************************************************************
 *									    
 *			_ l s t o u t   F u n c t i o n			    
 *			-------------------------------			    
 *									    
 *	Function "_lstout" is called to perform "write" operations to the   
 *	list device.							    
 *									    
 *	Calling Sequence:						    
 *		ret = _lstout(buffer,count,func);			    
 *									    
 *	Where:								    
 *		buffer	-> the output buffer (byte 0 is count)		    
 *		ret	=  count on return				    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"

/* note: not used for GEMDOS; we can use Fwrite as usual */

int _lstout(P(const char *) buffer)
PP(register const char *buffer;)						/* -> 1st char output      */
{
	register int count;						/* =  # bytes to xfer      */
	int xcount;						/* save area for count     */

	xcount = count = (0xFF & *buffer++);	/* Copy for later      */

	while (count-- > 0)					/* Until all written       */
#ifdef FIXED_BDOS
		j30ent(3, 0, *buffer++);		/* Output next character   */
#else
		__OSIF(LSTOUT, *buffer++);		/* Output next character   */
#endif
	return xcount;					/* return original count   */
}

#ifdef FIXED_BDOS
long xyzzx;
VOID j30june(NOTHING)
{
	asm("_j30ent:");
	asm("      move.l (a7)+,xyzzx");
	asm("      trap   #13");
	asm("      move.l xyzzx,-(a7)");
	asm("      rts");
}
#endif
