		include "aesparb.i"
		
		globl Graf_mkstate
		
		text
		
Graf_mkstate:
		move.l    a2,-(a7)
		movea.l   a0,a2
		moveq.l   #79,d0
		jsr       _AesCtrl(pc)
		movea.l   a2,a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		movea.l   (a7)+,a2
		rts
