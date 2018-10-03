		globl rc_center
		xref rc_inset
		
rc_center:
		move.l    a2,-(a7)
		move.l    a3,-(a7)
		movea.l   a1,a2
		movea.l   a0,a1
		movea.l   a2,a3
		move.l    (a1)+,(a3)+
		move.l    (a1)+,(a3)+
		sub.w     d0,4(a2)
		add.w     d1,6(a2)
		movea.l   a2,a0
		jsr       rc_inset(pc)
		movea.l   (a7)+,a3
		movea.l   (a7)+,a2
		rts
