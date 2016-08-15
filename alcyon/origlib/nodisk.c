/* NODISK: pulls out as much disk i/o as possible. */

#include <osif.h>
#include "lib.h"

/* stubroutine 'tag' */
VOID nodisk(NOTHING) { ; }


char * __nodisk_io_msg = "disk i/o rtns";

size_t _rdasc(P(FD *, fp), P(VOIDPTR, buff), P(long, bytes))
PP(FD *, fp;)
PP(VOIDPTR, buff;)
PP(long, bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nodisk_io_msg);
	return -1;
}

size_t _rdbin(P(FD *, fp), P(VOIDPTR, buff), P(long, bytes))
PP(FD *, fp;)
PP(VOIDPTR, buff;)
PP(long, bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nodisk_io_msg);
	return -1;
}


size_t _wrtasc(P(FD *, fp), P(const VOIDPTR, buff), P(long, bytes))
PP(FD *, fp;)
PP(const VOIDPTR, buff;)
PP(long, bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nodisk_io_msg);
	return -1;
}


size_t _wrtbin(P(FD *, fp), P(const VOIDPTR, buff), P(long, bytes))
PP(FD *, fp;)
PP(const VOIDPTR, buff;)
PP(long, bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nodisk_io_msg);
	return -1;
}

