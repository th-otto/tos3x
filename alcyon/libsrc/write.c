/***************************************************************************
 *									    
 *			   W r i t e   F u n c t i o n			    
 *			   ---------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	Function "write" simulates the UNIX write system call.  Any 	    
 *	arbitrary number of bytes are written to the file specified by file 
 *	descriptor.  No special alignment of file or buffer is required.    
 *									    
 *	Calling Sequence:						    
 *		ret = write(fd,buffer,bytes);				    
 *	Where:								    
 *		fd	is an open file descriptor			    
 *		buffer	is the buffer address				    
 *		bytes	is the number of bytes to be written		    
 *		ret	is the number of bytes actually written		    
 *									    
 *	Modifications:							    
 *		11-Dec-83 whf	PC-DOS mods, divide out _wrtasc & _wrtbin.  
 *		30-Nov-83 whf	Fix _wrtbin() boundary condition bug	    
 *		19-Oct-83 whf	Separate out _wrtchr(), _ttyout(), _lstout()
 *									    
 ****************************************************************************/
#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>

size_t write(P(int) fd, P(const VOIDPTR) buff, P(size_t) bytes)
PP(int fd;)									/* File descriptor      */
PP(const VOIDPTR buff;)						/* Buffer address       */
PP(size_t bytes;)							/* Number of bytes to xfer  */
{
	register FD *fp;							/* File (ccb) pointer       */
	register long xbytes = bytes;
	
	/* Get CCB address MGL */
	if ((fp = _chkc(fd)) == NULLFD)
		return -1;
	/* note: bytes is unsigned  */
	if (bytes == 0)						/* Trying to write 0        */
		return 0;						/* Yes, a wise guy!     */

	if ((fp->flags & ISREAD) != 0)		/* Check for readonly file  */
		RETERR(-1, EBADF);			/* Barf if so           */

#if !GEMDOS
	if (fp->flags & (ISTTY | ISLPT | ISQUE))	/* TTY, LST or QUE File?    */
		return _wrtchr(fp, buff, xbytes);	/*  Yes, handle it      */
#endif

	if (fp->flags & ISASCII)			/* If ascii file        */
		return _wrtasc(fp, buff, xbytes);	/* do ascii style       */
	return _wrtbin(fp, buff, xbytes);	/* do binary style      */
}
