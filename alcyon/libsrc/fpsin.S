* 
*	Floating Point Sine :
*		Front End to FFP Floating Point Package.
*
*		double
*		sin(farg)
*		double farg;
*
*	Input : in radians
*	Returns : sine of Floating point number
*
	xdef sin
	xdef _sin
	xref ffpsin
	text
sin:
_sin:
	link	a6,#-4
	movem.l	d3-d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpsin
	move.l	d7,d0
	movem.l	(sp)+,d3-d7
	unlk	a6
	rts
