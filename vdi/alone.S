**********************************  alone.s  **********************************
*
* =============================================================================
* $Author: lozben $	$Date: 91/01/03 15:04:53 $
* =============================================================================
*
* Revision 3.0  91/01/03  15:04:53  lozben
* New generation VDI
*
* Revision 2.2  90/02/16  12:20:32  lozben
* Fixed some of the code so that it can be assembled with mas.
*
* Revision 2.1  89/02/21  17:17:43  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
*
*******************************************************************************

#include "../common/config.h"
#include "vdidefs.h"
#include "asmdefs.h"
#include "../common/tosvars.inc"

#include "devequ.inc"
#include "lineaequ.inc"


*+
* alone.s - this file should be linked in when building screen.prg.
*
* author: Slavik Lozben
*-
		xdef	ringbell
		xdef	myvq_key_s
		xdef	_shifty


*+
* Return Shift, Control, Alt State
*-
myvq_key_s:	move.l	_sysbase,a0	; a0 -> base of os
		move.l	$24(a0),a0	; a0 -> _shifty
		move.b	(a0),_shifty	; _shifty <- current shift mode
		jmp	_vq_key_s	; go to the old guy

_shifty:	dc.w	0

*+
* Ring the bell, and return
*-
ringbell:	movea.l	#0,a5
		btst.b	#$2,_conterm	; see if bell is disabled
		beq	rgbel

		move.l	#bellsnd,-(sp)	; push ptr to data for bell
		move.w	#32,-(sp)	; push func #
		trap	#14		; xbios
		addq.l	#$6,sp		; clean stack
rgbel:		rts

		even
bellsnd:	dc.b	0,$34
		dc.b	1,0
		dc.b	2,0
		dc.b	3,0
		dc.b	4,0
		dc.b	5,0
		dc.b	6,0
		dc.b	7,$FE
		dc.b	8,$10		;enable envelope, ch a
		dc.b	9,0
		dc.b	10,0
		dc.b	11,0
		dc.b	12,$10
		dc.b	13,9		;envelope single attack
		dc.b	255,0

*
*
*  Quickly zero (lots of) memory.
*  Copyright 1986 Atari Corp.
*
*  Synopsis:	zeromem(start, end)
*		    LONG start;	    4(sp) -> first location
*		    LONG end;	    8(sp) -> last location + 1
*
*    Uses:	C registers d0-d2/a0-a2
*
*
		xdef _zeromem
_zeromem:
        movea.l   4(a7),a0	; a0 -> start
        movea.l   8(a7),a1	; a1 -> end+1
        movem.l   d3-d7/a3,-(a7)	; save registers
        moveq.l   #0,d1		; store zero into d1-d7/a3
        moveq.l   #0,d2
        moveq.l   #0,d3
        moveq.l   #0,d4
        moveq.l   #0,d5
        moveq.l   #0,d6
        moveq.l   #0,d7
        movea.w   d7,a3
        move.l    a0,d0		; word align first location
        btst      #0,d0
        beq.s     zerom1
        move.b    d1,(a0)+
zerom1:
        move.l    a1,d0		; d0 = ((a1 - a0) & ~0xff)
        sub.l     a0,d0
        and.l     #$FFFFFF00,d0	; mask fract bits, d0 = whole part
        beq.s     zerom3		; if (d0 == 0) do end-fraction;
        lea.l     0(a0,d0.l),a0	; a0 -> end of huge area
        movea.l   a0,a2		; a2 -> there, too
        lsr.l     #8,d0		; get 256-byte chunk count
zerom2:
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        movem.l   d1-d7/a3,-(a2)	; clear 32 bytes
        subq.l    #1,d0			; decrement count
        bne.s     zerom2		; while (d0) clear some more...
zerom3:
        cmpa.l    a0,a1			; while (a0 != a1)
        beq.s     zerom4		; (done)
        move.b    d1,(a0)+		; clear a byte
        bra.s     zerom3
zerom4:
        movem.l   (a7)+,d3-d7/a3		; restore registers
        rts

	bss

#include "vdivar.S"
