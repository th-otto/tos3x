		include "aesparb.i"
		
		globl Evnt_multi
		
		text
		
Evnt_multi:
		movem.l   a2-a6,-(a7)
		lea.l     30(a7),a4
		lea.l     _GemParBlk,a2
		lea.l     intin(a2),a3
		move.w    d0,(a3)+
		move.w    d1,(a3)+
		move.w    d2,(a3)+
		move.w    24(a7),(a3)
		move.l    a0,d0
		beq       ev_mult1
		movea.l   a0,a5
		lea.l     intin+8(a2),a6
		move.l    (a5)+,(a6)+
		move.l    (a5)+,(a6)+
		move.w    (a5)+,(a6)+
ev_mult1:
		move.l    a1,d0
		beq       ev_mult2
		movea.l   a1,a0
		lea.l     intin+18(a2),a5
		move.l    (a0)+,(a5)+
		move.l    (a0)+,(a5)+
		move.w    (a0)+,(a5)+
ev_mult2:
		lea.l     intin+32(a2),a3
		move.w    (a4)+,-(a3)
		move.w    (a4),-(a3)
		move.l    26(a7),addrin(a2)
		moveq.l   #25,d0
		jsr       _AesCtrl(pc)
		movea.l   a0,a3
		addq.w    #8,a3
		movea.l   34(a7),a1
		move.l    (a0)+,(a1)+
		move.l    (a0)+,(a1)+
		movea.l   38(a7),a0
		move.w    (a3)+,(a0)
		movea.l   42(a7),a1
		move.w    (a3),(a1)
		movem.l   (a7)+,a2-a6
		rts
