*
*	Block Fill function:
*
*	memset(dest,char,cnt);
*
*	BYTE	*dest;		/* -> area to be filled	*/
*	BYTE	char;		/* =  char to fill	*/
*	WORD	cnt;		/* =  # bytes to fill   */
*
	xdef	_memset
_memset:
	move.l	4(a7),a0			*	-> Output area
	move.l  a0,a1               * return value
	move.w	8(a7),d1			*	=  output char
	moveq #0,d0
	move.w	10(a7),d0			*	=  output count
	beq	filldone			*	Done if le
	subq.l	#1,d0				*	decrement
fillit:	move.b	d1,(a0)+			*	move a byte
	dbra	d0,fillit			*	Continue
filldone: move.l a1,d0
	rts
