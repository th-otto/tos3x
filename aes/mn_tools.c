/* MN_TOOLS.C
 * ======================================================================
 * DESCRIPTION: TOOLS for the menu.c file.
 * 		prototypes and defines are in tools.h
 *
 * 11/22/91 cjg	created from aesapi and aesalt
 * 01/30/92 cjg - Converted to Alcyon ( AES Version )
 *              - use internal AES calls
 * 02/19/92 cjg - merged into AES
 * 02/25/92 cjg - removed a heck of alot of code...
 * 03/24/92 cjg - removed WaitForUpButton()
 * 05/13/92 cjg - used binding calls
 * 05/15/92 cjg - Don't use binding calls - use internal AES calls
 */


/* INCLUDES
 * ======================================================================
 */
#include "aes.h"

#if SUBMENUS /* whole file */

#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"
#include "mn_tools.h"


/* FUNCTIONS
 * ======================================================================
 */



/* ObjcDraw()
 * ======================================================================
 */
VOID ObjcDraw(P(OBJECT *) tree, P(int) obj, P(GRECT *) rect)
PP(OBJECT *tree;)
PP(int obj;)
PP(GRECT *rect;)
{
	gsx_sclip(rect);
	ob_draw((LPTREE)tree, obj, MAX_DEPTH);
}



/* rc_2xy()
 * ================================================================
 */
VOID rc_2xy(P(GRECT *) r, P(int16_t *) pxy)
PP(GRECT *r;)
PP(int16_t *pxy;)
{
	*(GRECT *) pxy = *r;
	pxy[2] += r->g_x - 1;
	pxy[3] += r->g_y - 1;
}

#endif /* SUBMENUS */
