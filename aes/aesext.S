/*
 * These functions are referenced by AES,
 * but are defined elsewhere
 *
 *
 * This file is only used when linking a single module,
 * to get the same addresses as when linking the whole ROM
 */
#define __ASSEMBLER__ 1

#include "config.h"
#include "aesdefs.h"
#include "asmdefs.h"
#include "sections.inc"

#if BINEXACT
#  ifdef __GNUC__
#    undef BINEXACT
#    define BINEXACT 0
#  endif
#endif


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
_xbios          equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FB9C /*           de:E35FD8 */
_bios           equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBAC /*           de:E35FE8 */
_gemdos         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBBC /*           de:E35FF8 */
_g_name         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$9CE8 /* us:E300F4 de:E30124 */
_deskmain       equ DESKTBASE+(DESKTLEN-DTLEN_US)+$D6B6 /* us:E33AC2 de:E33AF2 */
_scan_2         equ DESKTBASE+$51C4                     /* us:E2B5D0 de:E2B62A */
_escan_str      equ DESKTBASE+$518C                     /* us:E2B598 de:E2B5F2 */
_save_2         equ DESKTBASE+$5208                     /* us:E2B614 de:E2B66E */
_app_reschange  equ DESKTBASE+$1902                     /* us:E27D0E de:E27D68 */
_XDeselect      equ 0
_getcookie      equ 0
#if (OS_COUNTRY == CTRY_US) | (OS_COUNTRY == CTRY_UK)
_infdata        equ DESKDBASE+$8E6                      /* E3EC06 */
#endif
#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_PL)
_infdata        equ DESKDBASE+$942                      /* E3EC92 */
#endif
#if (OS_COUNTRY == CTRY_ES)
_infdata        equ DESKDBASE+$928
#endif
#if (OS_COUNTRY == CTRY_IT)
_infdata        equ DESKDBASE+$91A
#endif
#if (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_SF)
_infdata        equ DESKDBASE+$95E                      /* E3ECCC */
#endif
#if OS_COUNTRY == CTRY_SV
_infdata        equ DESKDBASE+$8C2                      /* E3EC30 */
#endif
#endif


#if TOSVERSION == 0x206
_xbios          equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FB9C
_bios           equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBAC
_gemdos         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBBC
_g_name         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$9CE8
_deskmain       equ DESKTBASE+(DESKTLEN-DTLEN_US)+$D6B6 /* us:E3005C de:E3008C */
_scan_2         equ DESKTBASE+$51C4
_escan_str      equ DESKTBASE+$518C
_save_2         equ DESKTBASE+$5208
_app_reschange  equ DESKTBASE+$1902
_XDeselect      equ 0
_getcookie      equ 0
#if (OS_COUNTRY == CTRY_US) | (OS_COUNTRY == CTRY_UK)
_infdata        equ DESKDBASE+$8E6                      /* E3699C */
#endif
#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_PL)
_infdata        equ DESKDBASE+$942                      /* E36A28 */
#endif
#if (OS_COUNTRY == CTRY_ES)
_infdata        equ DESKDBASE+$928
#endif
#if (OS_COUNTRY == CTRY_IT)
_infdata        equ DESKDBASE+$91A
#endif
#if (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_SF)
_infdata        equ DESKDBASE+$95E                      /* E3ECCC */
#endif
#if OS_COUNTRY == CTRY_SV
_infdata        equ DESKDBASE+$8C2                      /* E3EC30 */
#endif
#endif


#if TOSVERSION == 0x208
_xbios          equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FB9C
_bios           equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBAC
_gemdos         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$FBBC
_g_name         equ DESKTBASE+(DESKTLEN-DTLEN_US)+$9CE8
_deskmain       equ DESKTBASE+(DESKTLEN-DTLEN_US)+$D6B6
_scan_2         equ DESKTBASE+$51C4
_escan_str      equ DESKTBASE+$518C
_save_2         equ DESKTBASE+$5208
_app_reschange  equ DESKTBASE+$1902
_XDeselect      equ 0
_getcookie      equ 0
#if (OS_COUNTRY == CTRY_US) | (OS_COUNTRY == CTRY_UK)
_infdata        equ DESKDBASE+$8E6
#endif
#if (OS_COUNTRY == CTRY_DE) | (OS_COUNTRY == CTRY_SG) | (OS_COUNTRY == CTRY_PL)
_infdata        equ DESKDBASE+$942
#endif
#if (OS_COUNTRY == CTRY_FR) | (OS_COUNTRY == CTRY_SF)
_infdata        equ DESKDBASE+$95E
#endif
#if OS_COUNTRY == CTRY_SV
_infdata        equ DESKDBASE+$8C2
#endif
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
_tosrsc equ DESKDBASE+DESKDLEN  /* 306us:00E3ECA2 306de:00E3ED2E */
#endif


#if !BINEXACT
/*
 * a reference to this might be generated for structure assignments
 */
	xdef _memcpy
_memcpy equ 0
#endif
