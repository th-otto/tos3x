#include <osif.h>
#include "lib.h"
#include <stddef.h>

long _pc_writeblk(P(FD *) ccbp, P(long) offset, P(const VOIDPTR) buff, P(long) bytes)
PP(FD *ccbp;)
PP(long offset;)
PP(const VOIDPTR buff;)
PP(long bytes;)
{
	return Fwrite(ccbp->dosfd, bytes, buff);
}
