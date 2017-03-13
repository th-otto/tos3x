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
#include <errno.h>


int close(P(int) fd)
PP(register int fd;)							/* File descriptor to close */
{
	register FD *fp;							/* file pointer Temporary   */
	register int rv;						/* return value         */

	if ((fp = _chkc(fd)) == NULLFD)		/* File Open?           */
		return -1;			/*      no, quit    */
	rv = 0;						/* assume it will work      */
	if ((fp->flags & (ISTTY | ISLPT)) == 0)	/* Character device?        */
	{									/* if not, handle file: **** */
		if ((rv = Fclose(fp->dosfd)) < 0)	/* Close the fcb       */
		{
			rv = -1;
		}
	}									/* end of file handling **** */
	__chinit(fd);						/* Release the space        */
	_freec(fd);							/* Release the channel      */
	if (rv == 0)					/* Was everything ok?       */
		return rv;					/* Yes, return Success      */
	__set_errno(_XltErr(rv, EACCES));			/* else let them know       */
	return -1;
}
