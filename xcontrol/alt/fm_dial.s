		include "aesparb.i"
		
		globl Form_dial
		
		text
		
Form_dial:
		movem.l   a2-a4,-(a7)
		lea.l     _GemParBlk+intin,a2
		move.w    d0,(a2)+
		movea.l   a0,a3
		movea.l   a2,a4
		addq.w    #8,a2
		move.l    (a3)+,(a4)+
		move.l    (a3)+,(a4)+
		movea.l   a1,a0
		movea.l   a2,a3
		move.l    (a0)+,(a3)+
		move.l    (a0)+,(a3)+
		moveq.l   #51,d0
		jsr       _AesCtrl(pc)
		movem.l   (a7)+,a2-a4
		rts
