#include "lib.h"
#include <mint/arch/nf_ops.h>
#include <sys/types.h>
#include <stdarg.h>
#include <errno.h>

/*** ---------------------------------------------------------------------- ***/

int nf_dbgvprintf(P(const char *) format, P(va_list) args)
PP(const char *format;)
PP(va_list args;)
{
	struct nf_ops *nf_ops;
	long nfid_stderr;
	int ret;
	
	if ((nf_ops = nf_init()) == NULL ||
		(nfid_stderr = NF_GET_ID(nf_ops, NF_ID_STDERR)) == 0)
	{
#ifdef ENOSYS
		errno = ENOSYS;
#else
		errno = EIO;
#endif
		return -1;
	}	
	{
		char buf[2048];
		
		ret = vsprintf(buf, format, args);
		ret = (*nf_ops->call)(nfid_stderr | 0, (__uint32_t)virt_to_phys(buf));
	}

	return ret;
}

/*** ---------------------------------------------------------------------- ***/

int nf_dbgprintf(P(const char *) format _va_alist)
PP(const char *format;)
_va_dcl
{
	int ret;
	va_list args;
	
	_va_start(args, format);
	ret = nf_dbgvprintf(format, args);
	_va_end(args);
	return ret;
}
