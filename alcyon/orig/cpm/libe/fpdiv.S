* 
*	Floating Point Division :
*		Front End to IEEE Floating Point Package.
*
*		double
*		fpdiv(divisor,dividend)
*		double divisor, dividend;
*
*	Return : Floating Point Quotient
*
	xdef fpdiv
	xdef _fpdiv
	xref iefdiv
	text
fpdiv:
_fpdiv:
	link	a6,#-4
	movem.l	d6-d7,-(sp)
	move.l	8(a6),d7
	move.l	12(a6),d6
	jsr		iefdiv
	move.l	d7,d0
	movem.l	(sp)+,d6-d7
	unlk	a6
	rts
