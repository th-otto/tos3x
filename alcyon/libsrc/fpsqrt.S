* 
*	Floating Point Square Root :
*		Front End to FFP Floating Point Package.
*
*		double
*		sqrt(farg)
*		double farg;
*
*	Input : in radians
*	Returns : square root of Floating point number
*
	xdef sqrt
	xdef _sqrt
	xref ffpsqrt
	text
sqrt:
_sqrt:
	link	a6,#-4
	movem.l	d3-d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpsqrt
	move.l	d7,d0
	movem.l	(sp)+,d3-d7
	unlk	a6
	rts
