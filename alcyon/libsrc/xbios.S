	xdef _xbios
	xref saveret
	text
_xbios:
	move.l    (a7)+,saveret
	trap      #14
	move.l    saveret,-(a7)
	rts
