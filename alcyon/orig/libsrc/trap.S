	xdef _trap
	text
_trap:	
	move.l (a7)+,savpc
    trap #1
	move.l savpc,-(a7)
    rts
    bss
savpc:
    ds.l 1
