#define NO_STDIO
#include "lib.h"

/* unsigned long remainder */

long ulrem(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
#if 0
    asm("move.l   12(a6),d1");  /* d1 = divisor */
    asm("move.l   8(a6),d0");  /* d0 = dividend */
    asm("move.l   d1,-(a7)");
    asm("move.l   d0,-(a7)");
    asm("jsr      _uldiv");
    asm("addq.l   #8,sp");
    asm("move.l   12(a6),d1");  /* d1 = divisor */
    asm("move.l   d1,-(a7)");
    asm("move.l   d0,-(a7)");
    asm("jsr      _lmul");  /* d0 = (a/b)*b */
    asm("addq.l   #8,sp");
    asm("move.l   8(a6),d1");  /* d1 = dividend */
    asm("sub.l    d0,d1");      /* d1 = a - (a/b)*b */
    asm("move.l   d1,d0");
#else
	return al1 - lmul(uldiv(al1, al2), al2);
#endif
}
