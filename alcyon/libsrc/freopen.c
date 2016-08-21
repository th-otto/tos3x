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


FILE *freopen(P(const char *) name, P(const char *) mode, P(FILE *) sp)
PP(register const char *name;)							/* file name            */
PP(register const char *mode;)									/* "r","w", or "a"      */
PP(register FILE *sp;)							/* stream pointer       */
{
	register int fd;						/* file descriptor      */
	register int flags;
	
	if (fclose(sp) == EOF)			/* try closing the file 1st */
		return NULL;				/*   oops, it failed        */
	if ((flags = __getmode(mode)) == -1)
		return NULL;
	fd = open(name, flags, 0644);	/*  open file ******** */

	if (fd < 0)							/* did one of those work?   */
		return NULL;				/*  no, oh well     */
	sp->_cnt = 0;						/* init count           */
	sp->_fd = fd;						/*  and file des        */
	sp->_base = sp->_ptr = NULL;		/*  and buffer pointers     */

	return sp;						/* return the stream ptr    */
}
