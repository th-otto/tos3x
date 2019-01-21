#include <compiler.h>
#include <mint/arch/nf_ops.h>
#include <sys/types.h>
#include <stdarg.h>

static long _nf_get_id(NOTHING);
static long _nf_call(NOTHING);

static long _nf_det(NOTHING)
{
asm("__nf_get_id:");
	asm("dc.w $7300");
	asm("rts");

asm("__nf_call:");
	asm("dc.w $7301");
	asm("rts");
}


long nf_get_id(P(const char *) feature_name)
PP(const char *feature_name;)
{
	return _nf_get_id(feature_name);
}

/*** ---------------------------------------------------------------------- ***/

extern VOIDPTR dskbufp;

int nf_dbgvprintf(P(const char *) format, P(va_list) args)
PP(const char *format;)
PP(va_list args;)
{
	long nfid_stderr;
	int ret;
	
	if ((nfid_stderr = _nf_get_id(NF_ID_STDERR)) == 0)
	{
		return -1;
	}	
	{
		ret = vsprintf(dskbufp, format, args);
		ret = _nf_call(nfid_stderr | 0, (__uint32_t)dskbufp);
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
