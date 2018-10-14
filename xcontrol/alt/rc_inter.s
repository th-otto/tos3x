		globl rc_intersect
		xref mymin
		xref mymax
		
rc_intersect:
		movem.l   d3-d5/a2-a3,-(a7)
		movea.l   a0,a2
		movea.l   a1,a3
		move.w    (a0),d1
		add.w     4(a2),d1
		move.w    (a1),d0
		add.w     4(a3),d0
		jsr       mymin(pc)
		move.w    d0,d3
		move.w    2(a2),d1
		add.w     6(a2),d1
		move.w    2(a3),d0
		add.w     6(a3),d0
		jsr       mymin(pc)
		move.w    d0,d4
		move.w    (a2),d1
		move.w    (a3),d0
		jsr       mymax(pc)
		move.w    d0,d5
		move.w    2(a2),d1
		move.w    2(a3),d0
		jsr       mymax(pc)
		sub.w     d5,d3
		ble       rc1
		moveq.l   #1,d1
		bra       rc2
rc1:
		clr.w     d1
rc2:
		beq       return
		sub.w     d0,d4
		ble       rc3
		moveq.l   #1,d1
		bra       rc4
rc3:
		clr.w     d1
rc4:
		beq       return
		move.w    d5,(a3)
		move.w    d0,2(a3)
		move.w    d3,4(a3)
		move.w    d4,6(a3)
return:
		move.w    d1,d0
		movem.l   (a7)+,d3-d5/a2-a3
		rts
