* 
*	Floating Point Logarithm :
*		Front End to IEEE Floating Point Package.
*
*		double
*		log(x)
*		double x;
*
*	Returns : the floating point logarithm
*
	xdef log
	xdef _log
	xref ieflog
	text
log:
_log:
	link	a6,#-4
	movem.l	d7,-(sp)
	move.l	8(a6),d7
	jsr		ieflog
	move.l	d7,d0
	movem.l	(sp)+,d7
	unlk	a6
	rts
