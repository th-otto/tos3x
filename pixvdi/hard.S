********************************** hard.s *************************************
*
* $Revision: 3.3 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/mouse.s,v $
* =============================================================================
* $Author: lozben $	$Date: 91/07/30 12:18:47 $     $Locker:  $
* =============================================================================
*
* $Log:	mouse.s,v $
*******************************************************************************

#include "../common/config.h"
#include "vdidefs.h"
#include "lineaequ.inc"

*+
* Link in this file if you choose to omit all the hardware assisted graphics
* routines and also eliminate font16x32. This files fixes the unresolved
* symboles problem.
*-

	xref	sb_cell
	xref	sb_scrup
	xref	sb_scrdn
	xref	sb_blank
	xref	sb_bitblt
	xref	sb_mono
	xref	sb_rect
	xref	sb_vline
	xref	sb_hline
	xref	sb_text
	xref	_f16x32
	xref	_off_8x8
	xref	_dat_8x16

sb_cell:
sb_scrup:
sb_scrdn:
sb_blank:
sb_bitblt:
sb_mono:
sb_rect:
sb_vline:
sb_hline:
sb_text:	rts

_f16x32:

	dc.w	1					; int16_t  font_id
	dc.w	10					; int16_t  point
	dc.b	'8x16 system font                '	; char  name[32]
	dc.w	0					; uint16_t first_ade
	dc.w	255					; uint16_t last_ade
	dc.w	13					; uint16_t top
	dc.w	11					; uint16_t ascent
	dc.w	8					; uint16_t half
	dc.w	2					; uint16_t descent
	dc.w	2					; uint16_t bottom
	dc.w	7					; uint16_t max_char_width
	dc.w	8					; uint16_t max_cell_width
	dc.w	1					; uint16_t left_offset
	dc.w	7					; uint16_t right_offset
	dc.w	1					; uint16_t thicken
	dc.w	1					; uint16_t ul_size
	dc.w	$5555				; uint16_t lighten
	dc.w	$5555				; uint16_t skew
	dc.w	12					; uint16_t flags
	dc.l	0					; uint8_t *hor_table
	dc.l	_off_8x8			; uint16_t *off_table
	dc.l	_dat_8x16			; uint16_t *dat_table
	dc.w	256					; uint16_t form_width
	dc.w	16					; uint16_t form_height
	dc.l	0					; uint16_t *next_font
	dc.w	0					; uint16_t next_seg


