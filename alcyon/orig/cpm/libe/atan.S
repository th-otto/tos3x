* 
*       Floating Point Arctangen:
*               Front End to FFP Floating Point Package.
*
*               double
*               atan(farg)
*               double farg;
*
*       Returns : negated Floating point number
*
	xdef _atan
	xref iefatan
	text
_atan:
	link    a6,#-4
	move.l  d7,-(sp)
	move.l  8(a6),d7
	jsr     iefatan
	move.l  d7,d0
	move.l  (sp)+,d7
	unlk    a6
	rts
