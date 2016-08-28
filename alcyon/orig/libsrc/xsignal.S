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
	.comm	___signal,64		*	-> epa's of C functions

	bss
epa:	ds.l	1			*		non-reentrant!!
	text
__illinst:				*
	move.l	___signal+16,epa	*	Set up epa
	add.l	#2,2(a7)		*	Bump PC to next inst
	bra	process			*	Process exception
__trace:
	move.l	___signal+20,epa	*	Set up epa
	bra	process			*	process

__trap:					*
	move.l	___signal+24,epa	*	Select epa
	bra	process			*

__buserr:				*	Here for buserr crap
	cmpm.l	(a7)+,(a7)+		*	Prune junk
	move.l	___signal+40,epa	*	move in epa
	bra	process			*	process exception

__arith:				*	
	move.l	___signal+32,epa	*	move in epa
process:				*	Here to process exception
	movem.l	d0-d7/a0-a6,-(sp)	*	Save all registers
	move.l	epa,a0			*	Load entry point
	suba.l    a6,a6			*	Clear out a6
* BUG: signal number not passed to user
	jsr	(a0)			*	Call processing routine
	movem.l	(sp)+,d0-d7/a0-a6	*	restore registers
	rtr				*	Resume processing
