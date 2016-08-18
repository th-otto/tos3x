		xref	__break
		xdef	_brk
		text
_brk:		
		movea.l	4(sp),a0		*	New break?
		move.l 	a0,d0
		lea	$100(a0),a0		*	Chicken factor
		cmpa.l	a0,sp			*	Compare
		bcc	brkok			*	OK, continue
		move.l	#-1,d0			*	Load return reg
		rts				*	Return

brkok:
		move.l	d0,__break		*	Save the break
		clr.l	d0			*	Set OK return
		rts				*	return
		
