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

#define OFFSECT   (fp->offset    & ~(SECSIZ-1))
#define HIWSECT   (fp->hiwater-1 & ~(SECSIZ-1))


size_t _wrtasc(P(FD *) fp, P(const VOIDPTR) buff, P(size_t) bytes)
PP(register FD *fp;)						/* -> CCB           */
PP(register const VOIDPTR buff;)				/* -> User's buffer     */
PP(register size_t bytes;)					/* # bytes to write     */
{
#if PCDOS
	size_t ii, jj, kk;

	for (jj = kk = 0; jj < bytes;)
	{									/* scan thru buff:      */
		for (; kk < bytes && buff[kk] != '\n'; ++kk)	/* Scan buff        */
			;							/* for newline chars        */
		if (kk - jj > 0)
		{								/* something to write?      */
			ii = _pc_writeblk(&(fp->fcb), fp->offset, buff + jj, kk - jj, 1);	/*     */
			fp->offset += ii;			/* Incr pos in file     */
			if (ii == 0)				/* Problems?            */
				RETERR(-1, EIO);	/* Tell them so         */
			jj = kk;					/* advance this ptr     */
		} else
		{								/* It was a newline     */
			kk++;						/* write it out next time   */
			ii = _pc_writeblk(&(fp->fcb), fp->offset, "\r", 1, 1);	/*       */
			fp->offset += ii;			/* Incr pos in file     */
		}
	}									/* end FOR loop         */
	if (fp->offset > fp->hiwater)		/* Need to advance?     */
		fp->hiwater = fp->offset;		/*   then do so         */
	return jj;							/* Number written       */
#endif

#if CPM
	register char *p1;					/* Temp buffer pointer      */
	register char cc;					/* character temp       */
	long xsector;						/* Sector temp          */
	long nsector;						/* Multi-sector count       */
	long written;						/* # bytes to write     */
	long xbytes;						/* Save byte count remaining */

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

			if (OFFSECT <= HIWSECT)		/* and below hiwater mark   */
			{
				if (_blkio(fp, xsector, fp->buffer, 1L, B_READ) != 1)	/* Now read the correct sec */
					RETERR(-1, EIO);	/* Couldn't         */
			}
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
			if (fp->offset > fp->hiwater)
				fp->hiwater = fp->offset;	/* Fix hiwater if necessary */
			return written;
		}
	}
#endif
}

/* unused */
static VOID _wrtclear(P(char *) ptr, P(int) bytes)
PP(register char *ptr;)
PP(register int bytes;)
{
	while (bytes-- > 0)
		*ptr++ = 0;
}

