/****************************************************************************
*
*			_ f i l b u f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"_filbuf" function is called by getc to fill the buffer.
*	It performs any initialization of the buffered on the first
*	call.
*
*	Calling sequence:
*		char = _filbuf(stream)
*	Where:
*		stream -> file info
*		char   = the first character returned, or FAILURE (-1)
*			on error or EOF
*
*****************************************************************************/

#include "lib.h"
#include <stdlib.h>
#include <errno.h>

#define CMASK 0xFF

int _filbuf(P(register FILE *) sp)
PP(register FILE *sp;)
{
	static char onebuf[MAXFILES];		/* a place if no mem avail. */

	if (!__validfp(sp))
	{
		__set_errno(EINVAL);
		return EOF;
	}
	if ((sp->_flag & _IOREAD) == 0)		/* is readable file?        */
	{
		return EOF;
	}
	if (sp->_flag & _IOSTRI)			/* is this stream a string? */
	{									/*    yes: handle EOS as EOF */
		sp->_flag |= _IOEOF;
		return EOF;
	}
	if (sp->_base == NULL)				/* has this been done?      */
	{
		/* is the No Buf flag set? */
		if (sp->_flag & _IONBF)
			sp->_flag |= _IONBF;		/*   set No Buf flag        */
		else if ((sp->_base = malloc(BUFSIZ)) == NULL)	/* can't we get buffer?   */
			sp->_flag |= _IONBF;		/*   set No Buf flag        */
		else
			sp->_flag |= _IOABUF;		/* we're all set        */
	}
	if (sp->_flag & _IONBF)			/* insure this set right    */
		sp->_base = &onebuf[fileno(sp)];	/*   set 'buf' to small buf */
	if (sp == stdin && (stdout->_flag & _IOLBF))	/* console i/o?      */
		fflush(stdout);					/* output whatever to con   */
	sp->_cnt = read(fileno(sp), sp->_base,	/* read to our buffer       */
					sp->_flag & _IONBF ? 1 : BUFSIZ);	/*   the right # of bytes   */
	/* did read screw up? */
	if (sp->_cnt <= 0)
	{
		if (sp->_cnt < 0)
			sp->_flag |= _IOERR | _IOEOF;
		else
			sp->_flag |= _IOEOF;		/* or just say we can't read */
		return EOF;
	}
	sp->_flag |= _IOREDN;
	sp->_cnt--;							/* take the 1st item        */
	sp->_ptr = sp->_base;				/* set up stream        */
	return (((int) (*sp->_ptr++) & CMASK));	/* and return the char      */
}
