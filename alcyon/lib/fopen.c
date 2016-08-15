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

static FILE *_fopen(P(register const char *) name, P(register const char *) mode, P(int) ascii)
PP(register const char *name;)			/* file name            */
PP(register const char *mode;)				/* "r","w", or "a"      */
PP(int ascii;)								/* CP/M text file       */
{
	register FILE *sp;					/* stream pointer       */
	register int ii;					/* index into _iob      */
	register int fd;					/* file descriptor      */

	/* look at _iob table not marked rd/wrt */
	for (ii = 0; ii < MAXFILES && (sp = (&_iob[ii]))->_flag & (_IOREAD | _IOWRT); ii++)
		;
	if (ii >= MAXFILES)
	{
		errno = EMFILE;
		return NULL;				/*   fail           */
	}
	if (*mode == 'w' || *mode == 'W')	/* 'w'rite mode?        */
		fd = _creat(name, CREATMODE, ascii);	/*  create file ******** */
	else if (*mode == 'a' || *mode == 'A')	/* 'a'ppend mode?       */
	{									/*              */
		if ((fd = _open(name, O_WRONLY, ascii)) < 0)	/* try open      */
			fd = _creat(name, CREATMODE, ascii);	/* ow. do create    */
		else
			lseek(fd, 0L, SEEK_END);			/* its out there, seek EOF  */
	}
	else if (*mode == 'r' || *mode == 'R')	/* 'r'ead mode?         */
		fd = _open(name, O_RDONLY, ascii);	/*  try open *********** */
	else
		return NULL;					/* bad mode barf...     */

	if (fd < 0)							/* did one of those work?   */
		return NULL;					/*  no, oh well     */
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */
	if (*mode == 'r' || *mode == 'R')	/* 'r'ead mode?         */
		sp->_flag = _IOREAD;			/*  say so          */
	else
		sp->_flag = _IOWRT;				/* else 'w'rite mode        */
	if (ascii == 0)						/* ascii mode?          */
		sp->_flag |= _IOASCI;			/*              */

	return sp;						/* return the stream ptr    */
}


/* ascii file open */
FILE *fopen(P(const char *) name, P(const char *) mode)
PP(const char *name;)
PP(const char *mode;)
{
	return _fopen(name, mode, 0);
}

/* ascii file open */
FILE *fopena(P(const char *) name, P(const char *) mode)
PP(const char *name;)
PP(const char *mode;)
{
	return _fopen(name, mode, 0);
}

/* binary file open */
FILE *fopenb(P(const char *) name, P(const char *) mode)
PP(const char *name;)
PP(const char *mode;)
{
	return _fopen(name, mode, 1);
}
