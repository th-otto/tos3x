* 
*	Floating Point Square Root :
*		Front End to IEEE Floating Point Package.
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
	xref iefsqrt
	text
sqrt:
_sqrt:
	link	a6,#-4
	movem.l	d3-d7,-(sp)
	move.l	8(a6),d7
	jsr		iefsqrt
	move.l	d7,d0
	movem.l	(sp)+,d3-d7
	unlk	a6
	rts
