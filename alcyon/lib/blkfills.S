*
*	Block Fill function:
*
*	blkfill(dest,char,cnt);
*
*	BYTE	*dest;		/* -> area to be filled	*/
*	BYTE	char;		/* =  char to fill	*/
*	WORD	cnt;		/* =  # bytes to fill   */
*
	xdef	_blkfill
_blkfill:
	move.l	4(a7),a0			*	-> Output area
	move.w	8(a7),d1			*	=  output char
	moveq #0,d0
	move.w	10(a7),d0			*	=  output count
	beq	filldone			*	Done if le
	subq.l	#1,d0				*	decrement
fillit:	move.b	d1,(a0)+			*	move a byte
	dbra	d0,fillit			*	Continue
filldone: clr.l	d0				*	always return 0
	rts
