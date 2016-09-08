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
#include <osiferr.h>
#include <errno.h>


long _wrtasc(P(FD *) ccb, P(const VOIDPTR) buffp, P(long) xbytes)
PP(FD *ccb;)						/* -> CCB           */
PP(const VOIDPTR buffp;)				/* -> User's buffer     */
PP(long xbytes;)					/* # bytes to write     */
{
	register FD *fp;
	register const char *buff;
	register long bytes;
#if PCDOS
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
				RETERR(-1, EIO);
			fp->offset += ii;			/* Incr pos in file     */
			jj = kk;					/* advance this ptr     */
		} else
		{								/* It was a newline     */
			kk++;						/* write it out next time   */
			ii = _pc_writeblk(fp, fp->offset, "\r", 1L);
			if (ii < 0)
				RETERR(-1, EIO);
			fp->offset += ii;			/* Incr pos in file     */
		}
	}									/* end FOR loop         */
	return jj;							/* Number written       */
#endif

#if CPM
	register char *p1;					/* Temp buffer pointer      */
	register char cc;					/* character temp       */
	static long xsector;					/* Sector temp          */
	static long nsector;					/* Multi-sector count       */
	static long written;					/* # bytes to write     */
	static long xbytes;					/* Save byte count remaining */

	written = bytes;					/* Remember original request */
	cc = 0;								/* Init to garbage value    */

	while (bytes > 0)					/* While more bytes to write */
	{
		xsector = fp->offset >> 7;		/* Compute beginning sector */

		if (xsector != fp->sector)		/* Have to read it first    */
		{
			if ((fp->flags & DIRTY) != 0)	/* Buffer has data?     */
			{
				if (_blkio(fp, fp->sector, fp->buffer, 1L, B_WRITE) != 1)	/* Try to write         */
					RETERR(-1, EIO);	/* Couldn't         */
				fp->flags &= ~DIRTY;	/* Nice clean buffer now    */
			}

			if (_blkio(fp, xsector, fp->buffer, 1L, B_READ) != 1)	/* Now read the correct sec */
				memset(fp->buffer, 0, SECSIZ);	/* Zero out the buffer      */
			fp->sector = xsector;		/* Mark buffer correctly    */
		}

		p1 = &(fp->buffer[(int) fp->offset & (SECSIZ - 1)]);	/* 1st char to write */
		
		xbytes = bytes;					/* Save byte count remaining */
		while (p1 < &(fp->buffer[SECSIZ])	/* Until buffer is full     */
			   && (bytes > 0))			/* or until request done    */
		{
			if (*buff == '\n' && cc != '\r')	/* need to insert C/R       */
				cc = '\r';
			else						/* Don't need to        */
			{
				cc = *buff++;
				bytes--;
			}
			*p1++ = cc;					/* Put char in buffer       */
			fp->offset++;				/* Increment file offset    */
		}
		if (p1 >= &(fp->buffer[SECSIZ]))	/* Need to write buffer     */
		{
			if (_blkio(fp, xsector, fp->buffer, 1L, B_WRITE) != 1)	/* Try the write        */
				return written - xbytes;	/* return # actually written */
		}
		else
			fp->flags |= DIRTY;			/* Buffer dirty again       */
		if (bytes == 0)					/* If done,         */
		{								/* Check offset here        */
			return written;
		}
	}
#endif
}
