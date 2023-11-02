* 
*	Floating Point Compare :
*		Front End to IEEE Floating Point Package.
*
*		fpcmp(source,dest)
*		double source, dest;
*
*	Returns : Condition codes based on Floating Point Compare
*			  in the Condition code register
*
	xdef fpcmp
	xdef _fpcmp
	xref iefcmp
	text
fpcmp:
_fpcmp:
	link	a6,#-4
	movem.l	d6-d7,-(sp)
	move.l	8(a6),d7
	move.l	12(a6),d6
	jsr		iefcmp
	movem.l	(sp)+,d6-d7
	unlk	a6
	rts
