		include "aesparb.i"
		
		globl Objc_change
		xref objc_offset
		
		text

Objc_change:
		movem.l   d3-d5/a2-a3/a5,-(a7)
		subq.w    #8,a7
		movea.l   a0,a5
		move.w    d0,d3
		movea.l   a1,a3
		move.w    d1,d4
		move.w    d2,d5
		lea.l     _GemParBlk+intin,a2
		move.l    a3,d0
		bne       ob_chan1
		pea.l     2(a7)
		lea.l     4(a7),a1
		move.w    d3,d0
		jsr       objc_offset(pc)
		addq.w    #4,a7
		move.w    d3,d0
		add.w     d0,d0
		add.w     d3,d0
		lsl.w     #3,d0
		move.w    20(a5,d0.w),4(a7)
		move.w    22(a5,d0.w),6(a7)
		bra       ob_chan2
ob_chan1:
		movea.l   a3,a0
		lea.l     (a7),a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
ob_chan2:
		move.w    d3,(a2)+
		clr.w     (a2)+
		lea.l     (a7),a0
		movea.l   a2,a1
		addq.w    #8,a2
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		move.w    d4,(a2)+
		move.w    d5,(a2)
		move.l    a5,_GemParBlk+addrin
		moveq.l   #47,d0
		jsr       _AesCtrl(pc)
		addq.w    #8,a7
		movem.l   (a7)+,d3-d5/a2-a3/a5
		rts
