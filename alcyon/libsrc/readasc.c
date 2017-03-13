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
#include "lib.h"
#include <errno.h>

/* Zaps CRs in buff */

static long _cr_collapse(P(char *) buf, P(long) cnt)
PP(register char *buf;)							/* Where to stomp       */
PP(long cnt;)								/* How much of buf to stomp */
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


long _rdasc(P(register FD *) fp, P(VOIDPTR) buff, P(long) bytes)
PP(register FD *fp;)								/* -> ccb for the file      */
PP(register VOIDPTR buff;)							/* -> buffer to receive data */
PP(register long bytes;)							/* =  # bytes to xfer       */
{
	long xbytes;						/* byte count save      */
	long ii, jj;						/* More byte count temps    */

	xbytes = bytes;						/* Remember org. request    */
	while (bytes > 0 && (fp->flags & ATEOF) == 0)	/* Until read is satisfied  */
	{
		ii = _pc_readblk(fp, fp->offset, buff, bytes);
		if (ii < 0)
		{
			__set_errno(EIO);
			return -1;
		}
		for (jj = 0; jj < ii; ++jj)		/* EOF scan         */
			if (((char *)buff)[jj] == EOFCHAR)
				break;					/* Last char EOF?       */
		jj = _cr_collapse(buff, jj);	/* Stomp out Returns (0x13) */
		if (jj == 0)
		{								/* No chars or Last char EOF? */
			fp->flags |= ATEOF;			/* Yes, mark file       */
			break;
		}
		fp->offset += ii;				/* Calculate new offset     */
#ifdef __ALCYON__
		buff += jj;						/* advance the buffer ptr   */
#else
		buff = (char *)buff + jj;		/* advance the buffer ptr   */
#endif
		bytes -= jj;					/* discount by bytes in buff */
	}									/* While bytes > 0      */
	xbytes -= bytes;					/* Number really read       */


	return xbytes;					/* Read fully satisfied     */
}
