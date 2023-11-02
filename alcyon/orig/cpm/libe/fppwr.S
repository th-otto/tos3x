* 
*       Floating Point Power :
*               Front End to IEEE Floating Point Package.
*
*               double
*               pow(x,y)
*               double x, y;
*
*       Returns : x ^ y
*
	xdef pow
	xdef _pow
	xref iefpwr
	.text
pow:
_pow:
	link    a6,#-4
	movem.l d3-d7,-(sp)
	move.l  8(a6),d7
	move.l  12(a6),d6
	jsr     iefpwr
	move.l  d7,d0
	movem.l (sp)+,d3-d7
	unlk    a6
	rts
