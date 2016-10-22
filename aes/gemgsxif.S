*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/gemgsxif.s,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:23:50 $	$Locker: kbad $
* =======================================================================
*  $Log:	gemgsxif.s,v $
* Revision 2.2  89/04/26  18:23:50  mui
* TT
* 
* Revision 2.1  89/02/22  05:27:16  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.2  88/10/24  10:34:08  mui
* move gsx_malloc to geminit.c
* 
* Revision 1.1  88/06/02  12:34:09  lozben
* Initial revision
* 
*************************************************************************
*	Translated by J.Eisenstein	1/19/87
*	Fix	1/21/87			Derek Mui, J.Eisenstein
*	Fix at gsx_mret	4/28/1988	D.Mui	
*	Remove gsx_mfset	5/8/91	D.Mui
*	Add gsx_wsopen for six resolutions 4/25/89	D.Mui
* 	Assemble with MAS 900801 kbad
*	Check for sparrow resolution at gsx_wsopen	7/19/92	D.Mui	

	xdef	_gl_tmp
	comm	_gl_tmp,20
	xdef	_old_mcod
	comm 	_old_mcod,4
	xdef	_old_bcod
	comm	_old_bcod,4
	xdef	_gl_moff
	comm	_gl_moff,2
	xdef	_gl_mlen
	comm	_gl_mlen,4
	xdef	_gl_graph
	comm	_gl_graph,2
	xdef	_gl_mouse
	comm	_gl_mouse,2


GSX_MFS	equ	 0*3
V_OPNWK	equ	 1*3
VST_CLP	equ	 2*3
VST_HGT	equ	 3*3
VR_RECT	equ	 4*3
VRO_CPY	equ	 5*3
VRT_CPY	equ	 6*3
VSL_WID	equ	 7*3


*	xdef _gsx_malloc
*
*_gsx_malloc:
*
*	clr.l	-(sp)
*	clr.l	-(sp)
*	pea	_gl_tmp
*	jsr	_gsx_fix
*
*	move.l	#$00003400,_gl_mlen
*	pea	 $3400
*	jsr	_dos_all
*	move.l	 d0,_gl_tmp
*
*	lea	16(sp),sp		; clean up stack
*	rts



	xdef _gsx_mfree

_gsx_mfree:
	tst.l	_gl_tmp			; Fix 8/3/92
	beq	g1
	move.l	_gl_tmp,-(sp)		; Fix 1/20/87
	jsr	_dos_fre
	addq.l	#4,sp
g1:
	rts



	xdef _gsx_mret

*  in:
*	sp+04	pmaddr
*	sp+08	pmlen

_gsx_mret:

	move.l	4(sp),a0
	move.l	_gl_tmp,(a0)
	move.l  8(sp),a0
	move.l _gl_mlen,(a0)
	rts



	xdef _gsx_ncode

*  in:
*	sp+04	code
*	  +06	n
*	  +08	m

_gsx_ncode:

	lea	4(sp),a0
	lea	_contrl,a1

	move.l	(a0)+,(a1)+
	addq.l	#2,a1
	move.w	(a0)+,(a1)
	addq.l	#6,a1
	move.w	_gl_handl,(a1)

	jmp	_gsx2



	xdef _gsx_1code

*  in:
*	sp+04	code
*	  +06	value

_gsx_1code:

	move.w  6(sp),_intin

	move.l	#$00000001,-(sp)	; Fix 1/20/87
	move.w	4+4(sp),-(sp)
	bsr	_gsx_ncode

	addq.l	#6,sp
	rts



	xdef _gsx_init

_gsx_init:

	bsr	_gsx_wsopen
	jsr	_gsx_start
	bsr	setmb

	moveq.l	#124,d0			; code 124 => vq_mouse
	bsr	vdi_short

	lea	_ptsout,a0
	move.w	(a0)+,_xrat
	move.w  (a0)+,_yrat

	rts




	xdef _gsx_grap

*  in:
*	sp+04	tographic

_gsx_grap:

	move.w	4(sp),d0
	lea	_gl_graph,a1
	cmp.w	(a1),d0
	beq	L8

	move.w	d0,(a1)
	beq	L9

	moveq.l #2,d0
	bsr	escape

setmb:	pea	_drwaddr
	pea	_far_mchange
	pea	_far_bchange
	bsr	_gsx_setmb
	lea	12(sp),sp

L8:	rts


L9:	moveq.l	#3,d0
	bsr	escape
	bra	_gsx_resetmb




	xdef _v_hardcopy

_v_hardcopy:

	moveq.l	#17,d0
	bra	escape




	xdef _gsx_esca

*  in:
*	sp+04	esc_code

_gsx_esca:

	move.w	4(sp),d0

escape:	move.w	d0,_contrl+10
	moveq.l	#005,d0			; code 005 => escape
	bra	vdi_short



*****	 Open physical work station for 6 resoultions	*****

	xdef _gsx_wsopen

_gsx_wsopen:

	lea	_intin,a1
	move.l	a1,a2			; a2 -> intin[0]

	moveq.l	#9,d0

L15:	move.w	#1,(a1)+
	dbra	d0,L15

	move.w	#2,(a1)			; intin[10] <- 2

	lea	restable,a0		; look for the drive handle 
	lea	restype,a1		; load in res table
L209:
	move.w	(a1)+,d0
	tst.w	(a0)+
	cmp.w	_gl_restype,d0
	bne	L209

	move.w	-(a0),(a2)		; intin[0] <- screen handle

	pea	_gl_ws
	pea	_gl_handl
	pea	(a2)
			
*	Check for sparrow machine	
		
*	move.l	_gl_vdo,d0		; check for sparrow 	
*	and.l	#$00030000,d0		; 7/17/92
*	cmp.l	#$00030000,d0
*	bne	L20_3
*	lea	_intin,a0
*	move.w	#5,(a0)
*	lea	_intout,a0
*	move.w	_d_rezword,(a0)
	jsr	_setres			; check mode code
L20_3:

	bsr	_v_opnwk
	lea	12(sp),sp

	lea	_gl_ws,a1
	lea	_gl_restype,a2

L20:	
	move.w	#5,(a2)		; default to 1280 x 960 
	
	cmp.w	#639,(a1)	; check for x = 640 ?
	bne	L18
	tst.w	(a1)+
	cmp.w	#199,(a1)	; 640 x 200
	bne	L20_1
	move.w	#2,(a2)
	bra	L22
	
L20_1:	cmp.w	#399,(a1)	; 640 x 400
	bne	L20_2
	move.w	#3,(a2)
	bra	L22
	
L20_2:	move.w	#4,(a2)		; 640 x 480
	bra	L22	

L18:	cmp.w	#319,(a1)+	; check for x = 320 ?
	bne	L22	
	cmp.w	#199,(a1)	; check for y = 200 ?
	bne	L18_1
	move.w	#1,(a2)		; Yes		 
	bra	L22

L18_1:	move.w	#6,(a2)		; No it is 320 x 480

L22:	clr.w	_gl_rsch	; gl_rschange  <- FALSE
	move.w	#1,_gl_graph	; gl_graphic   <- TRUE
	rts



	xdef _gsx_wscl

_gsx_wscl:

	moveq.l	#002,d0			; code 002 => v_clswk
	bra	vdi_short



	xdef _ratinit

_ratinit:

	clr.w	     -(sp)
	move.w	#122,-(sp)		; show cursor: force show
	bsr	_gsx_1code
	addq.l	#4,sp

	clr.w	_gl_moff
	rts


	xdef _bb_set

*  call:
*
*	bb_set (sx, sy, sw, sh, pts1, pts2, pfd, psrc, pdst)
*
*  in:
*	sp+04	 sx		word
*	  +06	 sy		word
*	  +08	 sw		word
*	  +10	 sh		word
*	  +12	*pts1		points to word array
*	  +16	*pts2		points to word array
*	  +20	*pfd		points to MFDB
*	  +24	*psrc		points to MFDB
*	  +28	*pdst		points to MFDB

_bb_set:

	lea	4(sp),a0		; a0 -> stack resident parameters

	move.w	(a0)+,d0		; d0[15:00] <- sx  (XminSRC)
	move.l	(a0)+,d1		; d1[31:16] <- sy  (YminSRC)
*					; d1[15:00] <- sw  (width)
	move.w	(a0)+,d2		; d2[15:00] <- sh  (height)

	add.w	d0,d1			; d1 <- sx + sw = XmaxSRC + 1
	subq.w	#1,d1			; d1 <- XmaxSRC

	lsr.w	#4,d0			; d0 <- XminSRC/16
	lsr.w	#4,d1			; d1 <- XmaxSRC/16
	sub.w	d0,d1			; d1 <- width-1	(in words)
	addq.w	#1,d1			; d1 <- width   (in words)

	lea	_gl_tmp+10,a1		; a1 -> gl_tmp.fd_stand

	move.w	#01,(a1)		; gl_tmp.fd_stand   <- TRUE
	move.w	d1,-(a1)		; gl_tmp.fd_wdwidth <- width in words
	move.w	d2,-(a1)		; gl_tmp.fd_h       <- height in pixels
	lsl.w	#4,d1
	move.w	d1,-(a1)		; gl_tmp.fd_w       <- width in pixels
	
	move.l	(a0)+,a1		; a1 -> pts1 array
	move.l	(a0)+,a2		; a2 -> pts2 array

	clr.l	(a2)+			; pts2[0] <- XminDST=0
*					; pts2[1] <- YminDST=0
	subq.w	#1,d1			; d1      <- width - 1
	move.w	d1,(a2)+		; pts2[2] <- XmaxDST
	subq.w	#1,d2			; d2      <- height -1
	move.w	d2,(a2)+		; pts2[3] <- YmaxDST

	lsl.w	#4,d0			; d0      <- XminSRC/16 *16
	move.w	d0,(a1)+		; pts1[0] <- XminSRC on word boundry
	add.w	d0,d1			; d1      <- XminSRC + width - 1
	move.l	d1,(a1)+		; pts1[1] <- YminSRC:  d1[31:16]
*					; pts1[2] <- XmaxSRC:  d1[15:00]
	swap	d1			; d1      <- YminSRC
	add.w	d1,d2			; d2      <- YminSRC + height -1
	move.w	d2,(a1)+		; pts1[3] <- YmaxSRC

	clr.l	      -(sp)		; gsx_fix(pfd, 0, 0, 0, 0)
	clr.l	      -(sp)
	move.l	(a0)+,-(sp)		; pfd -> space for MFDB
	jsr	_gsx_fix		; build MFDB

	bsr	_gsx_moff		; hide mouse cursor

	lea	12+28(sp),a0		; a0 -> pdst pointer
	move.l	 (a0),-(sp)		; push *pdst
	move.l	-(a0),-(sp)		; push *psrc
	pea	_ptsin			; push *ptsin
	move.w	#3,-(sp)		; replace mode (S_ONLY)
	bsr	_vro_cpyf

	lea	26(sp),sp		; clean the stack up from earlier calls

	bra	_gsx_mon		; show mouse cursor (RETURN from there)




	xdef _bb_save

*  call:
*
*	bb_save (ps->g_x, ps->g_y, ps->g_w, ps->g_h, &ptsin[0], &ptsin[4].
*		 &gl_src, &gl_src, &gl_tmp)
*
*  in:
*	sp+04	ps

_bb_save:

	pea	_gl_tmp
	lea	_gl_src,a1
	pea	 (a1)
	pea	 (a1)
	pea	_ptsin+08
	pea	_ptsin+00

	move.l	4+20(sp),a0		; a0 -> ps.g_w
	move.l	4(a0),-(sp)		; width and height
	move.l	 (a0),-(sp)		; Xmin  and Ymin
	bsr	_bb_set

	lea	28(sp),sp
	rts




	xdef _bb_resto

*  call:
*
*	bb_restore (pr->g_x, pr->g_y, pr->g_w, pr->g_h, &ptsin[4], &ptsin[0].
*		    &gl_dst, &gl_tmp, &gl_dst)
*
*  in:
*	sp+04	pr

_bb_resto:

	lea	_gl_dst,a1
	pea	 (a1)
	pea	_gl_tmp
	pea	 (a1)
	pea	_ptsin+00
	pea	_ptsin+08

	move.l	4+20(sp),a0		; a0 -> ps.g_w
	move.l	4(a0),-(sp)		; ps.g_w and ps.g_h
	move.l	 (a0),-(sp)		; ps.g_x and ps.g_y
	bsr	_bb_set

	lea	28(sp),sp
	rts




	xdef _gsx_setmb

*  in:
*	sp+04	boff
*	  +08	moff
*	  +12	pdrwaddr	*** not used ***

_gsx_setmb:

	move.l	4(sp),_contrl+14	; contrl[7-8] <- new button vector
	moveq.l	#125,d0
	bsr	vdi_short
	move.l	_contrl+18,_old_bcod

	move.l	8(sp),_contrl+14	; contrl[7-8] <- new motion vector
	moveq.l	#126,d0
	bsr	vdi_short
	move.l	_contrl+18,_old_mcod

	rts



	xdef _gsx_resetmb
_gsx_resetmb:

	move.l	_old_bcod,_contrl+14		; reset button vector
	moveq.l	#125,d0
	bsr	vdi_short
	move.l	_old_mcod,_contrl+14		; reset motion vector
	moveq.l	#126,d0
	bra	vdi_short



	xdef _gsx_tick

*  in:
*	sp+04	 tcode
*	  +08	*ptsave

_gsx_tick:

	move.l	4(sp),_contrl+14

	moveq.l	#118,d0			; code 118 => vex_timv
	bsr	vdi_short

	move.l	8(sp),a0
	move.l	_contrl+18,(a0)

	move.w	_intout,d0
	rts


*	xdef _gsx_mfse

*  in:
*	sp+04	pmfnew

*_gsx_mfse:

*	bsr	_gsx_moff		; hide mouse cursor

*	moveq.l	#36,d0			; number of words
*	move.l	4(sp),a0		; source
*	move.l	_ad_intin,a1		; destination


*L32:	move.w	(a0)+,(a1)+		; load mouse form into intin array
*	dbra	d0,L32

*	moveq.l	#GSX_MFS,d0
*	bsr	vdi_call

*	bra	_gsx_mon		; show mouse cursor



	xdef _gsx_mxmy

*  in:
*	sp+04	*pmx
*	  +08	*pmy

_gsx_mxmy:

	lea	4(sp),a0
	move.l	(a0)+,a1
	move.w	_xrat,(a1)
	move.l	(a0)+,a1
	move.w	_yrat,(a1)
	rts



	xdef _gsx_butt

_gsx_butt:

	move.w	_button,d0
	rts



	xdef _gsx_moff
	xdef _ratexit

_ratexit:
_gsx_moff:

	tst.w	_gl_moff
	bne	L35

	moveq.l	#123,d0			; code 123 => v_hide
	bsr	vdi_short

	clr.w	 _gl_mouse

L35:	add.w	#1,_gl_moff
	rts



	xdef _gsx_mon

_gsx_mon:

	moveq.l	#1,d0
	sub.w	d0,_gl_moff
	bne	L37

	move.w	  d0,-(sp)
	move.w	#122,-(sp)		; show cursor: don't force it
	bsr	_gsx_1code
	addq.l	#4,sp

	move.w	#1,_gl_mouse

L37:	rts




*  in:
*	sp+04	pwork_in
*	sp+08	phandle
*	sp+12	pwork_out

	xdef _v_opnwk
_v_opnwk:

	move.l	04(sp),iioff		; _intin array is pwork_in
	move.l	12(sp),a1		; a1 -> pwork_out
	move.l	a1,iooff		; _intout array is pwork_out
	lea	90(a1),a1		; a1 -> pwork_out+45
	move.l	a1,pooff		; _ptsout array is pwork_out+45

	moveq.l	#V_OPNWK,d0
	bsr	vdi_call

	move.l	08(sp),a1		; a1 -> phandle
	move.w	_contrl+12,(a1)		; phandle <- workstation handle
	
	move.l	#_intin,iioff
	move.l	#_intout,iooff
	move.l	#_ptsout,pooff
	rts



*  in:
*	sp+04	count
*	sp+06	pxyarray

	xdef _v_pline
_v_pline:

	lea	4(sp),a0

	clr.w	      -(sp)
	move.w	(a0)+,-(sp)
	move.w	   #6,-(sp)

	bsr	set_pioff
	bra	vdi_cont



	xdef _vst_clip

*  in:
*	sp+04	clip_flag
*	sp+06	pxyarray

_vst_clip:

	lea	4(sp),a0
	move.w	(a0)+,_intin		; intin[0] <- clip flag
	bsr	set_pioff		; redirect ptsin array pointer
	moveq.l	#VST_CLP,d0
	bra	vdi_call



*  in:
*	sp+04	height
*	  +06	pchr_wid
*	  +10	pchr_hei
*	  +14	pcell_wi
*	  +18	pcell_he

	xdef _vst_height
_vst_height:

	lea	_ptsin,a1
	clr.w	      (a1)+		; ptsin[0] <- 0
	move.w	4(sp),(a1)+		; ptsin[1] <- height

	moveq.l	#VST_HGT,d0
	bsr	vdi_call

	lea	_ptsout,a0
	lea	6(sp),a1

	move.l	(a1)+,a2		; a2 -> pchar_width
	move.w	(a0)+,(a2)		; pchr_width   <- ptsout[0]
	move.l	(a1)+,a2		; a2 -> pchar_height
	move.w	(a0)+,(a2)		; pchr_height  <- ptsout[1]
	move.l	(a1)+,a2		; a2 -> pcell_width
	move.w	(a0)+,(a2)		; pcell_width  <- ptsout[2]
	move.l	(a1)+,a2		; a2 -> pcell_height
	move.w	(a0)+,(a2)		; pcell_height <- ptsout[3]
	rts



*  in:
*	sp+04	pxyarray
*	sp+08	pdesMFDB

	xdef _vr_recfl
_vr_recfl:

	lea	4(sp),a0
	bsr	set_pioff
	move.l	(a0)+,_contrl+14

	moveq.l	#VR_RECT,d0
	bra	vdi_call




*  in:
*	sp+04	wr_mode
*	  +06	pxyarray
*	  +10	psrcMFDB
*	  +14	pdesMFDB

	xdef _vro_cpyf
_vro_cpyf:

	lea	4(sp),a0		; a0 -> parameters on stack
	move.w	(a0)+,_intin		; intin[0] <- writing mode	  4(sp)
	bsr	set_pioff		; initialize ptsin array pointer  6(sp)
	bsr	mfdb_load		; load MFDBs

	moveq.l	#VRO_CPY,d0
	bra	vdi_call




*  in:
*	sp+04	wr_mode
*	  +06	pxyarray
*	  +10	psrcMFDB
*	  +14	pdesMFDB
*	  +18	fgcolor
*	  +20	bgcolor

	xdef _vrt_cpyfm
_vrt_cpyfm:

	lea	  4(sp),a0		; a0 -> parameters on stack
	lea	 _intin,a2		; a2 -> intin array
	
	move.w	(a0)+,(a2)+		; intin[0] <- writing mode        4(sp)
	bsr	set_pioff		; initialize ptsin pointer	  6(sp)
	bsr	mfdb_load		; load MFDBs
	move.l	(a0)+,(a2)+		; intin[1] <- foreground color	 18(sp)
*					; intin[2] <- background color   20(sp)
	moveq.l	#VRT_CPY,d0
	bra	vdi_call



	xdef _vrn_trnf

*  in:
*	sp+04	psrcMFDB
*	  +08	pdesMFDB

_vrn_trnf:

	lea	 4(sp),a0		; a0 -> stack parameters
	bsr	mfdb_load

	moveq.l	#110,d0			; code 110 => vr_trn_fm
	bra	vdi_short



	xdef _vsl_width

*  in:
*	sp+04	width

_vsl_width:

	lea	_ptsin,a1
	move.w	4(sp),(a1)+		; ptsin[0] <- width
	clr.w	      (a1)		; ptsin[1] <- 0

	moveq.l	#VSL_WID,d0
	bra	vdi_call




* in:
*	a0	pointer to SRC and DST MFDBs

mfdb_load:

	lea	_contrl+14,a1		; a1 -> contrl[7]
	move.l (a0)+,(a1)+		; contrl[7] -> SRC MFDB
	move.l (a0)+,(a1)+		; contrl[8] -> DST MFDB
	rts



* in:
*	a0	pointer to ptsin array

set_pioff:

	move.l	(a0)+,pioff
	rts



* in:
*	d0	vdi op code

vdi_short:

	clr.l	-(sp)
	bra	vdi_sh


* in:
*	d0	offset into vdi_list

vdi_call:

	lea	vdi_list(pc,d0.w),a2

	clr.w	d0
	move.b	(a2)+,d0
	move.w	d0,-(sp)
	move.b	(a2)+,d0
	move.w	d0,-(sp)
	move.b	(a2)+,d0
vdi_sh:	move.w	d0,-(sp)

vdi_cont:

	jsr	_gsx_ncode
	addq.l	#6,sp
	move.l	#_ptsin,pioff		; some routines alter this pointer
	rts				; so we shall always reset it.

vdi_list:	

	dc.b	037,000,111		; GSX_MFS	vsc_form
	dc.b	011,000,001		; V_OPNWK	v_opnwk
	dc.b	001,002,129		; VST_CLP	vs_clip
	dc.b	000,001,012		; VST_HGT	vst_height
	dc.b	001,002,114		; VR_RECT	vr_recfl
	dc.b	001,004,109		; VRO_CPY	vro_cpyfm
	dc.b	003,004,121		; VRT_CPY	vrt_cpyfm
	dc.b	000,001,016		; VSL_WID	vsl_width


restable:
	dc.w	0,2,5,7,3,4,6,8,9
restype:
	dc.w	0,1,1,1,2,3,4,5,6
