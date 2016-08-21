******************************************************************************
*
*	Assembly language interface to signal processing in C.
*
*	Majority of work is done by "signal.c".  All we do is take the
*	exception and call the proper C function.  Registers, condition
*	codes, etc. are all preserved.  A return from the function causes
*	resumption of normal processing.
*
******************************************************************************

	xdef	__illinst		*	Illegal instruction EPA

	xdef	__trace			*	Trace Trap 	    EPA
	xdef	__trap			*	All trap instructions
	xdef	__buserr		*	BUSERR, addressing  EPA
	xdef	__arith			*	Arithmetic traps    EPA
	comm	___signal,64		*	-> epa's of C functions

	text
__illinst:				*
	move.l a0,-(a7)
	move.l	___signal+16,a0	*	Set up epa
	add.l	#2,2(a7)		*	Bump PC to next inst
	bra.s	process			*	Process exception
__trace:
	move.l a0,-(a7)
	move.l	___signal+20,a0	*	Set up epa
	bra.s	process			*	process

__trap:					*
	move.l a0,-(a7)
	move.l	___signal+24,a0 *	Select epa
	bra.s	process			*

__buserr:				*	Here for buserr crap
	move.l a0,-(a7)
	move.l	___signal+40,a0	*	move in epa
	bra.s	process			*	process exception

__arith:				*	
	move.l a0,-(a7)
	move.l	___signal+32,a0	*	move in epa
process:				*	Here to process exception
	movem.l	d0-d7/a1-a6,-(sp)	*	Save all registers
	suba.l    a6,a6			*	Clear out a6
	jsr	(a0)			*	Call processing routine
	movem.l	(sp)+,d0-d7/a1-a6	*	restore registers
	move.l (a7)+,a0
	rte
