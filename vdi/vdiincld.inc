/*
 ********************************  vdiincld.s  *********************************
 *
 * $Revision: 3.0 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/vdiincld.s,v $
 * =============================================================================
 * $Author: lozben $	$Date: 91/01/03 15:22:56 $     $Locker:  $
 * =============================================================================
 *
 * $Log:	vdiincld.s,v $
 * Revision 3.0  91/01/03  15:22:56  lozben
 * New generation VDI
 * 
 * Revision 2.2  90/03/01  13:32:58  lozben
 * *** Initial Revision ***
 * 
 *******************************************************************************
 */

#include "../common/config.h"

/*
 * This is an include file for other .s files.
 */
#if TOSVERSION >= 0x300
#define P68030	1	/* @check@ set to 1 if we are on a 68030 (else 0) */
#else
#define P68030	0	/* @check@ set to 1 if we are on a 68030 (else 0) */
#endif
#if TOSVERSION >= 0x206
#define PAL4096	1	/* @check@ set to 0 if st mode has a pal of 4096 */
#else
#define PAL4096	0	/* @check@ set to 0 if st mode has a pal of 4096 */
#endif

#define VIDEL_SUPPORT (TOSVERSION >= 0x404)
