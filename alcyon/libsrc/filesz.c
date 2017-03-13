/**********************************************************************
*
* 		_ f i l e s z   f u n c t i o n
* 		-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	This routine computes the size of a CP/M file, and leaves the fd
*	pointing to the end of file.  
*	Note that the file must have been previously opened.
*
*	Calling sequence:
*		size = _filesz(fd)
*	where:
*		size =	# bytes in file;
*			0 if not disk file;
*			-1 if I/O error
*		fd   = file descriptor returned by open.
*
***********************************************************************/

#include <osif.h>
#include "lib.h"
#include <errno.h>


long _filesz(P(int) fd)
PP(int fd;)								/* file descriptor      */
{
	register FD *fp;

	if ((fp = _chkc(fd)) == NULLFD)		/* Convert to pointer       */
		return -1;				/*  Can't: EBADF        */
	if (fp->flags & (ISTTY | ISLPT))	/* Non disk file?       */
		return 0;
	fp->offset = Fseek(0L, fp->dosfd, SEEK_END);

	fp->flags |= ATEOF;					/* make sure they know      */
	return fp->offset;					/* this is it           */
}
