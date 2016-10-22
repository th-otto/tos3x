*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/gsx2.s,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:27:45 $	$Locker: kbad $
* =======================================================================
*  $Log:	gsx2.s,v $
* Revision 2.2  89/04/26  18:27:45  mui
* TT
* 
* Revision 2.1  89/02/22  05:30:14  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:35:25  lozben
* Initial revision
* 
*************************************************************************
*******************************************************************************
*
*	GSX2.S - GSX entry to GIOS and support for GSXBIND.C
*
* Author:
*	Steve Cavender
*
* Date:
*	1 October 1984
*
* Last Modified:
*	03 Oct 84
*	01 Nov 84 lkw
*	19 Jan 87 jde
*	
*
*******************************************************************************
*	Use MAC to assemble			6/28/90		D.Mui
* assemble with MAS 900801 kbad


	xdef	_gsx2
_gsx2:
	lea	pblock,a0	; a0 -> pblock
	move.l	#_contrl,(a0)	; set up control array pointer
	move.l	a0,d1		; d1 -> pblock
	moveq.l	#115,d0	; d0 <- VDI opcode
	trap	#2
	rts


*  in:
*	sp+04	m1
*	sp+06	m2
*	sp+08	d1

	xdef	_MUL_DIV
_MUL_DIV:
	xdef	_mul_div
_mul_div:

	move.w	6(sp),d0	;	 m2
	add.w	d0,d0		;      2*m2
	muls	4(sp),d0	;   m1*2*m2
	divs	8(sp),d0	;  (m1*2*m2)/d1
	bmi	md_1

	addq.w	#1,d0		;  ((m1*2*m2)/d1)+1
	asr.w	#1,d0		; (((m1*2*m2)/d1)+1)/2
	rts

md_1:	subq.w	#1,d0		;  ((m1*2*m2)/d1)-1
	asr.w	#1,d0		; (((m1*2*m2)/d1)-1)/2
	rts


*	xdef	_umul_div
*_umul_div:
*
*	move.w	6(sp),d0	;         m2
*	add.w	d0,d0		;       2*m2
*	mulu	4(sp),d0	;    m1*2*m2
*	divu	8(sp),d0	;   (m1*2*m2)/d1
*	addq.w	#1,d0		;  ((m1*2*m2)/d1)+1
*	asr.w	#1,d0		; (((m1*2*m2)/d1)+1)/2
*	rts


	xdef	_i_ptsin
_i_ptsin:

	move.l	4(sp),pioff
	rts


	xdef	_i_intin
_i_intin:

	move.l	4(sp),iioff
	rts


	xdef	_i_ptsout
_i_ptsout:

	move.l	4(sp),pooff
	rts


	xdef	_i_intout
_i_intout:

	move.l	4(sp),iooff
	rts


	xdef	_i_ptr
_i_ptr:
	xdef	_i_lptr1
_i_lptr1:

	move.l	4(sp),_contrl+14
	rts


	xdef	_i_ptr2
_i_ptr2:

	move.l	4(sp),_contrl+18
	rts


	xdef	_m_lptr2
_m_lptr2:

	move.l	4(sp),a0
	move.l	_contrl+18,(a0)
	rts



	bss

	xdef	pblock
pblock:	ds.l	1
	xdef	iioff
iioff:	ds.l	1
	xdef	pioff
pioff:	ds.l	1
	xdef	iooff
iooff:	ds.l	1
	xdef	pooff
pooff:	ds.l	1

