********************************************************************
*
*	CP/M abort function for C Run Time Library
*
*	"abort" generates an Illegal Instruction Trap to recall DDT
*	(if it is loaded).
*
*********************************************************************
illegal equ $4AFC
	xdef _abort
	text
_abort:	
	move.w	4(sp),d0
a:	dc.w	illegal
	bra.b	a		*	For Wise Guys
