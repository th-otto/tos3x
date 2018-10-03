		include "aesparb.i"
		
		globl Graf_dragbox
		
		text
		
Graf_dragbox:
		movem.l   a2-a4,-(a7)
		movea.l   a0,a3
		lea.l     _GemParBlk+intin,a2
		move.w    4(a3),(a2)+
		move.w    6(a3),(a2)+
		move.w    (a0),(a2)+
		move.w    2(a3),(a2)+
		movea.l   a1,a0
		movea.l   a2,a4
		move.l    (a0)+,(a4)+
		move.l    (a0)+,(a4)+
		moveq.l   #71,d0
		jsr       _AesCtrl(pc)
		movea.l   a0,a2
		move.w    (a2)+,(a3)
		move.w    (a2),2(a3)
		movem.l   (a7)+,a2-a4
		rts
