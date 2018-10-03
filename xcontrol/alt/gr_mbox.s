		include "aesparb.i"
		
		globl Graf_mbox
		
		text
		
Graf_mbox:
		lea.l     _GemParBlk+intin,a1
		move.w    4(a0),(a1)+
		move.w    6(a0),(a1)+
		move.w    (a0),(a1)+
		move.w    2(a0),(a1)+
		move.w    d0,(a1)+
		move.w    d1,(a1)+
		moveq.l   #72,d0
		jsr       _AesCtrl(pc)
		rts
