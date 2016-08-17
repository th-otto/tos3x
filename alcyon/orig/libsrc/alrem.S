	xref	_ldiv
	xref	_ldivr
	comm	_ldivr,4
	xdef	_alrem
	xdef  alrem
	text
_alrem:
alrem:
	link	a6,#-2
	move.l	12(a6),-(sp)
	move.l	8(a6),a0
	move.l	(a0),-(sp)
	jsr	_ldiv
	cmpm.l	(sp)+,(sp)+
	move.l	_ldivr,d0
	move.l	8(a6),a1
	move.l	d0,(a1)
	unlk	a6
	rts
