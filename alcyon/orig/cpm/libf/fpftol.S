* 
*	Floating Point Float to Long Routine :
*		Front End to FFP Floating Point Package.
*
*		long
*		fpftol(fparg)
*		double fparg;
*
*	Condition Codes : V bit signifies Overflow
*
*	Return : Fixed Point representation of Floating Point Number
*
	xdef fpftol
	xdef _fpftol
	xref ffpfpi
	text
fpftol:
_fpftol:
	link	a6,#-4
	movem.l	d3-d7,-(sp)
	move.l	8(a6),d7
	jsr		ffpfpi
	move.l	d7,d0
	movem.l	(sp)+,d3-d7
	unlk	a6
	rts
