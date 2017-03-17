/*
 * resource set indices for doodle12
 *
 * created by ORCS 2.14
 */

/*
 * Number of Strings:        70
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       1
 * Number of Free Strings:   3
 * Number of Free Images:    0
 * Number of Objects:        94
 * Number of Trees:          4
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          3462
 */

#undef RSC_NAME
#define RSC_NAME "doodle12"
#undef RSC_ID
#ifdef doodle12
#define RSC_ID doodle12
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 70
#define NUM_FRSTR 3
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 1
#define NUM_OBS 94
#define NUM_TREE 4
#endif



#define DOODMENU   0 /* menu */
#define DOODDESK   3 /* TITLE in tree DOODMENU */
#define DOODFILE   4 /* TITLE in tree DOODMENU */
#define DOODOPTS   5 /* TITLE in tree DOODMENU */
#define DOODCOLR   6 /* TITLE in tree DOODMENU */
#define DOODINFO   9 /* STRING in tree DOODMENU */
#define DOODLOAD  18 /* STRING in tree DOODMENU */
#define DOODSAVE  19 /* STRING in tree DOODMENU */
#define DOODQUIT  20 /* STRING in tree DOODMENU */
#define DOODSVAS  21 /* STRING in tree DOODMENU */
#define DOODABAN  22 /* STRING in tree DOODMENU */
#define DOODERAP  24 /* STRING in tree DOODMENU */
#define DOODPENS  25 /* STRING in tree DOODMENU */
#define COLOR0    28 /* STRING in tree DOODMENU */
#define COLOR1    29 /* STRING in tree DOODMENU */
#define COLOR2    30 /* STRING in tree DOODMENU */
#define COLOR3    31 /* STRING in tree DOODMENU */
#define COLOR4    32 /* STRING in tree DOODMENU */
#define COLOR5    33 /* STRING in tree DOODMENU */
#define COLOR6    34 /* STRING in tree DOODMENU */
#define COLOR7    35 /* STRING in tree DOODMENU */
#define COLOR8    36 /* STRING in tree DOODMENU */
#define COLOR9    37 /* STRING in tree DOODMENU */
#define COLOR10   38 /* STRING in tree DOODMENU */
#define COLOR11   39 /* STRING in tree DOODMENU */
#define COLOR12   40 /* STRING in tree DOODMENU */
#define COLOR13   41 /* STRING in tree DOODMENU */
#define COLOR14   42 /* STRING in tree DOODMENU */
#define COLOR15   43 /* STRING in tree DOODMENU */

#define DOODINFD   1 /* form/dialog */
#define DOODOK     3 /* BUTTON in tree DOODINFD */

#define DOODSVAD   2 /* form/dialog */
#define DOODSOK    2 /* BUTTON in tree DOODSVAD */
#define DOODSCNL   3 /* BUTTON in tree DOODSVAD */
#define DOODNAME   4 /* FTEXT in tree DOODSVAD */ /* max len 0 */

#define DOODPEND   3 /* form/dialog */
#define DOODFINE   5 /* BUTTON in tree DOODPEND */
#define DOODMED    6 /* BUTTON in tree DOODPEND */
#define DOODBRD    7 /* BUTTON in tree DOODPEND */
#define DOODPEN    9 /* BUTTON in tree DOODPEND */
#define DOODERSR  10 /* BUTTON in tree DOODPEND */
#define DOODFILL  11 /* BUTTON in tree DOODPEND */
#define DOODPSOK  12 /* BUTTON in tree DOODPEND */
#define DOODCNCL  13 /* BUTTON in tree DOODPEND */

#define DOODOVWR   0 /* Alert string */
/* [2][You are about|to write over|an existing file.][Ok| Cancel ] */

#define DOODNWDW   1 /* Alert string */
/* [3][ GEM does not have| any windows left,| Dr Doodle aborting][ Sorry ] */

#define ERASEWRN   2 /* Alert string */
/* [2][You are about|to erase your|picture.][ OK |CANCEL] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD doodle12_rsc_load(void);
extern _WORD doodle12_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD doodle12_rsc_free(void);
#endif
