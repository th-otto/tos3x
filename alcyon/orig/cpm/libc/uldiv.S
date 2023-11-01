*#include <portab.h>
*long uldivr = 0;
*
*
*long uldiv(al1,al2)	/* unsigned long divide */
*long al1,al2;
*{
*
*#ifdef ALCYON
*	register long l1,l2;	
*	/* you must modify comparisons in the assembler file
*	 * for this routine to work.
*	 */
*#else
*	register unsigned long l1, l2;
*#endif
*	register long q, b;
*
*	l1 = al1;
*	l2 = al2;
*	if(l2==0) {
*		uldivr = 0x80000000;
*		return(0x80000000);
*	}
*	if(l2 > l1) {				/** comparison (ble->bls) **/
*		uldivr = l1;
*		return(0);
*	}
*	if(l1==l2) {
*		q = 1;
*		l1 = 0;
*		goto doret;
*	}
*	b = 1;					/* bit value */
*	while(l1>=l2) {				/** comparison (blt->blo) **/
*		if( l2 > (l2<<1) )		/** comparison (bgt->bhi) **/
*			break;			/* detect overflow */
*		l2 =<< 1;
*		b =<< 1;
*	}
*	q = 0;
*	while(b) {		/* now do shifts and subtracts */
*		if(l1>=l2) {			/** comparison (blt->blo) **/
*			q =| b;
*			l1 =- l2;
*		}
*		b =>> 1;
*		l2 =>> 1;
*	}
*doret:
*	uldivr = l1;
*	return(q);
*}
*
*
	xdef _uldivr
	data
_uldivr:
	dc.l 0
	xdef _uldiv
	text
_uldiv:
*~b=d4
*~q=d5
*~l1=d7
*~l2=d6
*~al1=8
*~al2=12
	link a6,#0
	movem.l d3-d7,-(sp)
	move.l 8(a6),d7
	move.l 12(a6),d6
	tst.l d6
	bne L2
	move.l #$80000000,_uldivr
	move.l #$80000000,d0
	divu.w #0,d0
	bra L1
L2:
	cmp.l d7,d6
	bls L3			*<<<<< ble
	move.l d7,_uldivr
	clr.l d0
	bra L1
L3:
	cmp.l d6,d7
	bne L4
	move.l #1,d5
	clr.l d7
	bra L5
L4:
	move.l #1,d4
L7:
	cmp.l d6,d7
	blo L6			*<<<< blt
	move.l d6,d0
	asl.l #1,d0
	cmp.l d0,d6
	bhi L6			*<<<< bgt
	asl.l #1,d6
	asl.l #1,d4
	bra L7
L6:
	clr.l d5
L9:
	tst.l d4
	beq L8
	cmp.l d6,d7
	blo L10			*<<<< blt
	or.l d4,d5
	sub.l d6,d7
L10:
	lsr.l #1,d4		*<<<< asr
	lsr.l #1,d6		*<<<< asr
	bra L9
L8:
L5:
	move.l d7,_uldivr
	move.l d5,d0
*	bra L1
L1:
	tst.l (sp)+
	movem.l (sp)+,d4-d7
	unlk a6
	rts
