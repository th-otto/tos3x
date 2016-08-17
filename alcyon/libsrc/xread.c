#include <osif.h>
#include "lib.h"
#include <stddef.h>

long _pc_readblk(P(FD *) ccbp, P(long) offset, P(VOIDPTR) buff, P(long) bytes)
PP(FD *ccbp;)
PP(long offset;)
PP(VOIDPTR buff;)
PP(long bytes;)
{
	return jread(ccbp->dosfd, bytes, buff);
}
