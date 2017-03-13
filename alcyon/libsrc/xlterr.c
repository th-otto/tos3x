#include "lib.h"
#include <errno.h>
#include <string.h>
#include <toserrno.h>

static char const xlttbl[] = {
	EINVAL,     /* E_INVFN */
	ENOENT,     /* E_FILNF */
	ENOTDIR,    /* E_PTHNF */
	EMFILE,     /* E_NHNDL */
	EACCES,     /* E_ACCDN */
	EBADF,      /* E_IHNDL */
	EIO,        /* -38 */
	ENOMEM,     /* E_NSMEM */
	EFAULT,     /* E_IMBA */
	EIO,        /* -41 */
	EIO,        /* -42 */
	EIO,        /* -43 */
	EIO,        /* -44 */
	EIO,        /* -45 */
	ENODEV,     /* E_DRIVE */
	EIO,        /* -47 */
	EXDEV,      /* E_NSAME */
	ENOENT,     /* E_NMFIL */
	EIO,        /* -50 */
	EIO,        /* -51 */
	EIO,        /* -52 */
	EIO,        /* -53 */
	EIO,        /* -54 */
	EIO,        /* -55 */
	EIO,        /* -56 */
	EIO,        /* -57 */
	EIO,        /* E_LOCKED */
	EIO,        /* E_NSLOCK */
	EIO,        /* -60 */
	EIO,        /* -61 */
	EIO,        /* -62 */
	EIO,        /* -63 */
	ESPIPE,     /* E_BADARG */
	EIO,        /* E_INTRN */
	ENOEXEC,    /* E_PLFMT */
	ENOMEM      /* E_GSBF */
};

int _XltErr(P(int) err, P(int) dflt)
PP(int err;)
PP(int dflt;)
{
	register unsigned int err_no;

	err_no = -err + E_INVFN;
	if (err_no < (sizeof(xlttbl) / sizeof(xlttbl[0])))
		return xlttbl[err_no];
	return dflt;
}
