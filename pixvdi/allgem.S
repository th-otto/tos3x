*********************************  allgem.s  ***********************************
*
* $Revision: 3.5 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/ttgem.s,v $
* =============================================================================
* $Author: lozben $	$Date: 91/07/30 16:01:22 $     $Locker:  $
* =============================================================================
*
* $Log:	ttgem.s,v $
*******************************************************************************

#include "vdiincld.inc"
#include "asmdefs.h"
#include "tosvars.inc"

#include "devequ.inc"
#include "lineaequ.inc"


		xdef	_InitFonts
		xdef	_v_clrwk
   		xdef	_DINIT_G
   		xdef	_vex_timv
   		xdef	_GCHC_KEY
   		xdef	_GCHR_KEY
		xdef	_GLOC_KEY
		xdef	_GSHIFT_S
   		xdef	_INIT_G
		xdef	_v_cellarray
		xdef	_v_nop
		xdef	_vq_cellarray
		xdef	esc_init
		xdef	_esc_init
		xdef	_dev_init

		xref	rout_init
		xref	_SETBLT
		

		xref	_MOV_CUR	; External Routines called
		xref	_vsc_form
		xref	gl_f_init
		xref	normal_ascii


		xref	esce
		xref	escf
		xref	mouse_rel
		xref	vb_draw

		xref	arrow_cdb
		xref	_shifty
		xref	rel_pblock


CON		equ	  2
DEV		equ	CON		; source of key press
BIOS		equ	 13
BCONSTAT	equ	  1
BCONIN		equ	  2
SETEXC		equ	  5
TICKCAL		equ	  6
XBIOS		equ	 14
BLITMODE	equ	$40

STAYINDEV equ 1
DEFREZ    equ 6

		text

*******************************************************************************
*
*	GET SHIFT STATE		entry: 		none
*				exit:		CTL/SHIFT/ALT status in d0
*				destroys:	nothing
*
*******************************************************************************

/* 306de: 00e06676 */
_GSHIFT_S:	move.b	_shifty,d0	; Get the keyboard state
		andi.w	#$000F,d0	; We only want the low 4 bits

* Reuse a rts for these stubed routines
_v_cellarray:
_v_nop:
_vq_cellarray:
return:		rts

*******************************************************************************
*
*	CLEAR SCREEN		entry: 		none
*				exit:		none
*				destroys:	a0/d0
*
*******************************************************************************

/* 306de: 00e06682 */
_v_clrwk: PIXLA(a0)
#if TOSVERSION < 0x400
		move.l    d0,-(a7)
#endif
        move.w	PIXLV(_v_planes,a0),d0	; d0 <- # of planes
		mulu.w	PIXLV(v_hz_rez,a0),d0		; d0 <- (planes) x (hor rez)
		mulu.w	PIXLV(v_vt_rez,a0),d0		; d0 <- (plns)x(hrez)x(vrt rz)
		lsr.l	#3,d0			; d0 <- # bytes to clr (screen)
			
		move.l	_v_bas_ad,-(sp)
		add.l	d0,(sp)			; ending addr
		move.l	_v_bas_ad,-(sp)		; starting address
#if VIDEL_SUPPORT
		jsr	clear			; use general clr routine
#else
		jsr	_zeromem
#endif
		addq.l	#8,sp			; cleanup stack

#if TOSVERSION < 0x400
		move.l    (a7)+,d0
#endif
		rts

#if VIDEL_SUPPORT
*+
*
*  Quickly zero (lots of) memory.
*  Copyright 1986 Atari Corp.
*
*  Synopsis:	clear(start, end)
*		    LONG start;	    4(sp) -> first location
*		    LONG end;	    8(sp) -> last location + 1
*
*    Uses:	C registers d0-d2/a0-a2
*
*-
clear:		move.l	4(sp),a0	; a0 -> start
		move.l	8(sp),a1	; a1 -> end+1
		movem.l	d3-d7/a3,-(sp)	; save registers

		PIXLA(a3)
		move.l	pal_map(a3),d1	; d1 <- desired background col

		move.l	d1,d2		; store background
		move.l	d1,d3		;    into d1-d7/a3
		move.l	d1,d4
		move.l	d1,d5
		move.l	d1,d6
		move.l	d1,d7
		move.l	d1,a3

		move.l	a0,d0		; word align first location
		btst	#0,d0
		beq	clr1		; (not necessary)
		move.b	d1,(a0)+

clr1:		move.l	a1,d0		; d0 = ((a1 - a0) & ~0xff)
		sub.l	a0,d0
		and.l	#$ffffff00,d0	; mask fract bits, d0 = whole part
		beq	clr3		; if (d0 == 0) do end-fraction;
		lea	(a0,d0.l),a0	; a0 -> end of huge area
		move.l	a0,a2		; a2 -> there, too
		lsr.l	#8,d0		; get 256-byte chunk count

clr2:		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		movem.l	d1-d7/a3,-(a2)	; clear 32 bytes
		subq.l	#1,d0		; decrement count
		bne	clr2		; while (d0) clear some more...

clr3:		cmp.l	a0,a1		; while (a0 != a1)
		beq	clr4		; (done)
		move.b	d1,(a0)+	; clear a byte
		bra	clr3

clr4:		movem.l	(sp)+,d3-d7/a3	; restore registers
		rts
#endif

*******************************************************************************
*
*	INITIALIZE GRAPHICS	entry: 		none
*				exit:		none
*				destroys:	see v_clrwk
*
*******************************************************************************

_INIT_G:	PIXLA(a1)
		move.l	#return,PIXLV(tim_addr,a1)	; Tick points to rts

		move	SR,-(sp)		; Disable interrupts
		ori.w	#$0700,SR

		pea	tick_int		; My timer vector
		move.w	#$100,-(sp)		; 100 is timer vector
		move.w	#5,-(sp)		; exchange exception vector
		trap	#13

		PIXLA(a1)
		addq.l	#8,sp			; clean up stack
		move.l	d0,PIXLV(tim_chain,a1)	; Set up chain before int enabl
		move	(sp)+,SR		; Restore interrupt context

		bsr	mouse_init		; Init the mouse
		bsr	escf			; hide alpha cursor

		bra	_v_clrwk


*******************************************************************************
*
*	EXCHANGE TIMER VECTOR	entry: 		new vector in CONTRL[7-8]
*				exit:		old vector in CONTRL[9-10]
*				destroys:	a0/a1
*
*******************************************************************************

/* 306de: 00e066e6 */
_vex_timv:	PIXLA(a1)
		move.l	PIXLV(_CONTRL,a1),a0

		move	SR,-(sp)		; Disable interrupts
		ori.w	#$0700,SR		; THIS IS PARANOID !
		move.l	PIXLV(tim_addr,a1),18(a0)
		move.l	14(a0),PIXLV(tim_addr,a1)
		move	(sp)+,SR

		move.w	#TICKCAL,-(sp)		; Get ms/tick
		trap	#BIOS
		addq.l	#2,sp

		PIXLA(a1)
		move.l	PIXLV(_INTOUT,a1),a0
		move.w	d0,(a0)

		rts

*******************************************************************************

#if TOSVERSION < 0x400

devtab:    dc.w 0,0,0,1,2,0,4,0,6,7,0

		xdef _FindDevice
_FindDevice:

        moveq.l   #0,d0
        move.b    _sshiftmod,d0 ; sshiftmod
        movea.l   $00003A22,a0 ; LA_INTIN
        move.w    (a0),d1
        cmp.w     #STAYINDEV,d1 
        beq.s     FindDev3
        cmp.w     #DEFREZ,d0
        beq.s     FindDev3
        cmp.w     #$000A,d1
        ble.s     FindDev1
        moveq.l   #0,d1
FindDev1:
        lsl.w     #1,d1
        move.w    devtab(pc,d1.w),d2
        cmp.w     d0,d2
        beq.s     FindDev3
        cmp.w     #DEFREZ,d2
        bne.s     FindDev2
        move.w    devtab(pc),d2
FindDev2:
        move.w    d2,-(a7)
        move.l    #-1,-(a7)
        move.l    #-1,-(a7)
        move.w    #$0005,-(a7)
        trap      #14 ; SETREZ(rez)
        lea.l     12(a7),a7
        bra.s     FindDev4
FindDev3:
        bsr       esc_init
FindDev4:
        pea.l     _colors
        move.w    #$0100,-(a7)
        move.w    #$0000,-(a7)
        move.w    #$0054,-(a7)
        trap      #14 ; EsetPalette(0, 256, colors)
        lea.l     10(a7),a7
        cmpi.b    #$01,_sshiftmod
        bne.s     FindDev5
        lea.l     _colors,a0
        move.w    30(a0),-(a7)
        move.w    #$0003,-(a7)
        move.w    #$0053,-(a7)
        trap      #14 ; EsetColor(3, colors[15])
        addq.w    #6,a7
FindDev5:
        cmpi.b    #$02,_sshiftmod
        bne.s     FindDev6
        move.w    #$0000,-(a7)
        move.w    #$00FE,-(a7)
        move.w    #$0053,-(a7)
        trap      #14 ; EsetColor(254, 0x000);
        addq.w    #6,a7
        move.w    #$0FFF,-(a7)
        move.w    #$00FF,-(a7)
        move.w    #$0053,-(a7)
        trap      #14 ; EsetColor(255, 0xfff);
        addq.w    #6,a7
FindDev6:
        moveq.l   #0,d0
        move.b    _sshiftmod,d0
        rts

#endif

*******************************************************************************
*
*	Timer interrupt routine	entry: 		none
*				exit:		none
*				destroys:	none
*
*******************************************************************************

/* 306de: 00e067ee */
tick_int:
#if TOSVERSION >= 0x400
		subq.l  #4,sp			; leave some room on the stack
#endif
		movem.l	d0-d7/a0-a6,-(sp)
		PIXLA(a1)
		move.l	PIXLV(tim_addr,a1),a0		; Invoke user routine
#if TOSVERSION >= 0x400
		move.l	PIXLV(tim_chain,a1),60(sp)	; store chained routine
#endif
		jsr	(a0)
		movem.l	(sp)+,d0-d7/a0-a6
#if TOSVERSION < 0x400
		move.l	PIXLV(tim_chain,a1),-(sp)	; store chained routine
#endif
		rts				; jump to next routine in chain


*******************************************************************************
*
*	DEINITIALIZE GRAPHICS	entry: 		none
*				exit:		none
*				destroys:	a1 + see dinit_mouse, v_clrwk,
*						esce
*
*******************************************************************************

/* 306de: 00e06806 */
_DINIT_G:	PIXLA(a1)
		move.l	PIXLV(tim_chain,a1),-(sp)	; reset timer vector
		move.w	#$100,-(sp)
		move.w	#SETEXC,-(sp)
		trap	#BIOS
		addq.l	#8,sp

		bsr	dinit_mouse
		bsr	_v_clrwk
		bra	esce			; Show the alpha cursor. return



*******************************************************************************
*
*
*	GCHC_KEY	get choice for choice input: NOT IMPLEMENTED
*
*			exit:	d0:   0	 nothing happened
*				      1	 choice value
*				      2	 button pressed
*
*				TERM_CH	 16 bit char info
*				
*******************************************************************************

_GCHC_KEY:	PIXLA(a1)
		move.w	#1,PIXLV(_TERM_CH,a1)		; return dummy choice per 8086
		moveq.l	#1,d0			; indicate choice value
		rts


*******************************************************************************
*
* title:	_GLOC_KEY
*
* purpose:	Note which buttons have had a state change and whether
*		the mouse has been moved since the previous envocation of
*		this routine. If the button states havn't changed, check
*		for a key press from the console device.
*
* output:
*
*	d0	0	- nothing
*
*		1	- button or key pressed
*
*  			   _TERM_CH = 16 bit char info
*
*		2 	- new coordinates
*
*			   _X1 = new X
*			   _Y1 = new Y
*
*		3	- button or key pressed and new coordinates
*			
*			   _TERM_CH = 16 bit char info
*			   _X1 = new X
*			   _Y1 = new Y
*
* 	NOTE:	THIS ROUTINE REPORTS THE LAST CHANGE IN BUTTON STATE 
*		RATHER THAN REPORTING THE BUTTON STATE ITSELF. THE
*		BUTTON STATE CHANGE FLAGS AND MOTION FLAG ARE CLEARED
*		ON EXIT SO THAT SUBSEQUENT CALLS TO THE ROUTINE WILL NOT 
*		YIELD DUPLICATE REPORTS OF THE SAME EVENT. WHEN USING THIS
*		ROUTINE TO POLL THE MOUSE FOR A PARTICULAR CONDITION, THE 
*		FIRST CALL MAY NOT RETURN CURRENT BUTTON CHANGE STATE DATA 
*		BUT IT WILL INITIALIZE THE SYSTEM FOR SUBSEQUENT CALLS.
*
*******************************************************************************

/* 306de: 00e06830 */
_GLOC_KEY:	PIXLA(a1)
		moveq.l	#3,d1		; d1 <- mask for updating cur_ms_stat

		move.w	SR,d0

		or.w	#$0700,SR		; >>>> BEGIN ATOM <<<<
		move.b	PIXLV(cur_ms_stat,a1),d2	; clearing state change flags indicates
		and.b	d1,PIXLV(cur_ms_stat,a1)	; that this data has been processed
		move.l	PIXLV(_GCURXY,a1),PIXLV(_X1,a1)	; X1<-latest X and Y1<-latest Y
		move.w	d0,SR			; >>>>  END ATOM  <<<<

		clr.w	d0		; d0 <- initial status: no change

		btst.l	#5,d2
		beq	test_button

		addq.w	#2,d0		; status: coordinate information valid

test_button:	and.b	#$C0,d2		; test for change in button state
		beq	key_stat	; if no change, check for key press

		move.w	#$20,d1		; d1 <- lf button terminator code: 32
		btst.l	#6,d2		; left button has priority over right.
		bne	button_exit	; if left isn't down then right is.

		addq.w	#1,d1		; d1 <- rt button terminator code: 33

button_exit:	move.w	d1,PIXLV(_TERM_CH,a1)	; store the character code
		addq.w	#1,d0		; status: mouse button pressed
		rts


key_stat:	move.w	 d0,-(sp)	; preserve mouse status
		bsr	_GCHR_KEY	; d0 on return:  0:no key  1:key press
		add.w	 (sp)+,d0	; d0 <- final locator status
		rts

*******************************************************************************
*
*	GCHR_KEY	get char for string input
*			entry: none
*			exit:  d0=1	key pressed
*			       d0=0	nothing happened
*
*			_TERM_CH		16 bit char info
*
*******************************************************************************

/* 306de: 00e0687e */
_GCHR_KEY:	move.w	#DEV,-(sp)	; see if a character present
		move.w	#BCONSTAT,-(sp)
		trap	#BIOS
		addq.l	#4,sp

		tst.w	d0		; d0=0 => no character
		beq	no_char

		move.w	#DEV,-(sp)	; load proper devoce
		move.w	#BCONIN,-(sp)	; get the character
		trap	#BIOS
		addq.l	#4,sp

		move.l	d0,d1
		swap	d1
		lsl.w	#8,d1		; d0<07:00> <- ascii code
		or.w	d1,d0		; d0<15:08> <- scan code

		PIXLA(a1)
		move.w	d0,PIXLV(_TERM_CH,a1)	; store character
		moveq.l	#1,d0		; indicate character present

no_char:	rts


*******************************************************************************
*
*  INITIALIZE MOUSE
*
*  on entry:
*
*     _GCURXY	initial XY position of mouse cursor
*
*******************************************************************************

mouse_init:
		PIXLA(a1)
		lea	user_init,a0		; button and motion vectors
		move.l	a0,PIXLV(user_but,a1)		; default to no action (rts)
		move.l	a0,PIXLV(user_mot,a1)
		move.l	#_MOV_CUR,PIXLV(user_cur,a1)	; set up link to vblank display

* Initialize the default mouse form (presently the arrow)

		lea	PIXLV(_INTIN,a1),a0		; a0 -> pointer to INTIN array
		move.l	(a0),-(sp)		; Save the intin pointer and
		move.l	#arrow_cdb,(a0)		; make it point to the arrow
		bsr	_vsc_form		; a1 gets clobbered here
		PIXLA(a1)

		move.l	(sp)+,PIXLV(_INTIN,a1)	; Restore INTIN pointer

* initialize mouse/draw status and semaphores

		moveq.l	#0,d0
		move.w	d0,PIXLV(_MOUSE_BT,a1)	; clear mouse button state
		move.b	d0,PIXLV(cur_ms_stat,a1)	; clear current mouse status
		move.b	d0,PIXLV(mouse_flag,a1)	; mouse_cdb is unlocked
		move.b	#1,PIXLV(_HIDE_CNT,a1)	; don't display cursor form yet

* Initialize communication packet between mouse interrupt service routine
* and vblank cursor display routine. Insert vblank draw routine into front
* of vblank queue.

		lea	PIXLV(xydraw,a1),a0		; init the draw parameter block

		move.w	PIXLV(v_hz_rez,a1),d0		; d0 <- horizontal rez
		lsr.w	#1,d0			; d0 <- x rez / 2
		move.w	d0,PIXLV(_GCURX,a1)		; init mouse X position

		move.w	PIXLV(v_vt_rez,a1),d0		; d0 <- vrtical rez
		lsr.w	#1,d0			; d0 <- y rez / 2
		move.w	d0,PIXLV(_GCURY,a1)		; init mouse Y position

		move.l	PIXLV(_GCURXY,a1),(a0)+	; to the initial position
		move.b	#1,(a0)			; mark data valid for update

		move.l	_vblqueue,a0		; insert the display routine
		move.l	#vb_draw,(a0)		; at the head of the queue

* Activate mouse interrupt processing

		pea	mouse_rel	; mouse packet interrupt handler
		pea	rel_pblock	; mouse configuration parameter block
		move.w	#1,-(sp)	; enable mouse, in relative mode
		clr.w	-(sp)		; INITMOUSE
		trap	#XBIOS
		lea	12(sp),sp

user_init:	rts



*******************************************************************************
*
*	DEINITIALIZE MOUSE	entry: 		none
*				exit:		none
*				destroys:	a0/d0
*
*******************************************************************************

dinit_mouse:	move.l	_vblqueue,a0	; remove draw routine from vblank queue
		clr.l	(a0)

		moveq.l	#-1,d0
		move.l	d0,-(sp)	; discontinue mouse processing
		move.l	d0,-(sp)
		clr.l	-(sp)		; type=0: disable mouse
		trap	#XBIOS		; INITMOUSE
		lea	12(sp),sp

		rts

	
*******************************************************************************
*			    escape initialization 			      *
*******************************************************************************

/* 306de: 00e069f4 */
	xdef resetdev
resetdev:
	rts

*+
* dev_init(devPtr). devPtr points to the proper device structure. Init current
* device pointer in linea variable structure, then drop through to esc_init().
*-
_dev_init: PIXLA(a1)
		tst.l	PIXLV(LA_USERDEVINIT,a1)			; see if vector set
		beq	devI4				; branch if not
		move.l	PIXLV(LA_USERDEVINIT,a1),a0		; see if vector set
		jsr	(a0)				; execute user routine

devI4:	move.l	4(sp),a0			; a0 -> device struct
		move.l	a0,PIXLV(LA_CURDEV,a1)			; init cur dev pointer
		move.l	#_ram8x16,DEVFNTPTR(a0)		; assume 8x16 sys fnt
		cmp.w	#400,DEVYREZ(a0)		; check vertical rez
		bge	devI0				; skip if fnt ok
		move.l	#_ram8x8,DEVFNTPTR(a0)		; set to an 8x8 sys fnt
		tst.l	DEVCURROUTINES(a0)		; see if routines set
		bne	esc_init			; if so don't do it

devI0:
#if VIDEL_SUPPORT
		cmp.w	#PIXPACKED,DEVFORMID(a0)	; see if pixel packed
		bne	devI2				; skip if not
		cmp.w	#16,DEVPLANES(a0)		; see if FALCON030
		bne	devI1				; if not init to other
		move.l	#_SPPixSoftList,DEVSOFTROUTINES(a0)
		move.l	#_SPPixHardList,DEVHARDROUTINES(a0)
		move.l	#_SPPixSoftList,DEVCURROUTINES(a0)
		bra	esc_init			; init glass TTY

devI1:
		move.l	#_PixSoftList,DEVSOFTROUTINES(a0)
		move.l	#_PixHardList,DEVHARDROUTINES(a0)
		move.l	#_PixSoftList,DEVCURROUTINES(a0)
		bra	esc_init			; init glass TTY
#endif

devI2:
		move.l	#_STSoftList,DEVSOFTROUTINES(a0)
		move.l	#_STHardList,DEVHARDROUTINES(a0)
		move.l	#_STSoftList,DEVCURROUTINES(a0)
		bra	esc_init			; init glass TTY

/* 306de: 00e0694a */
_esc_init:
esc_init:
		jsr	_InitFonts		; copy font headers into ram

    	PIXLA(a1)
		tst.l	PIXLV(LA_USERESCINIT,a1)		; see if vector set
		beq	esc4			; branch if not
		move.l	PIXLV(LA_USERESCINIT,a1),a0	; see if vector set
		jsr	(a0)			; execute user routine

esc4:		move.l	PIXLV(LA_CURDEV,a1),a0		; a0 -> curr device structure
		move.w	DEVPLANES(a0),PIXLV(_v_planes,a1)	; set # of planes
		move.w	DEVLINEWRAP(a0),PIXLV(_v_lin_wr,a1)	; set line wrap
		move.w	DEVLINEWRAP(a0),PIXLV(_bytes_lin,a1)	; set line wrap
		move.w	DEVYREZ(a0),PIXLV(v_vt_rez,a1)	; set vertical rez
		move.w	DEVXREZ(a0),PIXLV(v_hz_rez,a1)	; set horizontal rez
		tst.l	DEVVIDADR(a0)			; see if we need to set
		beq	esc2				; if not then skip
		move.l	DEVVIDADR(a0),_v_bas_ad		; set video base adr

esc2:
#if VIDEL_SUPPORT
		move.w	#0,byt_per_pix(a1)		; assume 0 bytes/pix
		move.w	DEVFORMID(a0),form_id(a1)   	; init mem config flag 
		cmp.w	#PIXPACKED,DEVFORMID(a0)     	; see if in pix packed
		bne	esc0
		move.w	PIXLV(_v_planes,a1),d1	; # of planes
		lsr.w	#3,d1			; d1 <- # of bytes per pix
		move.w	d1,PIXLV(byt_per_pix,a1)	; load # bytes per pix
#endif

esc0:		lea	PIXLV(save_block,a1),a2	; a0 -> save_block
		cmp.w	#4,PIXLV(_v_planes,a1)	; see if we have > 4 planes
		ble	esc1			; if so point to > save buff
		lea	PIXLV(_save_block,a1),a2	; a0 -> save_block
esc1:	move.l	a2,PIXLV(sv_blk_ptr,a1) 	; init ptr to pnt to save_block

		move.l	DEVFNTPTR(a0),a0	; a0 -> desired system font

		jsr	gl_f_init		; init the glob font variables

		PIXLA(a1)
		moveq.l	#0,d0
		move.w	d0,PIXLV(v_col_fg,a1)		; foreground color is all ones
#if VIDEL_SUPPORT
		move.l	d0,vl_col_fg(a1)	; set in case we are 32 bit mode
#endif
		move.w	d0,PIXLV(v_cur_cx,a1)
		move.w	d0,PIXLV(v_cur_cy,a1)
		move.w	d0,PIXLV(v_cur_off,a1)
		move.b	d0,PIXLV(v_delay,a1)		; cursor redisplay interval (immediate)

		not.l	d0
		move.w	d0,PIXLV(v_col_bg,a1)		; background color is all zeros
#if VIDEL_SUPPORT
		move.l	d0,PIXLV(vl_col_bg,a1)	; set in case we are 32 bit mode
#endif

		move.l	PIXLV(LA_CURDEV,a1),a0		; a0 -> curr device structure
		tst.w	DEVLOOKUPTABLE(a0)     	; see if there is a lookup tble
		beq	esc3			; if not then skip
		not.w	PIXLV(v_col_fg,a1)		; in a lookup table we need
#if VIDEL_SUPPORT
		not.l	PIXLV(vl_col_fg,a1)		;	to switch foreground
#endif
		not.w	PIXLV(v_col_bg,a1)		;	and background
#if VIDEL_SUPPORT
		not.l	PIXLV(vl_col_bg,a1)
#endif

esc3:		move.l	_v_bas_ad,PIXLV(v_cur_ad,a1)	; home cursor.
		move.l	#-1,PIXLV(col_and_mask,a1)	; init "and" mask for vs_color

		moveq.l	#1,d0
		move.b	d0,PIXLV(v_stat_0,a1)		; flash, nowrap, normal video.
		move.w	d0,PIXLV(disab_cnt,a1)	; cursor disabled 1 level deep.

		move.l	#30,d0
		move.b	d0,PIXLV(v_cur_tim,a1)	;.5 sec blink rate (@60 Hz vbl)
		move.b	d0,PIXLV(vct_init,a1)		;.5 sec blink rate (@60 Hz vbl)

		move.l	#normal_ascii,con_state ; Init conout state machine

		move.w	PIXLV(v_hz_rez,a1),d0		; d0 <- horizontal rez
		lsr.w	#1,d0			; d0 <- x rez / 2
		move.w	d0,PIXLV(_GCURX,a1)		; init mouse X position

		move.w	PIXLV(v_vt_rez,a1),d0		; d0 <- vrtical rez
		lsr.w	#1,d0			; d0 <- y rez / 2
		move.w	d0,PIXLV(_GCURY,a1)		; init mouse Y position

		move.w	#1,-(sp)		; try set Blit mode to hard
		move.w	#BLITMODE,-(sp)		; push funct #
		trap	#XBIOS
		addq.l	#4,sp			; cleanup stack

		jsr	_InitDevTabInqTab	; initialize more variables

		bra	_v_clrwk		; blank screen. ret from there
