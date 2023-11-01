/***************************************************************************
*									    
*			  R d a s c   F u n c t i o n			    
*			  ---------------------------			    
*									    
*	The "_rdasc" function performs a read operation for ascii  files.   
*	All Return chars (0x13) are eliminated (and not counted) from input.
*									    
*	Calling Sequence:						    
*		ret = _rdasc(fp,buffer,bytes);				    
*									    
*	Where:								    
*		fp	-> to the open CCB				    
*		buffer	-> the user's buffer				    
*		bytes	=  the number of bytes to be read		    
*		ret	=  the number of bytes actually read		    
*
*	Edits:
*	20-jan-84 whf	Handle PCDOS ^Z bug
****************************************************************************/

#include <osif.h>
#include <osiferr.h>
#include "lib.h"
#include <errno.h>

/* Zaps CRs in buff */
size_t _cr_collapse PROTO((char *buf, size_t cnt));

size_t _rdasc(P(register FD *) fp, P(VOIDPTR) buff, P(long) bytes)
PP(register FD *fp;)								/* -> ccb for the file      */
PP(register VOIDPTR buff;)							/* -> buffer to receive data */
PP(register long bytes;)							/* =  # bytes to xfer       */
{
#if CPM	   /*===============================================================*/
	register char *p1;						/* Temp byte pointer        */
	char c;								/* Temp char            */
	long xsector;						/* Temp sector number       */
	long xbytes;						/* byte count temp      */

	xbytes = bytes;						/* Remember org. request    */
	while (bytes > 0)					/* Until read is satisfied  */
	{
		xsector = fp->offset >> 7;		/* Calc starting sector     */
		if (xsector != fp->sector)		/* Match sector in buffer?? */
		{								/* No, must read first      */
			if ((fp->flags & DIRTY) != 0)	/* Buffer dirty??       */
			{							/* Yes, must write it       */
				if (_blkio(fp, fp->sector, fp->buffer,	/* Try to write buffer      */
						   1L, B_WRITE) != 1)	/*              */
					RETERR(FAILURE, EIO);	/* Unable to write, quit    */
				fp->flags &= ~DIRTY;	/* Wipe dirty bit       */
			}
			if (_blkio(fp, xsector, fp->buffer, 1L,	/* Read proper sector       */
					   B_READ) != 1)
			{							/* Assume no sparse ascii fs */
				fp->flags |= ATEOF;		/* Set EOF flag         */
				return (xbytes - bytes);	/* Do partial read      */
			}
			fp->sector = xsector;		/* Mark buffer correctly    */
		}
		p1 = &(fp->buffer[(int) fp->offset & (SECSIZ - 1)]);	/* Load byte pointer */
		/*              */
		while (p1 < &(fp->buffer[SECSIZ]) &&	/* Ie, more data in buffer  */
			   bytes > 0)				/* And request not satisfied */
		{
			c = *p1;					/* Pick up next character   */
			if (c == EOFCHAR)			/* ^Z??             */
			{							/* Yes,             */
				fp->flags |= ATEOF;		/*  Set EOF flag        */
				if (fp->offset > fp->hiwater)	/* set hiwater mark     */
					fp->hiwater = fp->offset;	/*     if necessary     */
				return (xbytes - bytes);	/* Return number read       */
			}
			else if (c == '\r')			/* Carriage return?     */
			{
				p1++;					/* Yes, just ignore that    */
				fp->offset++;			/* Increment file offset    */
			}
			else
			{							/* Regular character        */
				*buff++ = c;			/* Load buffer with byte    */
				bytes--;				/* Decrement count      */
				p1++;					/* increment counter        */
				fp->offset++;			/* Increment file offset    */
			}
		}
	}									/* While bytes > 0      */
#endif

#if PCDOS
	long xbytes;						/* byte count save      */
	size_t ii, jj;						/* More byte count temps    */

	xbytes = bytes;						/* Remember org. request    */
	while (bytes > 0 && (fp->flags & ATEOF) == 0)	/* Until read is satisfied  */
	{
		ii = _pc_readblk(&(fp->fcb), fp->offset, buff, (size_t) bytes, 1);
		for (jj = 0; jj < ii; ++jj)		/* EOF scan         */
			if (((char *)buff)[jj] == EOFCHAR)
				break;					/* Last char EOF?       */
		if (ii == 0 || ((char *)buff)[jj] == EOFCHAR)
		{								/* No chars or Last char EOF? */
			ii = jj;					/* Num valid chars      */
			fp->flags |= ATEOF;			/* Yes, mark file       */
		}
		jj = _cr_collapse(buff, ii);	/* Stomp out Returns (0x13) */
		fp->offset += ii;				/* Calculate new offset     */
#ifdef __ALCYON__
		buff += jj;						/* advance the buffer ptr   */
#else
		buff = (char *)buff + jj;		/* advance the buffer ptr   */
#endif
		bytes -= jj;					/* discount by bytes in buff */
	}									/* While bytes > 0      */
	xbytes -= bytes;					/* Number really read       */
#endif


	if (fp->offset > fp->hiwater)		/* Fix up hiwater mark      */
		fp->hiwater = fp->offset;
	return xbytes;					/* Read fully satisfied     */
}




#if PCDOS

size_t _cr_collapse(P(char *) buf, P(size_t) cnt)			/* Stomp out Returns (0x13) */
PP(register char *buf;)							/* Where to stomp       */
PP(size_t cnt;)								/* How much of buf to stomp */
{
	register char *chk;						/* Check pointer        */
	char *savbuf;						/* Save pointer         */
	char *endbuf;						/* Move to pointer      */

	savbuf = buf;						/* save for return val calc */
	endbuf = buf + cnt;					/* after last char in buf   */
	for (chk = buf; chk < endbuf; chk++)	/* Check out entire buffer  */
		if (*chk != '\r')				/* if NOT a Return:     */
			*buf++ = *chk;				/*   move it down       */
	return (long)buf - (long)savbuf;				/* New num chars in buf     */
}
#endif
