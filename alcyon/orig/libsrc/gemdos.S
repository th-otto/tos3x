	xdef _gemdos
	xdef saveret
	text
_gemdos:
	move.l    (a7)+,saveret
	trap      #1
	move.l    saveret,-(a7)
	rts

	bss
saveret: ds.l 1
