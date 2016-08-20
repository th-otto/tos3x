/***********************************************************************
*
*			f t e l l   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"ftell" returns the present value of the read/write pointer
*	within the stream.  This is a meaningless number for console
*	and list devices.
*
*	Calling sequence:
*		offset = ftell(sp)
*	Where:
*		sp -> (FILE *) stream
*		offset = (long) where stream is reading from/writing to
*
******************************************************************************/

#include "lib.h"
#include <unistd.h>

long ftell(P(FILE *) sp)
PP(register FILE *sp;)							/* stream pointer       */
{
	register long filepos;					/* file position        */
	register char *bp;						/* Buffer ptr           */
	long nread;							/* Temp var         */

	if (isatty(fileno(sp)))				/* are we talking to a tty? */
		return 0L;					/* quit now if so       */
	filepos = -1L;					/* default return value     */
	if (sp->_flag & (_IOREAD | _IOWRT))	/* if file is open      */
	{
		if ((filepos = lseek(fileno(sp), 0L, SEEK_CUR))	/* get where next byte read */
			== EOF)					/*  from or written to      */
			return -1L;			/*  quit if bad lseek       */
		if (sp->_base == NULL)			/* if file hasn't had i/o   */
			return filepos;				/*  return this position    */
		nread = (long)sp->_ptr - (long)sp->_base;	/* calc for # read/written  */
		filepos += nread;				/* correct for # read/wrtn  */
		if (sp->_flag & _IOREAD)		/* if reading from file     */
			if (filepos > 0)			/*  and we've read from file */
				filepos -= nread + sp->_cnt;	/* adjust file position     */
		/*  to reflect read ahead   */
		if (sp->_flag & _IOASCI)		/* ascii file? ************* */
		{								/* count the newlines       */
			if (sp->_flag & _IOWRT)		/* add in newline's cr's    */
			{							/*              */
				for (bp = sp->_base; bp < sp->_ptr; bp++)	/*         */
					if (*bp == '\n')	/* count newlines in stuff  */
						filepos++;		/*   written/read so far    */
			} else
			{							/* we're reading...     */
				if (filepos > 0)		/* check to see we've read  */
					for (bp = &(sp->_ptr[sp->_cnt - 1]);	/* start at end of buffer */
						 bp >= sp->_ptr; bp--)	/* back up to next read char */
						if (*bp == '\n')	/* count off for newlines   */
							filepos--;
			}
		}
	}
	return filepos;
}
