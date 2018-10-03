		include "aesparb.i"
		
		globl Wind_create
		
		text

Wind_create:
		move.l    a2,-(a7)
		move.l    a3,-(a7)
		lea.l     _GemParBlk+intin,a1
		move.w    d0,(a1)+
		movea.l   a0,a2
		movea.l   a1,a3
		move.l    (a2)+,(a3)+
		move.l    (a2)+,(a3)+
		moveq.l   #100,d0
		jsr       _AesCtrl(pc)
		movea.l   (a7)+,a3
		movea.l   (a7)+,a2
		rts
