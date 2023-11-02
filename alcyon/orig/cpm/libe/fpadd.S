* 
*	Floating Point Addition :
*		Front End to IEEE Floating Point Package.
*
*		double
*		fpadd(addend,adder)
*		double addend, adder;
*
*	Returns : Sum of two floating point numbers
*
	xdef fpadd
	xdef _fpadd
	xref iefadd
	text
fpadd:
_fpadd:
	link	a6,#-4
	movem.l	d6-d7,-(sp)
	move.l	8(a6),d7
	move.l	12(a6),d6
	jsr		iefadd
	move.l	d7,d0
	movem.l	(sp)+,d6-d7
	unlk	a6
	rts
