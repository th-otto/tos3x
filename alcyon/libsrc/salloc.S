*
*	_salloc function.
*
*	This function allocates a data area on the stack.  
*
*	Calling Sequence:
*
*	adr = _salloc(size);
*
*	Returns the address of the area allocated.
*
*	Pulled out of 'w.s' 1/84 whf
*	Fixed 2/84 sw
*
* DO NOT USE; does not work with the current function prologue/epilogue:
*
* link a6,#size
* movem.l saveregs,-(a7)
* ...
* movem.l (a7)+,saveregs
* unlk a6
* rts
*
* manipulating sp does not work because that restores
* the saved registers from the wrong location in the epilogue
*

	xdef	__salloc
__salloc:
	move.l	(sp)+,a0			*	Save return address
	clr.l	d0				*	Zap d0 high word
	move.w	(sp),d0			*	Get arg (word size)
	addq.l	#1+4,d0				*	Round 
	bclr.l	#0,d0				*		up to word
	move.l	__break,a1			*	a1 -> break area
	lea	$100(a1),a1			*	Add chicken factor
	lea	0(a1,d0.l),a1			*	And size
	cmpa.l	a1,sp				*	OK?
	bhi	ok				*sw	Overflow, die
	jmp	__sovf				*sw	Jump to error in startup
ok:	sub.l	d0,sp				*	allocate
	lea	4(sp),a1			*	a1 -> area start
	move.l	a1,d0				*	set return code
	jmp	(a0)				*	and "return"

