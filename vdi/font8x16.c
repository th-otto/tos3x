/*
 ********************************* fnt8x16 **************************************
 *
 * ==============================================================================
 * $Author: lozben $	$Date: 91/01/03 15:09:02 $
 * ==============================================================================
 *
 * Revision 3.0  91/01/03  15:09:02  lozben
 * New generation VDI
 * 
 * Revision 2.2  89/07/11  16:00:25  lozben
 * Changed the header to point to ram16x32 header as the next font.
 * 
 * Revision 2.1  89/02/21  17:20:29  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 ********************************************************************************
 */

#include "vdi.h"
#include "fontdef.h"
#include "gsxextrn.h"


extern int16_t const off_8x8[];

/* 206de: 00e33f62 */
/* 306de: 00e37baa */
/* 306us: 00e37b7a */
int16_t const dat_8x16[2048] =
{
#if TP_35_16 == 0
#include "../tospatch/fonts/8x16/system.c"
#endif
#if TP_35_16 == 1
#include "../tospatch/fonts/8x16/tospatch.c"
#endif
#if TP_35_16 == 2
#include "../tospatch/fonts/8x16/polish.c"
#endif
#if TP_35_16 == 3
#include "../tospatch/fonts/8x16/czech.c"
#endif
#if TP_35_16 == 4
#include "../tospatch/fonts/8x16/monaco.c"
#endif
#if TP_35_16 == 5
#include "../tospatch/fonts/8x16/blue.c"
#endif
#if TP_35_16 == 6
#include "../tospatch/fonts/8x16/classic.c"
#endif
#if TP_35_16 == 7
#include "../tospatch/fonts/8x16/classic1.c"
#endif
#if TP_35_16 == 8
#include "../tospatch/fonts/8x16/classic2.c"
#endif
#if TP_35_16 == 9
#include "../tospatch/fonts/8x16/computer.c"
#endif
#if TP_35_16 == 10
#include "../tospatch/fonts/8x16/cursive.c"
#endif
#if TP_35_16 == 11
#include "../tospatch/fonts/8x16/desktop.c"
#endif
#if TP_35_16 == 12
#include "../tospatch/fonts/8x16/deutsch.c"
#endif
#if TP_35_16 == 13
#include "../tospatch/fonts/8x16/eight.c"
#endif
#if TP_35_16 == 14
#include "../tospatch/fonts/8x16/griech.c"
#endif
#if TP_35_16 == 15
#include "../tospatch/fonts/8x16/ibm_g2.c"
#endif
#if TP_35_16 == 16
#include "../tospatch/fonts/8x16/kritzel.c"
#endif
#if TP_35_16 == 17
#include "../tospatch/fonts/8x16/mac_font.c"
#endif
#if TP_35_16 == 18
#include "../tospatch/fonts/8x16/mcappeal.c"
#endif
#if TP_35_16 == 19
#include "../tospatch/fonts/8x16/old.c"
#endif
#if TP_35_16 == 20
#include "../tospatch/fonts/8x16/rally.c"
#endif
#if TP_35_16 == 21
#include "../tospatch/fonts/8x16/skyvox.c"
#endif
#if TP_35_16 == 22
#include "../tospatch/fonts/8x16/stencil.c"
#endif
#if TP_35_16 == 23
#include "../tospatch/fonts/8x16/sv.c"
#endif
#if TP_35_16 == 24
#include "../tospatch/fonts/8x16/tempus.c"
#endif
#if TP_35_16 == 25
#include "../tospatch/fonts/8x16/thin.c"
#endif
#if TP_35_16 == 26
#include "../tospatch/fonts/8x16/thin1.c"
#endif
#if TP_35_16 == 27
#include "../tospatch/fonts/8x16/typewrit.c"
#endif
};



struct font_head const f8x16 ={

    1,					/*   int16_t  font_id	 	*/
    10,					/*   int16_t  point	 	*/
   "8x16 system font",	/*   char  name[32]	 	*/
    0,					/*   uint16_t first_ade	 	*/
    255,				/*   uint16_t last_ade	 	*/
    13,					/*   uint16_t top		 	*/
    11,					/*   uint16_t ascent	 	*/
    8,					/*   uint16_t half		 	*/
    2,					/*   uint16_t descent	 	*/
    2,					/*   uint16_t bottom	 	*/
    7,					/*   uint16_t max_char_width	*/
    8,					/*   uint16_t max_cell_width	*/
    1,					/*   uint16_t left_offset	 	*/
    7,					/*   uint16_t right_offset	 	*/
    1,					/*   uint16_t thicken	 	*/
    1,					/*   uint16_t ul_size	 	*/
    0x5555,				/*   uint16_t lighten	 	*/
    0x5555,				/*   uint16_t skew		 	*/
    STDFORM | MONOSPACE,	/*   uint16_t flags	 	*/
    0,					/*   uint8_t *hor_table	 	*/
    (const uint16_t *)off_8x8,			/*   uint16_t *off_table	 	*/
    (const uint16_t *)dat_8x16,			/*   uint16_t *dat_table	 	*/
    256,				/*   uint16_t form_width	 	*/
    16,					/*   uint16_t form_height	 	*/
#if PLANES8
    &ram16x32,			/*   uint16_t *next_font	 	*/
#else
	0,
#endif
    0					/*   uint16_t next_seg		*/
};
