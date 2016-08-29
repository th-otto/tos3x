#include "lib.h"

/* unsigned long divide */

long uldivr;

long uldiv(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
#if 0
	register unsigned long l1, l2;
	register long q, b;

	l1 = al1;
	l2 = al2;
	if (l2 == 0)
	{
		uldivr = 0x80000000;
		asm("divu.w #0,d0");
		return 0x80000000;
	}
	if (l2 > l1)
	{							/** comparison (ble->bls) **/
		uldivr = l1;
		return 0;
	}
	if (l1 == l2)
	{
		q = 1;
		l1 = 0;
		goto doret;
	}
	b = 1;								/* bit value */
	while (l1 >= l2)
	{							/** comparison (blt->blo) **/
		if (l2 > (l2 << 1))		/** comparison (bgt->bhi) **/
			break;						/* detect overflow */
		l2 <<= 1;
		b <<= 1;
	}
	q = 0;
	while (b)
	{									/* now do shifts and subtracts */
		if (l1 >= l2)
		{						/** comparison (blt->blo) **/
			q |= b;
			l1 -= l2;
		}
		b >>= 1;
		l2 >>= 1;
	}
  doret:
	uldivr = l1;
	return q;
#else
asm("   movem.l d4-d7,-(sp)");
asm("   move.l 8(a6),d7");
asm("   move.l 12(a6),d6");
asm("   tst.l d6");
asm("   bne L2");
asm("   move.l #$80000000,_uldivr");
asm("   move.l #$80000000,d0");
asm("   divu.w #0,d0");
asm("   bra L99");
asm("L2:");
asm("   cmp.l d7,d6");
asm("   bls L3"); /*          *<<<<< ble */
asm("   move.l d7,_uldivr");
asm("   clr.l d0");
asm("   bra L99");
asm("L3:");
asm("   cmp.l d6,d7");
asm("   bne L4");
asm("   move.l #1,d5");
asm("   clr.l d7");
asm("   bra L5");
asm("L4:");
asm("   move.l #1,d4");
asm("L7:");
asm("   cmp.l d6,d7");
asm("   blo L6"); /*          *<<<< blt */
asm("   move.l d6,d0");
asm("   asl.l #1,d0");
asm("   cmp.l d0,d6");
asm("   bhi L6"); /*          *<<<< bgt */
asm("   asl.l #1,d6");
asm("   asl.l #1,d4");
asm("   bra L7");
asm("L6:");
asm("   clr.l d5");
asm("L9:");
asm("   tst.l d4");
asm("   beq L8");
asm("   cmp.l d6,d7");
asm("   blo L10"); /*          *<<<< blt */
asm("   or.l d4,d5");
asm("   sub.l d6,d7");
asm("L10:");
asm("   lsr.l #1,d4"); /*       *<<<< asr */
asm("   lsr.l #1,d6"); /*       *<<<< asr */
asm("   bra L9");
asm("L8:");
asm("L5:");
asm("   move.l d7,_uldivr");
asm("   move.l d5,d0");
asm("L99:");
asm("   movem.l (sp)+,d4-d7");
#endif
#ifndef __ALCYON__
	return 0;
#endif
}
