* 
*	Floating Point Long to Float Routine :
*		Front End to FFP Floating Point Package.
*
*		double
*		fpltof(larg)
*		long larg;
*
*	Return : Floating Point representation of Long Fixed point integer
*
	xdef fpltof
	xdef _fpltof
	xref ffpifp
	text
fpltof:
_fpltof:
	link	a6,#-4
	movem.l	d3-d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpifp
	move.l	d7,d0
	movem.l	(sp)+,d3-d7
	unlk	a6
	rts
