#include "lib.h"
#include <errno.h>
#include <string.h>

#define NUMERRS 37
static char const _undeferr[] = "Error undefined";

int sys_nerr = NUMERRS;

const char *const sys_errlist[NUMERRS] = {
	"no error",							/* 0 */
	"Operation not permitted",			/* EPERM */
	"No such file or directory",		/* ENOENT */
	"No such process",					/* ESRCH */
	"Interrupted system call",			/* EINTR */
	"I/O error",						/* EIO */
	"No such device ord address",		/* ENXIO */
	"Argument list too long",			/* E2BIG */
	"Exec format error",				/* ENOEXEC */
	"Bad file number",					/* EBADF */
	"No child processes",				/* ECHILD */
	"Resource temporarily unavailable",	/* EAGAIN */
	"Cannot allocate memory",			/* ENOMEM */
	"Permission denied",				/* EACCES */
	"Bad address",						/* EFAULT */
	"Block device required",			/* ENOTBLK */
	"Device or resource busy",			/* EBUSY */
	"File exists",						/* EEXIST */
	"Invalid cross-device link",		/* EXDEV */
	"No such device",					/* ENODEV */
	"Not a directory",					/* ENOTDIR */
	"Is a directory",					/* EISDIR */
	"Invalid argument",					/* EINVAL */
	"Too many open files in system",	/* ENFILE */
	"Too many open files",				/* EMFILE */
	"Inappropriate ioctl for device",	/* ENOTTY */
	"Text file busy",					/* ETXTBSY */
	"File too large",					/* EFBIG */
	"No space left on device",			/* ENOSPC */
	"Illegal seek",						/* ESPIPE */
	"Read-only file system",			/* EROFS */
	"Too many links",					/* EMLINK */
	"Broken pipe",						/* EPIPE */
	"Numerical argument out of domain",	/* EDOM */
	"Numerical result out of range",	/* ERANGE */
/* end of UNIX v7, start of CPM specific      */
	"No directory space",				/* ENODSPC */
	"Can't rename file"					/* ERENAME */
};


const char *strerror(P(int) err_no)
PP(int err_no;)
{
	register const char *err;

	if (err_no < 0 || err_no >= sys_nerr)
		return _undeferr;
	return sys_errlist[err_no];
}
