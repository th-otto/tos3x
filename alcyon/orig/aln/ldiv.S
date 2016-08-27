	xref _ldivr
	comm _ldivr,4
	globl _ldiv
	globl ldiv
	text
_ldiv:
ldiv:
	movea.l   d3,a0
	move.l 4(a7),d1
	bge.s L1
	neg.l d1
L1:
	move.l 8(a7),d3
	bgt.s  L3
	blt.s  L2
	move.l #$80000000,d0
	move.l    d0,d1
	bra L99
L2:
	neg.l d3
L3:
	cmp.l     #1,d3
	bne L4
	move.l    d1,d0
	moveq.l   #0,d1
	bra.s     L11
L4:
	moveq.l   #0,d0
	cmp.l     d1,d3
	blt.s     L5
	bgt.s     L11
	moveq.l   #1,d0
	moveq.l   #0,d1
	bra.s     L11
L5:
	moveq.l   #2,d2
	cmp.l     #$00010000,d1
	bge.s     L8
	divu.w    d3,d1
	move.w    d1,d0
	clr.w     d1
	swap      d1
	bra.s     L11
L7:
    add.l     d2,d2
L8:
    add.l     d3,d3
    cmp.l     d3,d1
    bcc.s     L7
    bra.s     L10
L9:
    cmp.l     d3,d1
    bcs.s     L10
    or.l      d2,d0
    sub.l     d3,d1
L10:
    lsr.l     #1,d3
    lsr.l     #1,d2
    bne.s     L9
L11:
    tst.w     4(a7)
    bpl.s     L12
    neg.l     d0
    neg.l     d1
L12:
    tst.w     8(a7)
    bpl.s     L99
    neg.l     d0
L99:
	move.l d1,_ldivr
    move.l    a0,d3
	rts
