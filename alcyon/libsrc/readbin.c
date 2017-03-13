/***************************************************************************
 *									    
 *			  R d b i n   F u n c t i o n			    
 *			  ---------------------------			    
 *									    
 *	The "_rdbin" function performs a read operation for binary files.   
 *									    
 *	Calling Sequence:						    
 *		ret = _rdbin(fp,buffer,bytes);				    
 *									    
 *	Where:								    
 *		fp	-> to the open CCB				    
 *		buffer	-> the user's buffer				    
 *		bytes	=  the number of bytes to be read		    
 *		ret	=  the number of bytes actually read		    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <errno.h>

long _rdbin(P(register FD *) fp, P(VOIDPTR) buff, P(long) bytes)
PP(register FD *fp;)								/* -> ccb for the file      */
PP(register VOIDPTR buff;)							/* -> buffer to receive data */
PP(register long bytes;)							/* =  # bytes to xfer       */
{
	long xbytes;						/* More byte count temps    */

	xbytes = _pc_readblk(fp, fp->offset, buff, bytes);
	if (xbytes == 0)					/* EOF or error condition   */
		fp->flags |= ATEOF;				/* Set EOF flag         */
	if (xbytes < 0)
	{
		__set_errno(EIO);
		return -1;
	}
	fp->offset += xbytes;				/* Calculate new offset     */

	return xbytes;					/* Read fully satisfied     */
}
