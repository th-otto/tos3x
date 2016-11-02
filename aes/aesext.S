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
_infdata        equ DESKDBASE+$942                      /* E3EC92 */
_XDeselect      equ 0
_getcookie      equ 0
_scan_2         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$51EE /* E2B62A */
_escan_str      equ DESKTBASE+(DESKTLEN-DTLEN_US)+$51B6 /* E2B5F2 */
_save_2         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$5232 /* E2B66E */
_app_reschange  equ DESKTBASE+(DESKTLEN-DTLEN_US)+$192C /* E27D68 */
_g_name         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$9CE8 /* E30124 */
_deskmain       equ DESKTBASE+(DESKTLEN-DTLEN_US)+$D6B6 /* E33AF2 */
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
_tosrsc equ DESKDBASE+DESKDLEN  /* $00E3ED2E */
#endif
