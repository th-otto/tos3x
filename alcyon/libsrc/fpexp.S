* 
*	Floating Point Exponent :
*		Front End to FFP Floating Point Package.
*
*		double
*		exp(x)
*		double x;
*
*	Returns : e ^ x (where e = 2.718...)
*
	xdef exp
	xdef _exp
	xref ffpexp
	text
exp:
_exp:
	link	a6,#-4
	movem.l	d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpexp
	move.l	d7,d0
	movem.l	(sp)+,d7
	unlk	a6
	rts
