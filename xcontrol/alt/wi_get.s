		include "aesparb.i"
		
		globl Wind_get
		
		text

Wind_get:
		move.l    a2,-(a7)
		movea.l   a0,a2
		lea.l     _GemParBlk+intin,a0
		move.w    d0,(a0)+
		move.w    d1,(a0)+
		moveq.l   #104,d0
		jsr       _AesCtrl(pc)
		movea.l   a2,a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		movea.l   (a7)+,a2
		rts
