	xdef _sw_
	text
_sw_:
	tst.l	d5
	beq	ok
	move.l	#-1,(sp)	* Destroy the evidence
	divs	#$0,d5
ok:	rts

*********************************************************
*							*
*	Copyright Notice Module				*
*							*
*********************************************************

	  data
___cpyrt: dc.b	'CP/M-68K(tm), Version 1.2, '
	  dc.b 'Copyright (c) 1983, Digital Research '
serial:	  dc.b 'XXXX-0000-654321'
	  dc.w 0
