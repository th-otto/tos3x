/*********************************************************************
*
*			f c l o s e   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fclose" flushes a stream (buffered file) and releases the
*	channel and any allocated buffers.
*
*	Calling sequence:
*		ret = fclose(stream)
*	Where:
*		stream -> file to be closed (FILE *)
*		ret = SUCCESS or FAILURE (if IO error)
*
*****************************************************************************/

#include "lib.h"
#include <stdlib.h>
#include <errno.h>

int fclose(P(FILE *) sp)
PP(register FILE *sp;)
{
	register int fd;
	
	if (!__validfp(sp))
	{
		__set_errno(EINVAL);
		return EOF;
	}
	if (sp->_flag & (_IOREAD | _IOWRT))	/* is it closeable?     */
	{									/* yup...           */
		fflush(sp);						/* do the flush         */
		if (sp->_flag & _IOABUF)		/* was buf alloc'd?     */
			free(sp->_base);			/* free it ifso         */
		sp->_base = sp->_ptr = NULL;	/* reset these          */
		sp->_cnt = 0;
	}
	/* reset all flags */
	sp->_flag = 0;
	/* and return */
	fd = fileno(sp);
	fileno(sp) = -1;
	return close(fd);
}
