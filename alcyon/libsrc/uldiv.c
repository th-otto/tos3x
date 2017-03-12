#define NO_STDIO
#include "lib.h"

/* unsigned long divide */

long uldiv(P(long) al1, P(long) al2)
PP(long al1;)
PP(long al2;)
{
#ifdef __ALCYON__
	asm("move.l	d2,a0");
	asm("move.l	12(a6),d1");	/* d1 = divisor */
	asm("move.l	8(a6),d0");	/* d0 = dividend */

	asm("cmp.l	#$10000,d1"); /* divisor >= 2 ^ 16 ?   */
	asm("bcc	L3003");		/* then try next algorithm */
	asm("move.l	d0,d2");
	asm("clr.w	d2");
	asm("swap	d2");
	asm("divu	d1,d2");          /* high quotient in lower word */
	asm("move.w	d2,d0");		/* save high quotient */
	asm("swap	d0");
	asm("move.w	10(a6),d2");	/* get low dividend + high rest */
	asm("divu	d1,d2");		/* low quotient */
	asm("move.w	d2,d0");
	asm("bra	L3006");

asm("L3003:");
	asm("move.l	d1,d2");		/* use d2 as divisor backup */
asm("L3004:");
	asm("lsr.l	#1,d1");	/* shift divisor */
	asm("lsr.l	#1,d0");	/* shift dividend */
	asm("cmp.l	#$10000,d1"); /* still divisor >= 2 ^ 16 ?  */
	asm("bcc	L3004");
	asm("divu	d1,d0");		/* now we have 16-bit divisor */
	asm("and.l	#$ffff,d0"); /* mask out divisor, ignore remainder */

/* Multiply the 16-bit tentative quotient with the 32-bit divisor.  Because of
   the operand ranges, this might give a 33-bit product.  If this product is
   greater than the dividend, the tentative quotient was too large. */
	asm("move.l	d2,d1");
	asm("mulu	d0,d1");		/* low part, 32 bits */
	asm("swap	d2");
	asm("mulu	d0,d2");		/* high part, at most 17 bits */
	asm("swap	d2");		/* align high part with low part */
	asm("tst.w	d2");		/* high part 17 bits? */
	asm("bne	L3005");		/* if 17 bits, quotient was too large */
	asm("add.l	d2,d1");		/* add parts */
	asm("bcs	L3005");		/* if sum is 33 bits, quotient was too large */
	asm("cmp.l	8(a6),d1");	/* compare the sum with the dividend */
	asm("bls	L3006");		/* if sum > dividend, quotient was too large */
asm("L3005:");
	asm("subq.l	#1,d0");	/* adjust quotient */

asm("L3006:");
	asm("move.l	a0,d2");
#else
	(void) al1;
	(void) al2;
	__builtin_unreachable();
#endif
}
