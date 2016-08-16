/* NOASCII: pulls out as much ASCII disk i/o as possible. */

#include <osif.h>
#include "lib.h"

/* stubroutine 'tag'	    */
VOID noascii(NOTHING) { ; }


const char * __noascii_io_msg = "ascii disk i/o rtns";


size_t _wrtasc(P(FD *) fp, P(const VOIDPTR) buff, P(size_t) bytes)
PP(FD * fp;)
PP(const VOIDPTR buff;)
PP(size_t bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__noascii_io_msg);
	return -1;
}


size_t _rdasc(P(FD *) fp, P(VOIDPTR) buff, P(long) bytes)
PP(FD * fp;)
PP(VOIDPTR buff;)
PP(long bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__noascii_io_msg);
	return -1;
}
