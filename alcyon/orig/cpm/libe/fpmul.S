* 
*       Floating Point Multiplication :
*               Front End to IEEE Floating Point Package.
*
*               double
*               fpmul(multiplier,multiplicand)
*               double multiplier, multiplicand;
*
*       Return : Result of Floating Point Multiply
*
	xdef fpmul
	xdef _fpmul
	xdef fpmult
	xdef _fpmult
	xref iefmul
	text
fpmult:
_fpmult:
fpmul:
_fpmul:
	link    a6,#-4
	movem.l d6-d7,-(sp)
	move.l  8(a6),d7
	move.l  12(a6),d6
	jsr     iefmul
	move.l  d7,d0
	movem.l (sp)+,d6-d7
	unlk    a6
	rts
