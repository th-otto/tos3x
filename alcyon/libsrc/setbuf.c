/* setbuf - assign a buffer to a stream, after open but
	before any i/o. 
	Returns SUCCESS if ok, FAILURE o.w. */

#include "lib.h"

int setbuf(P(FILE *) sp, P(char *) buf)
PP(register FILE *sp;)
PP(register char *buf;)
{
	/* The ANSI standard says set(v)buf can only be called before any I/O is done */
	if (sp->_base != NULL)
		return EOF;
	sp->_base = sp->_ptr = buf;
	if (buf == NULL)
		sp->_flag |= _IONBUF;
	else
		sp->_flag &= ~_IONBUF;
	return 0;
}
