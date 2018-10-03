		include "aesparb.i"
		
		globl Graf_growbox
		
		text
		
Graf_growbox:
		move.l    a2,-(a7)
		move.l    a3,-(a7)
		movea.l   a0,a2
		lea.l     _GemParBlk+intin,a3
		move.l    (a2)+,(a3)+
		move.l    (a2)+,(a3)+
		movea.l   a1,a0
		lea.l     _GemParBlk+intin+8,a2
		move.l    (a0)+,(a2)+
		move.l    (a0)+,(a2)+
		moveq.l   #73,d0
		jsr       _AesCtrl(pc)
		movea.l   (a7)+,a3
		movea.l   (a7)+,a2
		rts
