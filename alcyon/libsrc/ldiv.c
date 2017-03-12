#define NO_STDIO
#include "lib.h"

long ldiv(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
#ifdef __ALCYON__
	asm("  xdef ldiv");
	asm("ldiv equ _ldiv");
#endif
#ifdef __ALCYON__
	asm("move.l   d2,-(a7)");
	
	asm("moveq    #1,d2"); /* sign of result stored in d2 (=1 or =-1) */
	asm("move.l   12(a6),d1"); /* d1 = divisor */
	asm("bpl L4001");
	asm("neg.l    d1");
	asm("neg.b    d2");      /* change sign because divisor <0  */
asm("L4001:");
	asm("move.l   8(a6),d0");  /* d0 = dividend */
	asm("bpl L4002");
	asm("neg.l    d0");
	asm("neg.b    d2");
	
asm("L4002:");
	asm("move.l   d1,-(a7)");
	asm("move.l   d0,-(a7)");
	asm("jsr _uldiv");  /* divide abs(dividend) by abs(divisor) */
	asm("addq.l   #8,sp");
	
	asm("tst.b    d2");
	asm("bpl L4003");
	asm("neg.l    d0");
	
asm("L4003:");
	asm("move.l   (a7)+,d2");
#else
	register int neg = 0;
	register long res;

	if (al1 < 0)
	{
		al1 = -al1;
		neg = !neg;
	}

	if (al2 < 0)
	{
		al2 = -al2;
		neg = !neg;
	}

	res = uldiv(al1, al2);
	if (neg)
		res = -res;

	return res;
#endif
}
