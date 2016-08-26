/***************************************************************************
 *									    
 *			    L s e e k   F u n c t i o n			    
 *			    ---------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	The lseek function changes the present position of reading	    
 *	to or writing from a file.					    
 *	The tell function returns the present position in a file.	    
 *									    
 *	Calling sequence:						    
 *									    
 *		ret = lseek(fd,offset,sense);				    
 *	or	ret = tell(fd);						    
 *									    
 *	Where:								    
 *									    
 *		fd	Is an open file descriptor			    
 *		offset	Is the (long) file offset			    
 *		sense	Is the meaning of offset			    
 *			0 => From the beginning of the file		    
 *			1 => From the present file position		    
 *			2 => From the end of the file			    
 *									    
 *		ret	Is the resulting absolute file offset		    
 *			-1 indicates failure				    
 *									    
 ****************************************************************************/

#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>


off_t lseek(P(int) fd, P(off_t) offs, P(int) whence)
PP(int fd;)									/* Open file descriptor     */
PP(off_t offs;)								/* File offset          */
PP(int whence;)								/* Sense of offset      */
{
	register FD *fp;							/* File pointer         */
	
	/* Convert to pointer */
	if ((fp = _chkc(fd)) == NULLFD)
		return -1;	
#if GEMDOS
	fp->offset = Fseek(offs, fp->dosfd, whence);
#else
	switch (whence)
	{
	case SEEK_SET:						/* From beginning of file   */
		fp->offset = offs;
		break;

	case SEEK_CUR:						/* From present position    */
		fp->offset += offs;
		break;

	case SEEK_END:						/* From end of file     */
		/* go find the end of file  */
		fp->hiwater = _filesz(fd);
		/* compute from end of file */
		fp->offset = fp->hiwater + offs;
		break;

	default:							/* All others NFG       */
		RETERR(-1, EINVAL);
	}
	/* bad seek call? */
	if (fp->offset < 0)
		fp->offset = -1;
#endif
	/* any seek clears EOF */
	fp->flags &= ~ATEOF;
	return fp->offset;
}
