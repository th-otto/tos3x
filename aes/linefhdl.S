#define __ASSEMBLER__ 1

#include "config.h"
#include "aesdefs.h"
#include "asmdefs.h"

#if LINEF_HACK

noregs equ 0

	xdef linefhandler
linefhandler:
		move.w    (a7)+,d2
		movea.l   (a7)+,a0
		move.w    (a0)+,d1
		btst      #0,d1
		bne.s     linefh1
		move.w    d2,sr
		move.l    a0,-(a7)
		andi.w    #$0FFF,d1
		movea.l   #lineftab,a0
		movea.l   0(a0,d1.w),a0
		jmp       (a0)
linefh1:
		andi.w    #$0FFE,d1
		beq.s     linefh2
		lsl.w     #2,d1
		ori.w     #$0700,sr
		lea.l     movinst+2(pc),a0
		move.w    d1,(a0)
		addq.l    #4,a7
movinst:
		dc.w $4cdf,0  /* movem.l   (a7)+,noregs */
linefh2:
		move.w    d2,sr
		unlk      a6
		rts
		
		xdef lineftab
lineftab: ds.b 0

#endif
