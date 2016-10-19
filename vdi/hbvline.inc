****************************  hbline.s  ********************************
*
* =======================================================================
* $Author: lozben $	$Date: 91/02/13 18:34:26 $
* =======================================================================
*
* Revision 3.2  91/02/13  18:34:26  lozben
* Adjusted the spelling of some of the linea equates to correspond
* to the ones in the lineaequ.s file.
* 
* Revision 3.1  91/01/31  16:20:29  lozben
* Adjusted the code to work with the multiple linea variable structures.
* 
* Revision 3.0  91/01/03  15:10:50  lozben
* New generation VDI
* 
* Revision 2.3  90/02/23  18:22:21  lozben
* We now use FG_B_PLANES as our color index instead of _FG_BP_{0-7}.
* 
* Revision 2.2  89/02/27  17:45:13  lozben
* *** initial revision ***
* 
*************************************************************************


#include "../common/config.h"
#include "vdidefs.h"
#include "lineaequ.inc"
#include "asmdefs.h"

#if BLITTER_SUPPORT /* whole file */

		xref	BLASTER

#ifndef B_OFF /* must match definitions in hbhline.inc */
/* offset to reference register */
#define B_OFF		 60

B_PATTERN	equ	-60 		; pattern register file
B_S_NXWD	equ	-28 		; offset to next src word
B_S_WRAP	equ	-26 		; wrap from end of one src line to next
B_S_ADDR	equ	-24 		; initial src address
B_F1_MSK	equ	-20 		; 1st fringe mask	0:old 1:new
B_CT_MSK	equ	-18 		; center mask		0:old 1:new
B_F2_MSK	equ	-16 		; 2nd fringe mask	0:old 1:new
B_D_NXWD	equ	-14 		; offset to next dst word
B_D_WRAP	equ	-12 		; wrap from end of one dst line to next
B_D_ADDR	equ	-10 		; initial dst address
B_D_SPAN	equ	-6 		; span in words of dst block
B_HEIGHT	equ	-4 		; pixel height of block
B_LOGOP		equ	-2 		; ctrl word: src^pat combo & logic op #
B_SKEW		equ	 0		; ctrl word: flags, pat index & skew


BF_PAT		equ  	8		; combo flag: 0:all ones  1:pattern
BF_SRC		equ  	9		; combo flag: 0:all ones  1:source
BM_PAT		equ  	$0100		; combo mask: 0:all ones  1:pattern
BM_SRC		equ  	$0200		; combo mask: 0:all ones  1:source
                     
                     
BF_GO		equ  	15		; execution flag         0:false 1:true
BF_HOG		equ  	14		; hog dma flag		 0:false 1:true
BF_SM		equ  	13		; smudge flag		 0:false 1:true
BF_PF		equ  	7		; pre-fetch flag  	 0:false 1:true
BF_FQ		equ  	6		; flush queue flag 	 0:false 1:true
BM_GO		equ  	$8000		; execution mask	
BM_HOG		equ  	$4000		; hog dma mask
BM_SM		equ  	$2000		; smudge mask
BM_PF		equ  	$0080		; pre-fetch mask
BM_FQ		equ  	$0040		; flush queue mask
#endif

	text

*+
*	Hard Vertical Line Code
*
*  in:
*	d4	X
*	d5	Y1
*	d6	X
*	d7	Y2
*
*	a4	pointer to LineA variable base
*-
		xdef	hb_vline
hb_vline:

	lea	BLASTER+B_OFF,a5	; a5 -> BLASTER reference register

	not.w	d4
	and.w	#$000F,d4		; d4 <- bit number within word
	clr.w	d0
	bset.l	d4,d0			; d0 <- pixel mask   0:DST 1:LINE
	move.w	d0,B_F1_MSK(a5)		; load the fringe mask

	move.w	LV(v_planes,a4),d0	; d0 <- planes
	move.w	d0,d1
	add.w	d1,d1			; d1 <- offset between words
	subq.w	#1,d0			; d0 <- dbra plane counter

	move.l	_v_bas_ad,a1		; a1 -> start of destination form
	asr.w	#4,d6			; d6 <- X/16
	muls	d6,d1			; d1 <- byte offset to X
	add.l	d1,a1			; a1 -> dst (X,0)

	move.w	LV(bytes_lin,a4),d2		; d2 <- screen width
	muls	d5,d2			; d2 <- offset to (0,Y1)
	add.l	d2,a1			; a1 -> (X,Y1)

	moveq.l	#7,d2			; d2 <- dbra loop counter

	move.w	LV(v_lin_wr,a4),d1	; d1 <- offset between lines

	lea	LV(LN_MASK,a4),a3		; a3 -> line style mask
	move.w	(a3),d3			; d3 <- line style mask

	sub.w	d5,d7			; d7 <- dY
	bge	vl_top


vl_bot:	neg.w	d7			; d7 <- absolute value of dY
	neg.w	d1			; d1 <- offset to next line up
	lea	B_PATTERN+32(a5),a0	; a0 -> last pattern register

vl_b2t:	add.w	d3,d3			; cy <- next style bit
	subx.w	d4,d4			; cy:0 => d0<-0000  cy:1 => d0<-FFFF
	move.w	d4,-(a0)		; load register
	add.w	d3,d3
	subx.w	d4,d4
	move.w	d4,-(a0)
	dbra	d2,vl_b2t

	move.w	#BM_GO+$0F00,d2		; d2 <- SKEW word (start on last line)
	bra	vl_wrap



vl_top:	lea	B_PATTERN(a5),a0	; a0 -> 1st pattern register

vl_t2b:	add.w	d3,d3			; cy <- next style bit
	subx.w	d4,d4			; cy:0 => d0<-0000  cy:1 => d0<-FFFF
	move.w	d4,(a0)+		; load pattern register
	add.w	d3,d3
	subx.w	d4,d4
	move.w	d4,(a0)+
	dbra	d2,vl_t2b		; next 2 registers

	move.w	#BM_GO+$0000,d2		; d2 <- initial pattern index


vl_wrap:

	move.w	d1,B_D_WRAP(a5)		; store destination wrap
	addq.w	#1,d7			; d7  <- dY+1: height

	lea	op_table,a2
	move.w	LV(WRT_MODE,a4),d1	; d1 <- writing mode
	lsl.w	#2,d1			; d1 <- offset into main table
	add.w	d1,a2			; a2 -> op code table for this mode

	subq.w	#8,d1			; if it's XOR mode ...
	bne	vl_newmask		
	tst.w	LV(LSTLIN,a4)		; and it's not the last line ...
	bne	vl_newmask

	subq.w	#1,d7			; don't plot last pixel

vl_newmask:

	move.w	d7,d1			; d1 <- line length
	move.w	(a3),d3			; d3 <- pattern for this line
	and.w	#$000F,d1		; d1 <- realignment factor
	rol.w	d1,d3			; d3 <- pattern aligned for next line
	move.w	d3,(a3)			; save it back to LN_MASK

        move.l  LV(FG_B_PLANES,a4),d5	; d5 <- color index
	moveq.l	#2,d4			; d4 <- mask for op_table offset
	move.w	#1,B_D_SPAN(a5)		; one word span

vl_loop:

	move.l	a1,B_D_ADDR(a5)		; next plane
	move.w	d7,B_HEIGHT(a5)		; line height

	ror.l	#1,d5			; what color is this plane ?
	scs	d3
	and.w	d4,d3
	move.w	0(a2,d3.w),B_LOGOP(a5)


*******************************************************************************
	move.w	d2,(a5)			; load the SKEW register and fly
*******************************************************************************

vl_sync:

	tas	(a5)
	nop
	bmi	vl_sync

vl_cont:

	addq.w	#2,a1			; a1 -> start of next plane
	dbra	d0,vl_loop		; do the next plane

#if P68030
	CACR_D0			; d0 <- cache control register
	bset.l	#11,d0			; set "Clr DATA Cache" bit
	D0_CACR			; clear the data cache
#endif
	rts


		xdef	op_table
op_table:  *	 color=0   color=1

	dc.w	BM_PAT+00,BM_PAT+03	; replace
	dc.w	BM_PAT+04,BM_PAT+07	; transparent
	dc.w	BM_PAT+06,BM_PAT+06	; xor
	dc.w	BM_PAT+01,BM_PAT+13	; reverse transparent

#endif /* BLITTER_SUPPORT */
