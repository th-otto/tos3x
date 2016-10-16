/*
 ********************************  vdidefs.h  *********************************
 *
 * =============================================================================
 * $Author: lozben $	$Date: 91/01/03 15:22:56 $     $Locker:  $
 * =============================================================================
 *
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

/*
 * set to 1 if we are on a 68010+ with 8-byte trap exception frames
 */
#define P68010  (TOSVERSION >= 0x300)

/*
 * set to 1 if we are on a 68030+ (else 0)
 * Used when invalidating caches might be neccessary
 */
#define P68030  (TOSVERSION >= 0x300)

#if TOSVERSION >= 0x206
#define	MAX_PAL		4096	/* palette size                     */
#else
#define MAX_PAL		512	    /* palette size                     */
#endif

/*
 * set to 1 for supporting pixel-packed VIDEL modes, including hicolor and truecolor
 */
#define VIDEL_SUPPORT (TOSVERSION >= 0x400)

#define BLITTER_SUPPORT ((TOSVERSION < 0x300) | (TOSVERSION >= 0x400))

/*
 * used to mark some strange looking code segments
 * that are needed to create a binary equivalent image
 * of the TOS ROMs
 */
#define BINEXACT 1

/*
 * used to mark code changes due to
 * multiple linea variable structures feature
 */
#define MULTI_LINEA (TOSVERSION >= 0x400)

/*
 * see lineavar.h for explanation
 */
#define LINEA_HACK (BINEXACT & (!MULTI_LINEA))
