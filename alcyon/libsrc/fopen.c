/**************************************************************************
*
*			f o p e n   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fopen" opens a buffered stream file, either on disk, console, or
*	line printer.
*	Note that "fopen" comes in three (3) flavors, one for ascii CP/M
*	files, another for binary files, and the default (presently to
*	ascii files).
*
*	Calling sequence:
*		stream = fopena(name,mode)	(ascii files)
*		stream = fopenb(name,mode)	(binary files)
*		stream = fopen(name,mode)	(default-ascii files)
*		stream = _fopen(name,mode,ascii)
*	Where:
*		stream -> file opened (FILE *), NULLPTR if failure
*		name   -> NULL terminated string containing filename,
*				name="CON:" means the console,
*				name="LST:" goes to the line printer, and
*				otherwise a disk file is assumed.
*		mode   -> "r" for read only,
*			  "w" for write only,
*			  "a" to append (write only) to end of file.
*		ascii  = 0 if the file is handled as a CP/M text file
*			(newline -> CRLF xlation, ^Z EOF convention).
*			Otherwise, file is treated as a binary file.
*
*****************************************************************************/

#include "lib.h"
#include <fcntl.h>
#include <errno.h>

FILE *fopen(P(register const char *) name, P(register const char *) mode)
PP(register const char *name;)			/* file name            */
PP(register const char *mode;)				/* "r","w", or "a"      */
{
	register FILE *sp;					/* stream pointer       */
	register int ii;					/* index into _iob      */
	register int fd;					/* file descriptor      */
	register int flags;
	
	if ((flags = __getmode(mode)) == -1)
		return NULL;
	
	/* look at _iob table not marked rd/wrt */
	for (ii = 0; ii < MAXFILES && (sp = (&_iob[ii]))->_flag & (_IOREAD | _IOWRT); ii++)
		;
	if (ii >= MAXFILES)
	{
		__set_errno(EMFILE);
		return NULL;				/*   fail           */
	}
	fd = open(name, flags, 0644);	/*  try open *********** */

	if (fd < 0)							/* did one of those work?   */
		return NULL;					/*  no, oh well     */
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */
	if ((flags & O_ACCMODE) != O_WRONLY)
		sp->_flag = _IOREAD;
	if ((flags & O_ACCMODE) != O_RDONLY)
		sp->_flag = _IOWRT;				/* else 'w'rite mode        */
	if (flags & O_TEXT)
		sp->_flag |= _IOASCI;

	return sp;						/* return the stream ptr    */
}
