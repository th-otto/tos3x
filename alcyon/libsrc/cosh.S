* 
*	Floating Point Hyperbolic cosine:
*		Front End to FFP Floating Point Package.
*
*		double
*		cosh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
	globl _cosh
	globl ffpcosh
	text
fpcosh:
_cosh:
	link	a6,#-4
	move.l	d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpcosh
	move.l	d7,d0
	move.l	(sp)+,d7
	unlk	a6
	rts
