	xref	_ldiv
	xdef	_aldiv
	xdef	aldiv
	text
_aldiv:
aldiv:
	link	a6,#-2
	move.l	12(a6),-(sp)
	move.l	8(a6),a0
	move.l	(a0),-(sp)
	jsr	_ldiv
	cmpm.l	(sp)+,(sp)+
	move.l	8(a6),a1
	move.l	d0,(a1)
	unlk	a6
	rts
