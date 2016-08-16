/*************************************************************************
*
*			Channel Info Functions
*			----------------------
*	Copyright 1982,83 by Digital Reserach Inc.  All rights reserved.
*
*	"isatty(fd)" == TRUE iff the file on channel fd is a 'CON:' device.
*	"isdev(fd)" == TRUE iff fd is 'CON:', 'LST:', 'AUX:'
*
*	Where:
*		fd  = file descriptor returned by 'open'
*
**************************************************************************/

#include <osif.h>
#include "lib.h"
#include <unistd.h>


int isatty(P(int) fd)
PP(int fd;)
{
	register FD *fp;

	if ((fp = _chkc(fd)) == NULLFD)		/* make sure its open  MGL  */
		return 0;					/*   isno TTY ifnot open    */
	return ((fp->flags & ISTTY) != 0);	/* test this flag       */
}
