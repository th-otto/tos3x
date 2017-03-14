	xdef _vdi
	
	xref _contrl

_vdi:
	move.l    #_contrl,pblock
	move.l    #pblock,d1
	moveq.l   #115,d0
	trap      #2
	rts

	bss

pbblock:
	ds.l	1
	xdef 	_iioff
_iioff: ds.l 1
	xdef 	_pioff
_pioff: ds.l 1
	xdef 	_iooff
_iooff: ds.l 1
	xdef 	_pooff
_pooff: ds.l 1
