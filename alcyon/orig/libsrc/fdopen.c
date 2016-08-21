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
*		stream -> stream info (FILE *) (NULL returned on fail)
*		fd     =  small int returned by open or creat
*		mode   = "r" for read-only, "w" for write, "a" for append
*
*****************************************************************************/

#include "lib.h"

FILE *fdopen(P(int) fd, P(const char *) mode)
PP(register int fd;)
PP(register const char *mode;)
{
	register FILE *sp;
	register int ii;

	/* is fd valid? */
	if (fd < 0 || lseek(fd, 0L, SEEK_CUR) == EOF)
		return NULL;
	/* look at _iob table not marked rd/wrt */
	for (ii = 0; (sp = (&_iob[ii]))->_flag & (_IOREAD | _IOWRT); ii++)
		if (ii >= MAXFILES)
			break;
	if (ii >= MAXFILES)
		return NULL;
	/* not 'r'ead mode? */
	if (*mode != 'r' && *mode != 'R')
	{
		/* set this flag */
		sp->_flag |= _IOWRT;
		if (*mode == 'a' || *mode == 'A')
			lseek(fd, 0L, SEEK_END);			/* its out there, seef EOF  */
	} else
	{
		sp->_flag |= _IOREAD;			/* 'r'ead mode          */
	}
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */
	
	return sp;						/* return the stream ptr    */
}
