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
#include "../libsrc/lib.h"
#include <osif.h>
#include <osiferr.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>


int _open(P(const char *) fname, P(int) mode, P(int) binary)
PP(const char *fname;)						/* -> File name         */
PP(int mode;)								/* Open mode            */
PP(int binary;)								/* File type            */
{
	register int ich;						/* Channel number for open  */
	register FD *ch;						/* -> CCB for channel       */

	/* Allocate a channel */
	if ((ich = _allocc()) == -1)
		return -1;
	
	/* Clear out channel's ccb */
	__chinit(ich);
	/* Get address of ccb */
	ch = _getccb(ich);
	
	/* If read only, set READONLY bit */
	if (mode == O_RDONLY)
		ch->flags |= ISREAD;
	/* Is ASCII file? */
	if (binary == 0)
		ch->flags |= ISASCII;
	
	/* if a terminal, mark as tty */
	if (strcasecmp(fname, __tname) == 0)
	{
		ch->flags |= ISTTY | OPENED;
		ch->dosfd = mode; /* BUG: ugly hack; the call here is from xmain, and mode is either 0 or 1 */
		return ich;
	} else if (strcasecmp(fname, __lname) == 0)
	{
		/* Mark as printer */
		ch->flags |= ISLPT | OPENED;
		return ich;
	}

	/* Use POS SVC interface */
	if (__open(ich, fname, OPEN, mode) != 0)
	{
		/* deallocate channel */
		_freec(ich);
		RETERR(-1, ENOENT);
	}
	
	/* Set OPEN bit */
	ch->flags |= OPENED;
	/* Kludge to set hi water mark */
	lseek(ch->chan, 0L, SEEK_END);
	lseek(ch->chan, 0L, SEEK_SET);
	return ich;
}


int open(P(const char *) fname, P(int) mode _va_alist)
PP(const char *fname;)
PP(int mode;)
_va_dcl
{
	return _open(fname, mode, 0);
}


int opena(P(const char *) fname, P(int) mode _va_alist)
PP(const char *fname;)
PP(int mode;)
_va_dcl
{
	return _open(fname, mode, 0);
}


int openb(P(const char *) fname, P(int) mode _va_alist)
PP(const char *fname;)
PP(int mode;)
_va_dcl
{
	return _open(fname, mode, 1);
}
