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
#include <string.h>

#define OFFSECT   (fp->offset    & ~(SECSIZ-1))
#define HIWSECT   (fp->hiwater-1 & ~(SECSIZ-1))


size_t _wrtbin(P(FD *) fp, P(const VOIDPTR) buff, P(size_t) bytes)
PP(register FD *fp;)				/* -> CCB           */
PP(const VOIDPTR buff;)	/* -> User's buffer     */
PP(size_t bytes;)			/* # bytes to write     */
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
	unsigned int xsector;					/* Sector temp          */
	unsigned int nsector;					/* Multi-sector count       */
	size_t written;					/* # bytes to write     */
	unsigned int offset;
	register unsigned int count;

	written = 0;					/* Remember request length  */
	xsector = fp->offset >> 7;			/* Calculate present sector */

	if (xsector != fp->sector)		/* No, do sectors match?    */
	{
		if ((fp->flags & DIRTY) != 0)	/* No, is buffer dirty?     */
		{
			if (_blkio(fp, fp->sector, fp->buffer, 1L, B_WRITE) != 1)	/* Yes, write it        */
				RETERR(FAILURE, EIO);	/* Couldn't write buffer    */
		}

		if (OFFSECT > HIWSECT)		/* Within the hiwater area? */
		{							/* If yes, then read it     */
			memset(fp->buffer, 0, SECSIZ);	/* Zero out the buffer      */
		} else
		{
			if (_blkio(fp, (long)xsector, fp->buffer, 1L, B_READ) != 1)	/* Try to read the correct  */
				RETERR(FAILURE, EIO);	/* Can't            */
		}

		fp->flags &= ~DIRTY;	/* Clear dirty bit      */
		fp->sector = xsector;		/* Label buffer         */
	}
	offset = (unsigned int) (fp->offset & (SECSIZ - 1));
	count = SECSIZ - offset;
	if (count > bytes)
		count = bytes;
	else if (count == SECSIZ)
		count = 0;

	if (count > 0)
	{
		blkmove(&fp->buffer[offset], buff, count);
		written += count;
		fp->offset += count;
		buff += count;
		bytes -= count;
		if (count + offset != SECSIZ)	/* buffer full?         */
		{							/* yes...           */
			fp->flags |= DIRTY;		/* Yes, buffer is now dirty */
			fp->sector = xsector;
		} else
		{
			if (_blkio(fp, (long)xsector, fp->buffer, 1L,	/* Write full buffer   */
					   B_WRITE) != 1)
				RETERR(FAILURE, EIO);	/* Couldn't         */
			fp->flags &= ~DIRTY;	/* turn off dirty bit       */
			fp->sector = -1;
			xsector++;						/* Bump sector counter      */
		}
	}

	/* End boundary problem code */
	/****************************/
	if (bytes >= SECSIZ)
	{
		/*************************************************			    */
		/* The observant reader will note that after the above malarkey, we are now */
		/* aligned on a sector boundary.  The following code exploits the oppor-    */
		/* tunity to do a multi-sector write.                      */
		/*************************************************			    */

		nsector = bytes / SECSIZ;				/* divide by 128 for sectors */
		if (_blkio(fp, (long)xsector, buff, (long)nsector, B_WRITE) != nsector)	/* Multi-sector xfer        */
			RETERR(FAILURE, EIO);		/* Just quit on error       */
		xsector += nsector;
		/* Byte count           */
		count = nsector * SECSIZ;
		written += count;				/* Update address       */
		fp->offset += count;			/* Update offset now        */
		buff += count;
		bytes -= count;					/* Subtract multi-sector    */
	}

	if (bytes > 0)
	{
		if (OFFSECT > HIWSECT)		/* Within the hiwater area? */
		{							/* If yes, then read it     */
			memset(fp->buffer, 0, SECSIZ);	/* Zero out the buffer      */
		} else
		{
			if (_blkio(fp, (long)xsector, fp->buffer, 1L, B_READ) != 1)	/* Try to read the correct  */
				RETERR(FAILURE, EIO);	/* Can't            */
		}
		blkmove(fp->buffer, buff, bytes);
		fp->flags |= DIRTY;
		fp->sector = xsector;
		written += bytes;
		fp->offset += bytes;
		buff += bytes;
	}

	if (fp->offset > fp->hiwater)		/* Fix up hiwater mark      */
		fp->hiwater = fp->offset;		/*              */
	return written;				/*      return success  */
#endif
}
