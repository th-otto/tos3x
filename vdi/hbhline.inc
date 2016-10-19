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

#ifndef B_OFF /* must match definitions in hbvline.inc */
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
*	Hard Horizontal Line Code
* in:
*	d0	offset between pattern words
*	d1	word containing X1
*	d2	span-1 (words)
*	d3	mod 16 mask
*	d4	left fringe mask
*	d5	Y1
*	d6	right fringe mask
*
*	a0	points to 1st pattern word
*	a4	points to line A variables base address  (v_planes)
*-
		xdef	hb_hline
hb_hline:

	lea	BLASTER+B_OFF,a5	; a5 -> bit blt reference register
	clr.w	B_S_NXWD(a5)		; proceed from left to right

	addq.w	#1,d2			; d2 <- span
	move.w	d2,B_D_SPAN(a5)

	lea	B_F1_MSK(a5),a3
	move.w	 d4,(a3)+		; B_F1_MSK <- left fringe
	move.w	#-1,(a3)+		; B_CT_MSK <- center fringe
	move.w	 d6,(a3)+		; B_F2_MSK <- right fringe

	move.w	(a4),d2			; d2 <- planes		( v_planes   )
	move.w	d2,d7
	subq.w	#1,d7			; d7 <- dbra plane counter

	add.w	d2,d2			; d2 <- offset between words in plane
	move.w	d2,(a3)+		; load B_D_NXWD

	move.l	_v_bas_ad,a1		; a1 -> start of dst form
	muls	LV(bytes_lin,a4),d5	; d5 <- offset to 1st row of dst
	add.l	d5,a1			; a1 -> row containing line

	muls	d2,d1			; d1 <- offset to word containing X1
	add.l	d1,a1			; a1 -> 1st word of line

	lea	B_PATTERN(a5),a2	; a2 -> 1st pattern register
        move.l  LV(FG_B_PLANES,a4),d5	; d5 <- color index

	move.w	LV(WRT_MODE,a4),d2	; d2 <- writing mode (0-3)
	add.w	d2,d2
	add.w	d2,d2			; d2 <- offset to writing mode table
	lea	op_table(pc),a4		; a4 -> 1st logic op table
	add.w	d2,a4			; a4 -> writing mode table

	moveq.l	#2,d3			; d3 <- logic op offset mask
	move.w	#BM_GO,d4		; d4 <- SKEW word
	moveq.l	#1,d6			; d6 <- height


hl_loop:

	move.w	(a0),(a2)		; load the pattern into BLASTER
	add.w	d0,a0			; a0 -> next pattern word

	move.l	a1,B_D_ADDR(a5)		; new dst plane
	move.w	d6,B_HEIGHT(a5)		; height of 1

	ror.l	#1,d5			; what color is this plane ?
	scs	d2
	and.w	d3,d2			; d2 <- logic op offset: 0=>0  1=>2
	move.w	0(a4,d2.w),B_LOGOP(a5)	; load the logic op

*******************************************************************************
	move.w	d4,(a5)			; start the BLASTER
*******************************************************************************

hl_sync:

	tas	(a5)
	nop
	bmi	hl_sync

hl_cont:

	addq.l	#2,a1			; a1 -> next dst plane
	dbra	d7,hl_loop

#if P68030
	CACR_D7			; d7 <- cache control register
	bset.l	#11,d7			; set "Clr DATA Cache" bit
	D7_CACR			; clear the data cache
#endif
	rts


#endif /* BLITTER_SUPPORT */
