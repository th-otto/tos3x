*
*	Setjmp / longjmp implementation.  See file "setjmp.h" for details.
*
	xdef	_setjmp			*	Set up longjump
	xdef	_longjmp		*	do longjump
*
_setjmp:				*	Save environment
	movea.l	4(a7),a0		*	a0 -> Environment buffer
	move.l	a7,(a0)+		*	Stack pointer to first long
	move.l	a6,(a0)+		*	Frame pointer to second long
	move.l	(a7),(a0)+		*	Return address to third long
	movem.l	d2-d7/a2-a5,(a0)	*	Save register variables
	clr.l	d0			*	Return 0 value
	rts				*	Return to caller
*
_longjmp:				*	Return to saved point
	movea.l	4(a7),a0		*	a0 -> Environment buffer
	move.w	8(a7),d0		*	Load return value
	ext.l	d0			*	just in case ...
	movea.l	(a0)+,a7		*	Reset stack
	movea.l	(a0)+,a6		*	Reset frame
	move.l	(a0)+,(a7)		*	Load TOS with return addr
	movem.l	(a0),d2-d7/a2-a5	*	Restore register variables
	rts				*	and return
