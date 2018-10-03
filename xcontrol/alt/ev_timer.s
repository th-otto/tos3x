		include "aesparb.i"
		
		globl Evnt_timer
		
		text
		
Evnt_timer:
		subq.w    #4,a7
		move.l    d0,(a7)
		lea.l     _GemParBlk+intin,a0
		move.w    2(a7),(a0)+
		move.w    (a7),(a0)
		moveq.l   #24,d0
		jsr       _AesCtrl(pc)
		addq.w    #4,a7
		rts
