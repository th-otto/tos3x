/************************************************************************
*
*			_ f l s b u f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	'_flsbuf' handles writes to the stream when its buffer is full.
*	Included is the ability to handle 'non-buffered' (_IONBUF), as
*	well as line buffered (_IOLBUF) output.  It is supposed to be
*	called by 'putc'.  It will init the buffers, as needed.
*
*	Calling sequence:
*		chr = _flsbuf( ch, stream )
*	Where:
*		ch  = the 1st char to be buffered
*		stream -> where its going (FILE *)
*		chr = ch if write ok, FAILURE (-1 int) o.w.
*
*	Modifications:
*		19-Oct-83 whf	Handle IONBUF differently
*****************************************************************************/

#include "lib.h"
#include <stdlib.h>
#include <unistd.h>


int _flsbuf(P(int) c, P(FILE *) sp)
PP(register int c;)								/* char to be written       */
PP(register FILE *sp;)							/* stream to write to       */
{
	char csave;							/* a temp save area     */
	register int n;						/* number chars written     */
	register int ns;					/* num chars sposed to be w. */

	ns = n = 0;
	/* is this file writeable?  */
	if ((sp->_flag & _IOWRT) == 0)
		return EOF;
	c &= 0xff;
	/* if no init yet and not a no buff file */
	if (sp->_base == NULL && (sp->_flag & _IONBUF) == 0)
	{
		if ((sp->_ptr = sp->_base = malloc(BUFSIZ)) == NULL)
		{
			/* set to a no buff file */
			sp->_flag |= _IONBUF;
		} else
		{
			/* mark it as alloc'd */
			sp->_flag |= _IOABUF;
			/* do we handle newlines?   */
			if (isatty(fileno(sp)))
			{
				sp->_flag |= _IOLBUF;
			} else
			{
				/* lv room for 1st & last ch */
				sp->_cnt = BUFSIZ - 2;
				*sp->_ptr++ = c;
				return c;
			}
		}
	}
	if (sp->_flag & _IONBUF)			/* insure this pts ok       */
		sp->_ptr = sp->_base = &csave;	/* give buff a temp place   */
	*sp->_ptr++ = c;					/* put this somewhere       */
	if (sp->_flag & _IONBUF)			/* if a no buff file        */
	{
		/* write single char */
		ns = 1;
		n = write(fileno(sp), sp->_base, ns);
		/* enforce coming back again */
		sp->_cnt = 0;
	} else if (sp->_flag & _IOLBUF)
	{
		/* its a line buff file */
		if (c == '\n' || sp->_ptr >= sp->_base + BUFSIZ)
		{
			/* output the line */
			ns = (long)sp->_ptr - (long)sp->_base;
			n = write(fileno(sp), sp->_base, ns);
			/* reset the buffer */
			sp->_ptr = sp->_base;
		}
		/* enforce coming right back */
		sp->_cnt = 0;
	} else
	{
		/* o.w. we really have a full buffer */
		ns = (long)sp->_ptr - (long)sp->_base;
		n = write(fileno(sp), sp->_base, ns);
		/* lv room for last char */
		sp->_cnt = BUFSIZ - 1;
		/* init ptr */
		sp->_ptr = sp->_base;
	}
	/* error on write? */
	if (ns != n)
	{
		/* mark stream and die */
		sp->_flag |= _IOERR;
		return EOF;
	}
	return c;
}
