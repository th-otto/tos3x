		include "aesparb.i"
		
		globl Graf_rubberbox
		
		text
		
Graf_rubberbox:
		move.l    a2,-(a7)
		movea.l   a0,a2
		lea.l     _GemParBlk+intin,a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		moveq.l   #70,d0
		jsr       _AesCtrl(pc)
		move.w    (a0)+,4(a2)
		move.w    (a0),6(a2)
		movea.l   (a7)+,a2
		rts
