/*****************************************************************************
*
*			f r e o p e n   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"freopen" opens a buffered stream file, either on disk, console, or
*	line printer.  It first closes the given stream, and reuses that slot.
*	Comes in 3 flavors, like "fopen".
*
*	Calling sequence:
*		stream = freopen(name,mode,stream)
*		stream = freopa(name,mode,stream)
*		stream = freopb(name,mode,stream)
*		stream = _freopen(name,mode,stream,binary)
*	Where:
*		stream -> file reopened (FILE *), NULL if failure
*		name   -> NULL terminated string containing filename,
*				name="CON:" means the console,
*				name="LST:" goes to the line printer, and
*				otherwise a disk file is assumed.
*		mode   -> "r" for read only,
*			  "w" for write only,
*			  "a" to append (write only) to end of file.
*		binary  = 0 if the file is handled as a CP/M text file
*			(newline -> CRLF xlation, ^Z EOF convention).
*			Otherwise, file is treated as a binary file.
*
*****************************************************************************/

#include "lib.h"
#include <fcntl.h>


FILE *_freope PROTO((const char *name, const char *mode, FILE *sp, int binary));


FILE *_freope(P(const char *) name, P(const char *) mode, P(FILE *) sp, P(int) binary)
PP(register const char *name;)							/* file name            */
PP(register const char *mode;)									/* "r","w", or "a"      */
PP(register FILE *sp;)							/* stream pointer       */
PP(int binary;)							/* CP/M text file       */
{
	register int fd;						/* file descriptor      */

	if (fclose(sp) == EOF)			/* try closing the file 1st */
		return NULL;				/*   oops, it failed        */
	if (*mode == 'w' || *mode == 'W')	/* 'w'rite mode?        */
		fd = _creat(name, 0, binary);	/*  create file ******** */

	else if (*mode == 'a' || *mode == 'A')	/* 'a'ppend mode?       */
	{									/*              */
		if ((fd = _open(name, O_WRONLY, binary)) < 0)	/* try open      */
			fd = _creat(name, 0, binary);	/*  do create if needed */
		else
			lseek(fd, 0L, SEEK_END);			/* its out there, seef EOF  */
	}
	else if (*mode == 'r' || *mode == 'R')	/* 'r'ead mode?         */
		fd = _open(name, O_RDONLY, binary);	/*  try open *********** */
	else
		return NULL;				/* bad mode barf...     */

	if (fd < 0)							/* did one of those work?   */
		return NULL;				/*  no, oh well     */
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */
	if (*mode == 'r' || *mode == 'R')	/* 'r'ead mode?         */
		sp->_flag = _IOREAD;			/*  say so          */
	else
		sp->_flag = _IOWRT;				/* else 'w'rite mode        */
	if (binary == 0)						/* binary file?          */
		sp->_flag |= _IOASCI;			/*              */

	return sp;						/* return the stream ptr    */
}


FILE *freopen(P(const char*) name, P(const char*) mode, P(FILE *) sp)
PP(const char *name;)
PP(const char *mode;)
PP(FILE *sp;)
{
	return _freope(name, mode, sp, 0);
}										/* reopen ascii file        */


FILE *freopa(P(const char*) name, P(const char*) mode, P(FILE *) sp)
PP(const char *name;)
PP(const char *mode;)
PP(FILE *sp;)
{
	return _freope(name, mode, sp, 0);
}										/* reopen ascii file        */


FILE *freopb(P(const char*) name, P(const char*) mode, P(FILE *) sp)
PP(const char *name;)
PP(const char *mode;)
PP(FILE *sp;)
{
	return _freope(name, mode, sp, 1);
}
