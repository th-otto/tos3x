* 
*       Floating Point Negation :
*               Front End to IEEE Floating Point Package.
*
*               double
*               fpneg(farg)
*               double farg;
*
*       Returns : negated Floating point number
*
	xdef fpneg
	xdef _fpneg
	xref iefneg
	text
fpneg:
_fpneg:
	link    a6,#-4
	movem.l d7,-(sp)
	move.l  8(a6),d7
	jsr     iefneg
	move.l  d7,d0
	movem.l (sp)+,d7
	unlk    a6
	rts
