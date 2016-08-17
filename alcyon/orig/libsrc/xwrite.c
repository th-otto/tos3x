#include <osif.h>
#include "lib.h"
#include <stddef.h>

size_t _pc_writeblk(P(struct fcbtab *) fcb, P(long) offset, P(const VOIDPTR) buff, P(size_t) bytes, P(size_t) secsiz)
PP(struct fcbtab *fcb;)
PP(long offset;)
PP(const VOIDPTR buff;)
PP(size_t bytes;)
PP(size_t secsiz;)
{
	/* ugly hack: fetch the dos handle from ccb struct */
	int dosfd;
	/* even more ugly because the offsetof macro does not work for alcyon */
#ifdef __ALCYON__
	dosfd = ((struct ccb *)(((char *)fcb) - 18))->dosfd;
#else
	dosfd = ((struct ccb *)(((char *)fcb) - offsetof(struct ccb, fcb)))->dosfd;
#endif
	return jwrite(dosfd, (long) (int) bytes * (long) (int) secsiz, buff);
}
