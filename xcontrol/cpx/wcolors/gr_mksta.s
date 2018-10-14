	XDEF _Graf_mkstate
	XREF @_AESif

	TEXT
	
_Graf_mkstate:
	link      a6,#0
	move.w    #$0114,d0
	jsr       @_AESif(pc)
	lea.l     2(a0),a1
	movea.l   8(a6),a0
	move.l    (a1)+,(a0)+
	move.l    (a1)+,(a0)+
	unlk      a6
	rts

	end
