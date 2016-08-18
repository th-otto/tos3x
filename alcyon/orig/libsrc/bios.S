	xdef _bios
	xref saveret
	text
_bios:
	move.l    (a7)+,saveret
	trap      #13
	move.l    saveret,-(a7)
	rts
