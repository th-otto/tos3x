/*
 * These functions are referenced by DESKTOP,
 * but are defined elsewhere
 *
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */
#define __ASSEMBLER__ 1

#include "config.h"
#include "deskdefs.h"
#include "asmdefs.h"
#include "sections.inc"


/* from AES */

	xdef _strcpy
	xdef _strcat
	xdef _xstrpcpy
	xdef _xstrpcat
	xdef _scasb
	xdef _strscn
	xdef _strlen
	xdef _streq
	xdef _fmt_str
	xdef _unfmt_str
	xdef _inf_sset
	xdef _fs_sget
	xdef _inf_gindex
	xdef _merge_str
	xdef _wildcmp
	xdef _bfill
	xdef _min
	xdef _max
	xdef _rc_copy
	xdef _rc_equal
	xdef _rc_intersect
	xdef _rc_union
	xdef _rc_constrain
	xdef _inside
	xdef _LBCOPY
	xdef _strchk
	xdef _cart_init
	xdef _cart_sfirst
	xdef _c_sfirst
	xdef _cart_snext
	xdef _toupper
	xdef _isdrive
	xdef _rom_ram
	xdef _trap
	xdef _trp14
	xdef _trp13
	xdef _deskerr
	xdef _desknoerr
	xdef _mediach
	
	xdef _dsptch
	xdef _wm_new
	xdef _wm_close
	xdef _wm_delete
	xdef _wm_find
	xdef _wm_update
	xdef _wm_create
	xdef _wm_open
	xdef _wm_get
	xdef _wm_set
	xdef _wm_calc
	xdef _ap_init
	xdef _ap_exit
	xdef _ap_rdwr
	xdef _fs_input
	xdef _rs_sglobe
	xdef _rs_load
	xdef _rs_free
	xdef _rs_obfix
	xdef _rs_gaddr
	xdef _mn_popup
	xdef _mn_istart
	xdef _mn_bar
	xdef _do_chg
	xdef _ob_gclip
	xdef _ob_offset
	xdef _ob_order
	xdef _ob_find
	xdef _ob_add
	xdef _ob_draw
	xdef _ob_change
	xdef _ob_center
	xdef _gr_mouse
	xdef _gr_mkstate
	xdef _gr_growbox
	xdef _gr_shrinkbox
	xdef _gr_rubbox
	xdef _ev_multi
	xdef _ev_button
	xdef _ev_dclick
	xdef _fm_do
	xdef _fm_dial
	xdef _fm_alert
	xdef _fm_error
	xdef _sh_get
	xdef _sh_put
	xdef _sh_write
	
	xdef _gsx_sclip
	xdef _gsx_attr
	xdef _gsx_xline
	xdef _vro_cpyfm
	xdef _gsx_ncode
	
	xdef lmul
	xdef ldiv
	
_strcpy         equ 0
_strcat         equ 0
_xstrpcpy       equ 0
_xstrpcat       equ 0
_scasb          equ 0
_strscn         equ 0
_strlen         equ 0
_streq          equ 0
_fmt_str        equ 0
_unfmt_str      equ 0
_inf_sset       equ 0
_fs_sget        equ 0
_inf_gindex     equ 0
_merge_str      equ 0
_wildcmp        equ 0
_bfill          equ 0
_min            equ 0
_max            equ 0
_rc_copy        equ 0
_rc_equal       equ 0
_rc_intersect   equ 0
_rc_union       equ 0
_rc_constrain   equ 0
_inside         equ 0
_LBCOPY         equ 0
_strchk         equ 0
_cart_init      equ 0
_cart_sfirst    equ 0
_c_sfirst       equ 0
_cart_snext     equ 0
_toupper        equ 0
_isdrive        equ 0
_rom_ram        equ 0
_trap           equ 0
_trp14          equ 0
_trp13          equ 0
_deskerr        equ 0
_desknoerr      equ 0
_mediach        equ 0
	
_dsptch         equ 0
_wm_new         equ 0
_wm_close       equ 0
_wm_delete      equ 0
_wm_find        equ 0
_wm_update      equ 0
_wm_create      equ 0
_wm_open        equ 0
_wm_get         equ 0
_wm_set         equ 0
_wm_calc        equ 0
_ap_init        equ 0
_ap_exit        equ 0
_ap_rdwr        equ 0
_fs_input       equ 0
_rs_sglobe      equ 0
_rs_load        equ 0
_rs_free        equ 0
_rs_obfix       equ 0
_rs_gaddr       equ 0
_mn_popup       equ 0
_mn_istart      equ 0
_mn_bar         equ 0
_do_chg         equ 0
_ob_gclip       equ 0
_ob_offset      equ 0
_ob_order       equ 0
_ob_find        equ 0
_ob_add         equ 0
_ob_draw        equ 0
_ob_change      equ 0
_ob_center      equ 0
_gr_mouse       equ 0
_gr_mkstate     equ 0
_gr_growbox     equ 0
_gr_shrinkbox   equ 0
_gr_rubbox      equ 0
_ev_multi       equ 0
_ev_button      equ 0
_ev_dclick      equ 0
_fm_do          equ 0
_fm_dial        equ 0
_fm_alert       equ 0
_fm_error       equ 0
_sh_get         equ 0
_sh_put         equ 0
_sh_write       equ 0

_gsx_sclip      equ 0
_gsx_attr       equ 0
_gsx_xline      equ 0
_vro_cpyfm      equ 0
_gsx_ncode      equ 0

lmul            equ 0
ldiv            equ 0
