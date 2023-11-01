/*************************************************************************
*
*			a c c e s s   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"access" returns 0 if access to a file is allowed, -1 o.w.
*	Under CP/M, this just tests for existence.
*
*	Calling sequence:
*		ret = access(fname,mode)
*	Where:
*		ret = 0 if accessable, -1 o.w.
*		fname -> file's name, NULL terminated string
*		mode = test for read, write, exec, dir path access
*			(ignored by CP/M)
*
*	Made O/S independent 20-Aug-83 sw
*	Added chmod & chown to this source module 16-Sep-83 whf
*
**************************************************************************/

#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <fcntl.h>

int access(P(const char *) fname, P(int) mode)
PP(const char *fname;)
PP(int mode;)
{
	register int rval;

	/* BUG: mode not evaluated here */
	/* BUG: existing does mean we are able to open it */
	UNUSED(mode);
	if ((rval = open(fname, O_RDONLY)) >= 0)	/* File must be there ... */
	{
		close(rval);					/* Free up fd         */
		return 0;
	}
	RETERR(-1, ENOENT);
}
