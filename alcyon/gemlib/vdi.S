	xdef _vdi
	xdef _pblock

_vdi:
	move.l    #_pblock,d1
	moveq.l   #115,d0
	trap      #2
	rts

	data

_pblock:
	dc.l    _contrl
	dc.l    _intin
	dc.l    _ptsin
	dc.l    _intout
	dc.l    _ptsout
