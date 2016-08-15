
* 
*	Floating Point Hyperbolic tangent:
*		Front End to FFP Floating Point Package.
*
*		double
*		tanh(farg)
*		double farg;
*
*	Returns : negated Floating point number
*
	xdef _tanh
	xref ffptanh
.text
fptanh:
_tanh:
	link	a6,#-4
	move.l	d7,-(sp)
	move.l	8(a6),d7
	jsr		ffptanh
	move.l	d7,d0
	move.l	(sp)+,d7
	unlk	a6
	rts
