* 
*       Floating Point Cosine :
*               Front End to FFP Floating Point Package.
*
*               double
*               cos(farg)
*               double farg;
*
*       Input : in radians
*       Returns : cosine of Floating point number
*
	xdef cos
	xdef _cos
	xref ffpcos
	text
cos:
_cos:
	link    a6,#-4
	movem.l d3-d7,-(sp)
	move.l  8(a6),d7
	jsr     ffpcos
	move.l  d7,d0
	movem.l (sp)+,d3-d7
	unlk    a6
	rts
