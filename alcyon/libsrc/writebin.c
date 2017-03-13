/***************************************************************************
 *									    
 *			W r t b i n    F u n c t i o n			    
 *			------------------------------			    
 *									    
 *	Function "wrtbin" is called from "write" to take advantage of a	    
 *	potential multi-sector transfer for binary files.		    
 *									    
 *	Calling sequence:						    
 *		ret = _wrtbin(fp,buffer,bytes);				    
 *									    
 *	Where:								    
 *		fp	Points to the affected ccb.			    
 *		buffer	Is the buffer address				    
 *		bytes	Is the number of bytes to write			    
 *									    
 *		ret	Is the number of bytes actually written		    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <errno.h>
#include <string.h>



long _wrtbin(P(FD *) fp, P(const VOIDPTR) buff, P(long) bytes)
PP(register FD *fp;)				/* -> CCB           */
PP(const VOIDPTR buff;)				/* -> User's buffer     */
PP(long bytes;)					/* # bytes to write     */
{
	long ii;							/* Byte counter         */

	ii = _pc_writeblk(fp, fp->offset, buff, bytes);
	if (ii <= 0)						/* Problems?            */
	{
		__set_errno(EIO);			/* Tell them so         */
		return -1;
	}
	fp->offset += ii;					/* Incr pos in file     */
	return ii;							/* Number written       */
}
