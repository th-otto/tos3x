	xdef	_memcpy
_memcpy:
	move.l	4(a7),a0			*	-> dst
	move.l	8(a7),a1			*	-> src
	move.l  a0,d0               * return value
	moveq #0,d1
	move.w	12(a7),d1			* count
	beq	copydone			*	Done if le
	subq.l	#1,d1				*	decrement
copyit:	move.b	(a1)+,(a0)+			*	move a byte
	dbra	d1,copyit			*	Continue
copydone:
	rts
