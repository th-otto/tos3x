/***************************************************************************
 *									    
 *			_ t t y o u t   F u n c t i o n			    
 *			-------------------------------			    
 *									    
 *	Function "_ttyout" is called to perform "write" operations to the   
 *	console device.							    
 *									    
 *	Calling Sequence:						    
 *		ret = _ttyout(buffer);					    
 *									    
 *	Where:								    
 *		buffer	-> the output buffer (byte 0 is count)		    
 *		ret	=  count on return				    
 ****************************************************************************/
#include <osbind.h>
#include "lib.h"

int _ttyout(P(const char *) buf)
PP(register const char *buf;)							/* -> 1st char output      */
{
	int count;							/* =  # bytes to xfer      */

	count = *buf++ & 0xff;						/* Get num bytes to output */
	count = Fwrite(1, (long)count, buf);
	return count;						/* return original count   */
}
