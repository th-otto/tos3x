		include "aesparb.i"
		
		globl Evnt_mouse
		
		text
		
Evnt_mouse:
		move.l    a2,-(a7)
		move.l    a3,-(a7)
		movea.l   a1,a2
		movea.l   a0,a1
		lea.l     _GemParBlk+intin,a3
		move.l    (a1)+,(a3)+
		move.l    (a1)+,(a3)+
		move.w    (a1)+,(a3)+
		moveq.l   #22,d0
		jsr       _AesCtrl(pc)
		movea.l   a2,a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		movea.l   (a7)+,a3
		movea.l   (a7)+,a2
		rts
