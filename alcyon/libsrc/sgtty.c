/*************************************************************************
*
*			stty / gtty   Functions
*			-----------------------
*	Copyright 1983 by Digital Reserach Inc.  All rights reserved.
*
*	"stty(fd,argp)" sets the bits for CON: file opened on 'fd'
*	"gtty(fd,argp)" gets the bits for CON: file opened on 'fd'
*
*	Where:
*		fd  = file descriptor returned by 'open'
*		argp pts to struct 'sgttyb' (see "sgtty.h")
*
**************************************************************************/

#include <osif.h>
#include "lib.h"
#include <sgtty.h>

int stty(P(int) fd, P(const struct sgttyb *) argp)
PP(int fd;)								/* opened file descriptor */
PP(const struct sgttyb *argp;)					/* ptr to control info */
{
	register FD *fp;

	if ((fp = _chkc(fd)) == NULLFD || (fp->flags & ISTTY) == 0)
		return -1;
	/* no checks: do whatever */
	blkmove((char *)&(fp->fcb), (const char *)argp, sizeof(*argp));
	return 0;
}


int gtty(P(int) fd, P(struct sgttyb *) argp)
PP(int fd;)								/* opened file descriptor */
PP(struct sgttyb *argp;)					/* ptr to control info */
{
	register FD *fp;

	if ((fp = _chkc(fd)) == NULLFD || (fp->flags & ISTTY) == 0)
		return -1;
	/* no checks: do whatever */
	blkmove((char *)argp, (char *)&(fp->fcb), sizeof(*argp));
	return 0;
}
