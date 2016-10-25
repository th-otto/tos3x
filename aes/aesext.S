/*
 * These functions are referenced by AES,
 * but are defined elsewhere
 *
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */
#define __ASSEMBLER__ 1

#include "../common/config.h"
#include "aesdefs.h"
#include "../common/asmdefs.h"
#include "../common/sections.inc"

/* from DESKTOP */
	xdef _infdata
	xdef _XDeselect
	xdef _getcookie
	xdef _scan_2
	xdef _escan_str
	xdef _save_2
	xdef _app_reschange
	xdef _g_name
	xdef _deskmain
	xdef _xbios
	xdef _bios
	xdef _gemdos

#if TOSVERSION == 0x306
_xbios          equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FB9C /* E35FD8 */
_bios           equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBAC /* E35FE8 */
_gemdos         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBBC /* E35FF8 */
_infdata        equ 0
_XDeselect      equ 0
_getcookie      equ 0
_scan_2         equ 0
_escan_str      equ 0
_save_2         equ 0
_app_reschange  equ 0
_g_name         equ 0
_deskmain       equ 0
#endif

#if MULTILANG_SUPPORT
	xdef _RSCUS
	xdef _RSCDE
	xdef _RSCFR
	xdef _RSCUK
	xdef _RSCES
	xdef _RSCIT
	xdef _RSCSV
	xdef _RSCSF
	xdef _RSCSG
	xdef _RSCTR
	xdef _RSCFI
	xdef _RSCNO
_RSCUS equ 0
_RSCDE equ 0
_RSCFR equ 0
_RSCUK equ 0
_RSCES equ 0
_RSCIT equ 0
_RSCSV equ 0
_RSCSF equ 0
_RSCSG equ 0
_RSCTR equ 0
_RSCFI equ 0
_RSCNO equ 0
#else
	xdef _tosrsc
_tosrsc equ 0
#endif
