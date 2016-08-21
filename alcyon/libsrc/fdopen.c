/***************************************************************************
*
*			f d o p e n   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fdopen" associates a file which was opened by a file descriptor
*	(using "open" or "creat") with a stream.
*
*	Calling sequence:
*		stream = fdopen( fd, mode )
*	Where:
*		stream -> stream info (FILE *) (NULLPTR returned on fail)
*		fd     =  small int returned by open or creat
*		mode   = "r" for read-only, "w" for write, "a" for append
*
*****************************************************************************/

#include "lib.h"
#include <errno.h>
#include <fcntl.h>


FILE *fdopen(P(int) fd, P(const char *) mode)
PP(register int fd;)
PP(register const char *mode;)
{
	register FILE *sp;
	register int ii;
	register int flags;

	/* is fd valid? */
	if (fd < 0 || lseek(fd, 0L, SEEK_CUR) == EOF)
		return NULL;
	/* look at _iob table not marked rd/wrt */
	for (ii = 0; ii < MAXFILES && (sp = (&_iob[ii]))->_flag & (_IOREAD | _IOWRT); ii++)
		;
	if (ii >= MAXFILES)
	{
		__set_errno(EMFILE);
		return NULL;				/*   fail           */
	}
	if ((flags = __getmode(mode)) == -1)
		return NULL;
	/* not 'r'ead mode? */
	if ((flags & O_ACCMODE) != O_WRONLY)
		sp->_flag = _IOREAD;
	if ((flags & O_ACCMODE) != O_RDONLY)
		sp->_flag = _IOWRT;				/* else 'w'rite mode        */
	if (flags & O_TEXT)
		sp->_flag |= _IOASCI;
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */
	if (flags & O_APPEND)
		lseek(fd, 0L, SEEK_END);			/* its out there, seef EOF  */
	
	return sp;						/* return the stream ptr    */
}
