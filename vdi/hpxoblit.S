*********************************  sbblit.s  **********************************
*
* =============================================================================
* $Author: lozben $	$Date: 91/08/06 17:34:28 $
* =============================================================================
*
*******************************************************************************


#include "config.h"
#include "vdidefs.h"
#include "lineaequ.inc"
#include "asmdefs.h"

#if VIDEL_SUPPORT

#include "sbframe.inc"
#include "blaster.inc"

*******************************************************************************
*******************************************************************************
**
**	s_xy2addr:
**
**		input:	d0.w =  x coordinate
**			d1.w =  y coordinate
**			a3.l -> line variable structure
**			a6.l -> frame
**
**		output:	a0.l -> address of word containing x,y
**
**
**	d_xy2addr:
**
**		input:	d0.w =  x coordinate.
**			d1.w =  y coordinate.
**			a3.l -> line variable structure
**			a6.l -> frame
**
**		output:	a1.l -> address of word containing x,y
**
**		destroys: d0,d1
**
*******************************************************************************
*******************************************************************************

s_xy2addr:	move.l	S_FORM(a6),a0		; a0 -> strt of srce form (0,0)
		mulu	byt_per_pix(a3),d0	; d0 <- x portion of offset
		mulu	S_NXLN(a6),d1		; d1 <- y portion of offset
		add.l	d0,d1			; d1 <- byte offs into mem frm
		add.l	d1,a0			; a0 -> (x,y)
		rts

d_xy2addr:	move.l	D_FORM(a6),a1		; a0 -> strt of dst form (0,0)
		mulu	byt_per_pix(a3),d0	; d0 <- x portion of offset
		mulu	D_NXLN(a6),d1		; d1 <- y portion of offset
		add.l	d0,d1			; d1 <- byte offs into mem frm
		add.l	d1,a1			; a0 -> (x,y)
		rts


*+
* purpose:	set up parameters for bitblt and thread together the
*		appropriate bitblt fragments for software implementation
*		of bitblt. We are dealing with opaque case.
*
* in:		d0.w	Xmin source
*		d2.w	Xmin destination
*		d4.w	Xmax source
*		d6.w	Xmax destination
*
*		a6.l	points to frame with following parameters set:
*
*		Ymin source, Ymax source, Ymin destination, Ymax destination
*		Xmin source, Xmax source, Xmin destination, Xmax destination
*		rectangle height, rectangle width, number of planes
*-
		xdef	hpx_bitblt
hpx_bitblt:
	tst.w	S_NXPL(a6)		; see if we are doing transparent blit
	bne	skip0			; if not then skip
	jmp	spx_tbitblt		; else do transparent blit

skip0:	move.l	_lineAvars,a3		; a3 -> linea variable structure
	lea	BLASTER+B_OFF,a5	; a5 -> SKEW (bit blt reference reg)
	move.w	S_YMIN(a6),d1		; d1 <- Ymin source
	bsr	s_xy2addr		; a0 -> start of source block

	move.w	D_XMIN(a6),d0		; compute address of source block
	move.w	D_YMIN(a6),d1
	bsr	d_xy2addr		; a1 -> start of destination block

	cmp.l	a1,a0			; which address is larger: src or dst
	blt	do_neg_case		; if source < dest then do neg case

	move.w	B_WD(a6),B_D_SPAN(a5)	; set span in words
	ble	leave			; leave if span is zero
	move.w	B_HT(a6),B_HEIGHT(a5)	; set height in lines
	ble	leave			; leave if number of lines is zero

	move.w	#2,B_S_NXWD(a5)		; set offset to next src word
	move.w	S_NXLN(a6),d5		; d5 <- src line wrap (in bytes)
	lsr.w	#1,d5			; d5 <- src line wrap (in words)
	sub.w	B_WD(a6),d5		; d5 <- source linewrap - span
	addq.w	#1,d5			; last word fetch does increment
	lsl.w	#1,d5			; offset to nxt src line in bytes
	move.w	d5,B_S_WRAP(a5)		; wrap from end of one src line to nxt
	move.l	a0,B_S_ADDR(a5)		; set starting src adr

	move.w	#2,B_D_NXWD(a5)		; set offset to next dst word
	move.w	D_NXLN(a6),d5		; d5 <- dst line wrap (in bytes)
	lsr.w	#1,d5			; d5 <- dst line wrap (in words)
	sub.w	B_WD(a6),d5		; d5 <- destination linewrap - span
	addq.w	#1,d5			; last word fetch does increment
	lsl.w	#1,d5			; offset to nxt dst line in bytes
	move.w	d5,B_D_WRAP(a5)		; wrap from end of one dst line to nxt
	move.l	a1,B_D_ADDR(a5)		; set starting dst adr

	move.w	#-1,B_F1_MSK(a5)	; set up the 1st fringe mask
	move.w	#-1,B_CT_MSK(a5)	; set up the center mask
	move.w	#-1,B_F2_MSK(a5)	; set up the 2nd fringe mask

	move.b	#2,B_LOGOP(a5)			; set halftone op to source
	move.b	OP_TAB(a6),B_LOGOP+1(a5)	; set the blit logic op

*******************************************************************************
	move.w	#$8000,(a5)		; load control word (GO:1 HOG:0)
*******************************************************************************

blt_p_sync:

	tas	(a5)			; attempt to restart blt
	nop				; this is executed prior to restart
	bmi	blt_p_sync		; quit if blt had finished

#if P68030
	CACR_D0			; d2 <- cache control register
	bset.l	#11,d0			; set "Clr DATA Cache" bit
	D0_CACR			; clear the data cache
#endif

leave:	rts

******************************************************************************
************************** 16 bit case negative case *************************
******************************************************************************

do_neg_case:

	move.w	S_XMAX(a6),d0		; d0 <- Xmax source
	move.w	S_YMAX(a6),d1		; d1 <- Ymax source
	bsr	s_xy2addr		; a0 -> start of source block

	move.w	D_XMAX(a6),d0		; d0 <- Xmax source
	move.w	D_YMAX(a6),d1		; d1 <- Ymax source
	bsr	d_xy2addr		; a1 -> start of destination block

	move.w	B_WD(a6),B_D_SPAN(a5)	; set span in words
	ble	leave			; leave if span is zero
	move.w	B_HT(a6),B_HEIGHT(a5)	; set height in lines
	ble	leave			; leave if number of lines is zero

	move.w	#-2,B_S_NXWD(a5)		; set offset to next src word
	move.w	S_NXLN(a6),d5		; d5 <- src line wrap (in bytes)
	lsr.w	#1,d5			; d5 <- src line wrap (in words)
	sub.w	B_WD(a6),d5		; d5 <- source linewrap - span
	lsl.w	#1,d5			; offset to nxt src line in bytes
	neg.w	d5			; we are going in neg direction
	move.w	d5,B_S_WRAP(a5)		; wrap from end of one src line to nxt
	move.l	a0,B_S_ADDR(a5)		; set starting src adr

	move.w	#-2,B_D_NXWD(a5)	; set offset to next dst word
	move.w	D_NXLN(a6),d5		; d5 <- dst line wrap (in bytes)
	lsr.w	#1,d5			; d5 <- dst line wrap (in words)
	sub.w	B_WD(a6),d5		; d5 <- destination linewrap - span
	addq.w	#1,d5			; last word fetch does increment
	lsl.w	#1,d5			; offset to nxt dst line in bytes
	neg.w	d5			; we are going in neg direction
	move.w	d5,B_D_WRAP(a5)		; wrap from end of one dst line to nxt
	move.l	a1,B_D_ADDR(a5)		; set starting dst adr

	move.w	#-1,B_F1_MSK(a5)	; set up the 1st fringe mask
	move.w	#-1,B_CT_MSK(a5)	; set up the center mask
	move.w	#-1,B_F2_MSK(a5)	; set up the 2nd fringe mask

	move.b	#2,B_LOGOP(a5)			; set halftone op to source
	move.b	OP_TAB(a6),B_LOGOP+1(a5)	; set the blit logic op

*******************************************************************************
 	move.w	#$8080,(a5)		; load control word (GO:1 HOG:0)
*******************************************************************************

blt_n_sync:

	tas	(a5)			; attempt to restart blt
	nop				; this is executed prior to restart
	bmi	blt_n_sync		; quit if blt had finished

#if P68030
	CACR_D0			; d2 <- cache control register
	bset.l	#11,d0			; set "Clr DATA Cache" bit
	D0_CACR			; clear the data cache
#endif
	rts

#endif
