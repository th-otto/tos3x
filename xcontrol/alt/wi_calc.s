		include "aesparb.i"
		
		globl Wind_calc
		xref gl_wbox
		xref gl_hbox
		
		text

Wind_calc:
		movem.l   d3-d6/a2,-(a7)
		moveq.l   #1,d2
		move.w    d2,d3
		move.w    d2,d4
		move.w    d2,d5
		lea.l     gl_hbox,a2
		moveq.l   #15,d6
		and.w     d1,d6
		tst.w     d6
		beq       wi_calc1
		moveq.l   #-1,d6
		add.w     (a2),d6
		add.w     d6,d3
wi_calc1:
		moveq.l   #16,d6
		and.w     d1,d6
		beq       wi_calc2
		moveq.l   #-1,d6
		add.w     (a2),d6
		add.w     d6,d3
wi_calc2:
		move.w    d1,d6
		and.w     #$01E0,d6
		beq       wi_calc3
		moveq.l   #-1,d6
		add.w     gl_wbox,d6
		add.w     d6,d4
wi_calc3:
		move.w    d1,d6
		and.w     #$0E20,d6
		beq       wi_calc4
		moveq.l   #-1,d1
		add.w     (a2),d1
		add.w     d1,d5
wi_calc4:
		tst.w     d0
		bne       wi_calc5
		neg.w     d2
		neg.w     d3
		neg.w     d4
		neg.w     d5
wi_calc5:
		move.w    (a0),d0
		add.w     d2,d0
		move.w    d0,(a1)
		move.w    2(a0),d1
		add.w     d3,d1
		move.w    d1,2(a1)
		move.w    4(a0),d0
		sub.w     d4,d0
		sub.w     d2,d0
		move.w    d0,4(a1)
		move.w    6(a0),d1
		sub.w     d3,d1
		sub.w     d5,d1
		move.w    d1,6(a1)
		movem.l   (a7)+,d3-d6/a2
		rts
