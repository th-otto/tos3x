#include "lib.h"
#include <errno.h>
#include <string.h>

#define NUMERRS 37
static char const _undeferr[] = "Error undefined";

int sys_nerr = NUMERRS;

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


const char *strerror(P(int) err_no)
PP(int err_no;)
{
	register const char *err;

	if (err_no < 0 || err_no >= sys_nerr)
		return _undeferr;
	return sys_errlist[err_no];
}
