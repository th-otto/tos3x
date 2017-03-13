/***************************************************************************
 *									    
 *			W r t a s c    F u n c t i o n			    
 *			------------------------------			    
 *									    
 *	Function "_wrtasc" is called from "write" to handle writes to a     
 *	file opend in ascii mode.					    
 *									    
 *	Calling sequence:						    
 *		ret = _wrtasc(fp,buffer,bytes);				    
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


long _wrtasc(P(FD *) ccb, P(const VOIDPTR) buffp, P(long) xbytes)
PP(FD *ccb;)						/* -> CCB           */
PP(const VOIDPTR buffp;)				/* -> User's buffer     */
PP(long xbytes;)					/* # bytes to write     */
{
	register FD *fp;
	register const char *buff;
	register long bytes;
	long ii, jj, kk;

	fp = ccb;
	buff = buffp;
	bytes = xbytes;
	for (jj = kk = 0; jj < bytes;)
	{									/* scan thru buff:      */
		for (; kk < bytes && buff[kk] != '\n'; ++kk)	/* Scan buff        */
			;							/* for newline chars        */
		if (kk - jj > 0)
		{								/* something to write?      */
			ii = _pc_writeblk(fp, fp->offset, buff + jj, kk - jj);
			if (ii < 0)
			{
				__set_errno(EIO);
				return -1;
			}
			fp->offset += ii;			/* Incr pos in file     */
			jj = kk;					/* advance this ptr     */
		} else
		{								/* It was a newline     */
			kk++;						/* write it out next time   */
			ii = _pc_writeblk(fp, fp->offset, "\r", 1L);
			if (ii < 0)
			{
				__set_errno(EIO);
				return -1;
			}
			fp->offset += ii;			/* Incr pos in file     */
		}
	}									/* end FOR loop         */
	return jj;							/* Number written       */
}
