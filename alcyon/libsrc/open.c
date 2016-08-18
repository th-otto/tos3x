/****************************************************************************
*
*		   C   F I L E   O P E N   R O U T I N E
*		   -------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	The "open" routine opens a "C" file and returns a file id. 
*	Comes in 3 flavors, one for ascii, one for binary, and the default
*	(currently to ascii).
*
*	Calling Sequence:
*		fid = open(fname,mode)
*		fid = opena(fname,mode)
*		fid = openb(fname,mode)
*		fid = _open(fname,mode,type);
*
*	Where:
*		fname	is the address of a null terminated file name.
*		mode	is the open mode:
*				0 => READ access only
*				1 => WRITE access only
*				2 => Both READ and WRITE
*		type	is 0 for ASCII files, 1 for BINARY
*
*
*****************************************************************************/
#include "lib.h"
#include <osif.h>
#include <osiferr.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>


int open(P(const char *) fname, P(int) flags  _va_alist)
PP(const char *fname;)						/* -> File name         */
PP(int flags;)								/* Open mode            */
_va_dcl
{
	register int ich;						/* Channel number for open  */
	register FD *ch;						/* -> CCB for channel       */
	mode_t mode;
	va_list args;
	int ret;
	
	/* Allocate a channel */
	if ((ich = _allocc()) == -1)
		return -1;
	
	/* Clear out channel's ccb */
	__chinit(ich);
	/* Get address of ccb */
	ch = _getccb(ich);
	
	/* If read only, set READONLY bit */
	if (flags == O_RDONLY)
		ch->flags |= ISREAD;
	/* Is ASCII file? */
	if (flags & O_TEXT)
		ch->flags |= ISASCII;
	
	/* if a terminal, mark as tty */
	if (strcasecmp(fname, __tname) == 0)
	{
		ch->flags |= ISTTY | OPENED;
		ch->dosfd = flags; /* ??? */
	} else if (strcasecmp(fname, __lname) == 0)
	{
		/* Mark as printer */
		ch->flags |= ISLPT | OPENED;
	} else
	{
		/* Use POS SVC interface */
		if (flags & O_CREAT)
		{
			_va_start(args, flags);
			mode = _va_arg(args, mode_t);
			_va_end(args);
			if (!(flags & O_EXCL))
			{
				if (flags & O_TRUNC)
					ret = __open(ich, fname, CREATE);
				else
					ret = __open(ich, fname, OPEN);
			} else
			{
				ret = __open(ich, fname, OPEN);
				if (ret == 0)
				{
					close(ch->chan);
					RETERR(-1, EEXIST);
				} else if (flags & O_TRUNC)
				{
					ret = __open(ich, fname, CREATE);
				}
			}
			if (ret == 0)
				chmod(fname, mode);
		} else
		{
			ret = __open(ich, fname, OPEN);
		}
		if (ret != 0)
		{
			/* deallocate channel */
			_freec(ich);
			RETERR(-1, ENOENT);
		}
		
		/* Set OPEN bit */
		ch->flags |= OPENED;

		/* Kludge to set hi water mark */
		lseek(ch->chan, 0L, SEEK_END);

		if (!(flags & O_APPEND))
			lseek(ch->chan, 0L, SEEK_SET);
	}
	return ich;
}
