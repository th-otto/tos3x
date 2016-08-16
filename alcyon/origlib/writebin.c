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
#include <osiferr.h>
#include <errno.h>

#define OFFSECT   (fp->offset    & ~(SECSIZ-1))
#define HIWSECT   (fp->hiwater-1 & ~(SECSIZ-1))


size_t _wrtbin(P(FD *) fp, P(const VOIDPTR) buff, P(size_t) bytes)
PP(register FD *fp;)				/* -> CCB           */
PP(const VOIDPTR buff;)				/* -> User's buffer     */
PP(size_t bytes;)					/* # bytes to write     */
{
#if PCDOS
	size_t ii;							/* Byte counter         */

	ii = _pc_writeblk(&(fp->fcb), fp->offset, buff, bytes, 1);	/*       */
	if (ii == 0)						/* Problems?            */
		RETERR(-1, EIO);			/* Tell them so         */
	fp->offset += ii;					/* Incr pos in file     */
	if (fp->offset > fp->hiwater)		/* Need to advance?     */
		fp->hiwater = fp->offset;		/*   then do so         */
	return ii;							/* Number written       */
#endif

#if CPM		 /*=============================================================*/
	static long xsector;					/* Sector temp          */

	static long nsector;					/* Multi-sector count       */

	static long written;					/* # bytes to write     */

	register char *p1;					/* Temp buffer pointer      */

	written = bytes;					/* Remember request length  */
	xsector = fp->offset >> 7;			/* Calculate present sector */

	if ((fp->offset & (SECSIZ - 1)) != 0)	/* Are we at a boundary?    */
	{
		if (xsector != fp->sector)		/* No, do sectors match?    */
		{
			if ((fp->flags & DIRTY) != 0)	/* No, is buffer dirty?     */
			{
				if (_blkio(fp, fp->sector, fp->buffer, 1L, B_WRITE) != 1)	/* Yes, write it        */
					RETERR(FAILURE, EIO);	/* Couldn't write buffer    */
				fp->flags &= ~DIRTY;	/* Clear dirty bit      */
			}

			if (OFFSECT <= HIWSECT)		/* Within the hiwater area? */
			{							/* If yes, then read it     */
				if (_blkio(fp, xsector, fp->buffer, 1L, B_READ) != 1)	/* Try to read the correct  */
					RETERR(FAILURE, EIO);	/* Can't            */
			}
			else
				blkfill(fp->buffer, 0, SECSIZ);	/* Zero out the buffer      */
			
			fp->sector = xsector;		/* Label buffer         */
		}
		p1 = &fp->buffer[(int) fp->offset & (SECSIZ - 1)];	/* 1st loc in buffer */
		
		while (p1 < fp->buffer + SECSIZ &&	/* while still in buffer    */
			   bytes > 0)				/* And still writing        */
		{
			*p1++ = *buff++;			/* Move a byte          */
			bytes--;					/* Decrement counter        */
		}
		if (bytes <= 0)					/* Byte count exhausted??   */
		{
			if (p1 == fp->buffer + SECSIZ)	/* buffer full?         */
			{							/* yes...           */
				if (_blkio(fp, xsector, fp->buffer, 1L,	/* Write full buffer   */
						   B_WRITE) != 1)
					RETERR(FAILURE, EIO);	/* Couldn't         */
				fp->flags &= ~DIRTY;	/* turn off dirty bit       */
			} else
				fp->flags |= DIRTY;		/* Yes, buffer is now dirty */
			fp->offset += written;		/* fix offset           */
			if (fp->offset > fp->hiwater)	/* See if above hiwater mark */
				fp->hiwater = fp->offset;	/* Fix if necessary     */
			return (written);			/* Return original byte cnt */
		}
		if (_blkio(fp, xsector, fp->buffer, 1L,	/* Write full buffer        */
				   B_WRITE) != 1)
			RETERR(FAILURE, EIO);		/* Couldn't         */
		xsector++;						/* Bump sector counter      */
	}


	/* End boundary problem code */
	/****************************/
	
	/*************************************************			    */
	/* The observant reader will note that after the above malarkey, we are now */
	/* aligned on a sector boundary.  The following code exploits the oppor-    */
	/* tunity to do a multi-sector write.                      */
	/*************************************************			    */
	
	nsector = bytes >> 7;				/* divide by 128 for sectors */
	if (nsector > 0)					/* Check for no more left   */
	{
		if ((xsector <= fp->sector) || (fp->sector < xsector + nsector))
		{
			fp->flags &= ~DIRTY;
			fp->sector = -1;
		}
		if (_blkio(fp, xsector, buff, nsector, B_WRITE) != nsector)	/* Multi-sector xfer        */
			RETERR(FAILURE, EIO);		/* Just quit on error       */
	}
	
	bytes -= (nsector << 7);			/* Subtract multi-sector    */
	/* Byte count           */
	buff += (nsector << 7);				/* Update address       */
	fp->offset += written;				/* Update offset now        */
	/* (All I/O is complete)    */
	if (fp->offset > fp->hiwater)		/* Fix up hiwater mark      */
		fp->hiwater = fp->offset;		/*              */
	if (bytes == 0)						/* If done,         */
		return written;				/*      return success  */
	
	if ((fp->flags & DIRTY) != 0)		/* Is buffer dirty?     */
	{									/* Test (again) here in case */
		if (_blkio(fp, fp->sector,		/*  of boundary condition   */
				   fp->buffer, 1L, B_WRITE) != 1)	/* Yes, write it        */
			RETERR(FAILURE, EIO);		/* Couldn't write buffer    */
	}
	fp->flags |= DIRTY;					/* Let's dirty the buffer   */
	fp->sector = fp->offset >> 7;		/* Mark sector number       */
	if (OFFSECT <= HIWSECT)				/* Sector in high water area */
		/* MGL              */
		_blkio(fp, fp->sector, fp->buffer, 1L,	/* Read sector          */
			   B_READ);					/*              */
	else
		blkfill(fp->buffer, 0, SECSIZ);
	p1 = &(fp->buffer[0]);				/* p1 -> address        */
	while (bytes > 0)					/* Move the bytes       */
	{									/*              */
		*p1++ = *buff++;				/* One at a time        */
		bytes--;						/* Decrement count      */
	}
	
	return written;					/* Return requested #       */
#endif
}
