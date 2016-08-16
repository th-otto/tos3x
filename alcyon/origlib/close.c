/***************************************************************************
 *									    
 *			  C l o s e   F u n c t i o n			    
 *			  ---------------------------			    
 *	Copyright 1982 by Digital Research Inc.  All rights reserved.	    
 *									    
 *	The close function is used to terminate access to a file / device.  
 *									    
 *	Calling Sequence:						    
 *									    
 *		ret = close(fd);					    
 *									    
 *	Where "fd" is an open file descriptor to be CLOSEd.		    
 *									    
 *	Modifications:							    
 *	12/83: PC-DOS mods   whf					    
 *	10/83: changed user # handling   whf				    
 *	9/83: add steve's user # mods, change _chkc   whf		    
 *	8/83: improve error handling	whf				    
 ****************************************************************************/
#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>

/* End of file for ASCII */
extern char __xeof;

int close(P(int) fd)
PP(register int fd;)							/* File descriptor to close */
{
	register FD *fp;							/* file pointer Temporary   */
	register int rv;						/* return value         */
	register int xuser;						/* User number temporary    */

	if ((fp = _chkc(fd)) == NULLFD)		/* File Open?           */
		RETERR(-1, EBADF);			/*      no, quit    */
	rv = 0;						/* assume it will work      */
	if ((fp->flags & (ISTTY | ISLPT)) == 0)	/* Character device?        */
	{									/* if not, handle file: **** */
#if CPM
		if ((fp->flags & ISASCII) != 0 && (fp->flags & ISREAD) == 0)	/*  */
		{								/* ASCII file? not ReadOnly? */
			if (fp->offset < fp->hiwater)	/* Have we been seeking?    */
				lseek(fd, 0L, SEEK_END);		/*   Seek to EOF ifso       */
			write(fd, &__xeof, 1);		/* Write a ^Z character     */
		}
		if ((fp->flags & DIRTY) != 0)	/* Buffer dirty?        */
			if (_blkio(fp, fp->sector, fp->buffer, 1L, B_WRITE) != 1)	/* write it */
				rv = -1;			/* can't            */
		xuser = _chkuser(fp->user);		/* Check user # for change  */
		if ((__OSIF(CLOSE, &(fp->fcb)) & 0xFF) == 0xFF)	/* Close the fcb       */
			rv = -1;
		_uchkuser(fp->user, xuser);		/* Change user # if needed  */
#endif
	UNUSED(xuser);
	
#if GEMDOS
		if ((rv = jclose(fp->dosfd)) < 0)	/* Close the fcb       */
			rv = -1;
#else
#if PCDOS
		if ((__OSIF(CLOSE, &(fp->fcb)) & 0xFF) == 0xFF)	/* Close the fcb       */
			rv = -1;
#endif
#endif
	}									/* end of file handling **** */
	__chinit(fd);						/* Release the space        */
	_freec(fd);							/* Release the channel      */
	if (rv == 0)					/* Was everything ok?       */
		return rv;					/* Yes, return Success      */
	else
		RETERR(-1, EIO);			/* else let them know       */
}
