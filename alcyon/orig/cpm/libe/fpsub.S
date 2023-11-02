* 
*       Floating Point Subtraction :
*               Front End to IEEE Floating Point Package.
*
*               double
*               fpsub(subtrahend,minuend)
*               double subtrahend, minuend;
*
*       Returns : Floating point subtraction result
*
	xdef fpsub
	xdef _fpsub
	xref iefsub
	text
fpsub:
_fpsub:
	link    a6,#-4
	movem.l d6-d7,-(sp)
	move.l  8(a6),d7
	move.l  12(a6),d6
	jsr     iefsub
	move.l  d7,d0
	movem.l (sp)+,d6-d7
	unlk    a6
	rts
