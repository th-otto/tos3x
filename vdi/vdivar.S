*******************************  vdivar.s  ***********************************
*
* $Revision: 3.4 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/vdivar.s,v $
* ============================================================================
* $Author: lozben $	$Date: 91/09/10 19:58:36 $     $Locker:  $
* ============================================================================
*
* $Log:	vdivar.s,v $
* Revision 3.4  91/09/10  19:58:36  lozben
* Deleted the _q_circle declaration from the overlay variable space.
* 
* Revision 3.3  91/08/06  20:32:02  lozben
* Added variables _copytran, fringe0, innerLoop, fringe1
* 
* Revision 3.2  91/07/30  17:44:44  lozben
* Made some minor adjustments to work with all the vdi files that use this
* variable space.
* 
* Revision 3.1  91/01/16  12:13:10  lozben
* Deleted all the non overlay variables (linea var struct).
* 
* Revision 3.0  91/01/03  15:23:20  lozben
* New generation VDI
* 
******************************************************************************

		bss
		even

	xdef _dskbuf
_dskbuf: ds.b 7730

#define _fill_buffer _dskbuf

SIZE_FONT_HEAD	equ	90	     * size of font_head structure in FONTDEF.H

*****************************************
*	Overlayable Variables           *
*****************************************

		xdef	_FLIP_Y
		xdef	_Q
		xdef	_Qbottom
		xdef	_Qhole
		xdef	_Qptr
		xdef	_Qtmp
		xdef	_Qtop
		xdef	_charx
		xdef	_chary
		xdef	_collision
		xdef	_deftxbu
		xdef	_direction
		xdef	_done
		xdef	_gotseed
		xdef	_h_align
		xdef	_height
		xdef	_leftoldy
		xdef	_leftdirection
		xdef	_leftseed
		xdef	_leftcollision
		xdef	_newxleft
		xdef	_newxright
		xdef	_oldxleft
		xdef	_oldxright
		xdef	_oldy
		xdef	_rmchar
		xdef	_rmcharx
		xdef	_rmchary
		xdef	_rmword
		xdef	_rmwordx
		xdef	_rmwordy
		xdef	_search_color
		xdef	_seed_type
		xdef	_v_align
		xdef	_width
		xdef	_wordx
		xdef	_wordy
		xdef	_xleft
		xdef	_xright
		xdef	_copytran
		xdef	ptsin_array
		xdef	fringe0
		xdef	innerLoop
		xdef	fringe1


_seed_type	equ	_fill_buffer+00		; indicates the type of fill
_Qbottom	equ	_fill_buffer+04	 	; the bottom of the Q (zero)
_Qtop		equ	_fill_buffer+06	 	; points top seed +3
_Qptr		equ	_fill_buffer+08	 	; points to the active point
_Qtmp		equ	_fill_buffer+10
_Qhole		equ	_fill_buffer+12		; an empty space in the Q
_oldy		equ	_fill_buffer+14		; the previous scan line
_oldxleft	equ	_fill_buffer+16		; left end of line at oldy
_oldxright	equ	_fill_buffer+18		; right end
_newxleft	equ	_fill_buffer+20		; ends of line at oldy +
_newxright	equ	_fill_buffer+22		;     the current direction
_xleft		equ	_fill_buffer+24		; temporary endpoints
_xright		equ	_fill_buffer+26
_direction	equ	_fill_buffer+28		; is next scan line up or down?
_done		equ	_fill_buffer+30		; is the seed queue full?
_gotseed	equ	_fill_buffer+32		; a seed was put in the Q
_leftoldy	equ	_fill_buffer+34 	; like _oldy. (new seedfill)
_leftdirection	equ	_fill_buffer+36 	; like _direction. (new seedfill)
_leftseed	equ	_fill_buffer+38 	; like _gotseed. (new seedfill)
_h_align	equ	_fill_buffer+40
_leftcollision	equ	_fill_buffer+40 	; like _collision. (new seedfill)
_v_align	equ	_fill_buffer+42		; scaler alignments
_width		equ	_fill_buffer+44
_collision	equ	_fill_buffer+42  	; seed was removed from Q (new rtn)
_search_color	equ	_fill_buffer+44		; the color of the border
_Q		equ	_fill_buffer+48	 	; storage for the seed points (1280)
_height		equ	_fill_buffer+46		; extent of string set in dqt_extent
_wordx		equ	_fill_buffer+48
_wordy		equ	_fill_buffer+50		; add this to each space for interword
_rmword		equ	_fill_buffer+52		; the number of pixels left over
_rmwordx	equ	_fill_buffer+54
_rmwordy	equ	_fill_buffer+56		; add this to use up remainder
_charx		equ	_fill_buffer+58
_chary		equ	_fill_buffer+60	 	; inter-character distance
_rmchar		equ	_fill_buffer+62		; number of pixels left over
_rmcharx	equ	_fill_buffer+64
_rmchary	equ	_fill_buffer+66		; add this to use up remainder
_FLIP_Y		equ	_fill_buffer+68		; Non-zero PTSOUT contains magnitudes
_copytran	equ	_fill_buffer+72		;
ptsin_array	equ	_fill_buffer+250	; copy of ptsin[] (4096 bytes)
fringe0		equ	_fill_buffer+1000	; area to compile routines in
innerLoop	equ	_fill_buffer+2000	; area to compile routines in
fringe1		equ	_fill_buffer+3000	; area to compile routines in
_deftxbu	equ	_fill_buffer+4500	; scratch buf for 8x16 (276 bytes)
