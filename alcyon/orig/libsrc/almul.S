	text
	xdef	almul
	xref	lmul
*
*	/ this routine multiplies together two 32 bit signed long integers.
*	/ the first arg is a pointer to the long, which is multiplied by the
*	/ second arg, with the result being stored at the firstarg.
*	/ lmul.s does the meat of the work
*
almul:
	move.l	a5,-(sp)
	move.l	8(sp),a5
	move.l	12(sp),-(sp)
	move.l	(a5),-(sp)
	jsr		lmul
	add		#8,sp
	move.l	d0,(a5)
	move.l	(sp)+,a5
	rts
