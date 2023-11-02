
* 
*	Floating Point Hyperbolic sine:
*		Front End to FFP Floating Point Package.
*
*		double
*		sinh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
	xdef _sinh
	xref iefsinh
	text
_sinh:
	link	a6,#-4
	move.l	d7,-(sp)
	move.l	8(a6),d7
	jsr		iefsinh
	move.l	d7,d0
	move.l	(sp)+,d7
	unlk	a6
	rts
