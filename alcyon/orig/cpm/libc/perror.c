/***********************************************************************
*
*			p e r r o r   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"perror" produces a short error message on stderr describing
*	the last error encountered during a call to the system.
*	It assumes the variable "errno" contains this error.
*	Note: "errno" is set when errors occur, but is not cleared.
*	Note also that some of the UNIX errors are meaningless under 
*	CP/M, and will print an undefined message (defined below).
*	Finally, note that the defines in """errno.h""" will index 
*	appropriately into the sys_errlist table, below.
*
*	Calling sequence:
*		ret = perror(s)
*	Where:
*		ret = errno
*		s   -> a string containing a message printed with
*			the explanatory error message.
*
**************************************************************************/
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <string.h>

#define NUMERRS 37
static char const _undeferr[] = "Error undefined";

int sys_nerr = NUMERRS;

extern const char *const sys_errlist[NUMERRS];

static char *_itoa PROTO((int nm, char *bp));

int perror(P(const char *) str)
PP(const char *str;)
{
	register const char *err;
	char lbuf[20];
	char *buf;

	if (errno < 0 || errno >= sys_nerr)
		err = _undeferr;
	else
		err = sys_errlist[errno];
	/* BUG: using write() here badly interferes with buffered output on stderr */
	write(STDERR, str, strlen(str));
	write(STDERR, ": ", 2);
	write(STDERR, err, strlen(err));
	write(STDERR, " (", 2);
	buf = _itoa(errno, lbuf);
	if (_errcpm != 0)					/* assume this had something to do */
	{
		*buf++ = '.';
		buf = _itoa(_errcpm, buf);
	}
	*buf++ = ')';
	*buf++ = '\n';
	*buf = '\0';
	write(STDERR, lbuf, strlen(lbuf));
	return errno;
}


static char *_itoa(P(int) nm, P(char *) bp)
PP(int nm;)
PP(register char *bp;)
{
	/* temporary... */
	sprintf(bp, "%d", nm);
	while (*bp)
		bp++;
	return bp;
}

const char *const sys_errlist[NUMERRS] = {
	_undeferr,							/* 0 */
	_undeferr,							/* 1 */
	"ENOENT No such file",				/* 2 */
	_undeferr,							/* 3 */
	_undeferr,							/* 4 */
	"EIO I/O error",					/* 5 */
	_undeferr,							/* 6 */
	"E2BIG Arg list too long",			/* 7 */
	_undeferr,							/* 8 */
	"EBADF Bad file number",			/* 9 */
	_undeferr,							/* 10 */
	_undeferr,							/* 11 */
	"ENOMEM Not enough core",			/* 12 */
	"EACCES Permission denied",			/* 13 */
	_undeferr,							/* 14 */
	_undeferr,							/* 15 */
	_undeferr,							/* 16 */
	_undeferr,							/* 17 */
	_undeferr,							/* 18 */
	_undeferr,							/* 19 */
	_undeferr,							/* 20 */
	_undeferr,							/* 21 */
	"EINVAL Invalid argument",			/* 22 */
	"ENFILE File table overflow",		/* 23 */
	"EMFILE Too many open files",		/* 24 */
	"ENOTTY Not a typewriter",			/* 25 */
	_undeferr,							/* 26 */
	"EFBIG File too big",				/* 27 */
	"ENOSPC No space left on device",	/* 28 */
	_undeferr,							/* 29 */
	"EROFS Read-only file system",		/* 30 */
	_undeferr,							/* 31 */
	_undeferr,							/* 32 */
	_undeferr,							/* 33 */
	_undeferr,							/* 34 */
/* end of UNIX v7, start of CPM specific      */
	"ENODSPC No directory space",		/* 35 */
	"ERENAME Can't rename file"			/* 36 */
};

