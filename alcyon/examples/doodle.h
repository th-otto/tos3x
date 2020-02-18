/*
 * resource set indices for doodle
 *
 * created by ORCS 2.17
 */

/*
 * Number of Strings:        62
 * Number of Bitblks:        10
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       1
 * Number of Free Strings:   3
 * Number of Free Images:    3
 * Number of Objects:        102
 * Number of Trees:          4
 * Number of Userblks:       0
 * Number of Images:         10
 * Total file size:          4326
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "doodle"
#endif
#undef RSC_ID
#ifdef doodle
#define RSC_ID doodle
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 62
#define NUM_FRSTR 3
#define NUM_UD 0
#define NUM_IMAGES 10
#define NUM_BB 10
#define NUM_FRIMG 3
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 1
#define NUM_OBS 102
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
#define DOODSVAS  20 /* STRING in tree DOODMENU */
#define DOODABAN  21 /* STRING in tree DOODMENU */
#define DOODQUIT  23 /* STRING in tree DOODMENU */
#define DOODERAP  25 /* STRING in tree DOODMENU */
#define DOODPENS  27 /* STRING in tree DOODMENU */
#define COLOR0    29 /* STRING in tree DOODMENU */
#define COLORB0   30 /* BOX in tree DOODMENU */
#define COLOR1    31 /* STRING in tree DOODMENU */
#define COLORB1   32 /* BOX in tree DOODMENU */
#define COLOR2    33 /* STRING in tree DOODMENU */
#define COLORB2   34 /* BOX in tree DOODMENU */
#define COLOR3    35 /* STRING in tree DOODMENU */
#define COLORB3   36 /* BOX in tree DOODMENU */
#define COLOR4    37 /* STRING in tree DOODMENU */
#define COLORB4   38 /* BOX in tree DOODMENU */
#define COLOR5    39 /* STRING in tree DOODMENU */
#define COLORB5   40 /* BOX in tree DOODMENU */
#define COLOR6    41 /* STRING in tree DOODMENU */
#define COLORB6   42 /* BOX in tree DOODMENU */
#define COLOR7    43 /* STRING in tree DOODMENU */
#define COLORB7   44 /* BOX in tree DOODMENU */
#define COLOR8    45 /* STRING in tree DOODMENU */
#define COLORB8   46 /* BOX in tree DOODMENU */
#define COLOR9    47 /* STRING in tree DOODMENU */
#define COLORB9   48 /* BOX in tree DOODMENU */
#define COLOR10   49 /* STRING in tree DOODMENU */
#define COLORB10  50 /* BOX in tree DOODMENU */
#define COLOR11   51 /* STRING in tree DOODMENU */
#define COLORB11  52 /* BOX in tree DOODMENU */
#define COLOR12   53 /* STRING in tree DOODMENU */
#define COLORB12  54 /* BOX in tree DOODMENU */
#define COLOR13   55 /* STRING in tree DOODMENU */
#define COLORB13  56 /* BOX in tree DOODMENU */
#define COLOR14   57 /* STRING in tree DOODMENU */
#define COLORB14  58 /* BOX in tree DOODMENU */
#define COLOR15   59 /* STRING in tree DOODMENU */
#define COLORB15  60 /* BOX in tree DOODMENU */

#define DOODINFD   1 /* form/dialog */
#define DOODOK     6 /* BUTTON in tree DOODINFD */
#define DOODIMG    9 /* IMAGE in tree DOODINFD */

#define DOODSVAD   2 /* form/dialog */
#define DOODSOK    2 /* BUTTON in tree DOODSVAD */
#define DOODSCNL   3 /* BUTTON in tree DOODSVAD */
#define DOODNAME   4 /* FTEXT in tree DOODSVAD */ /* max len 8 */

#define DOODPEND   3 /* form/dialog */
#define DOODPFIN   3 /* IMAGE in tree DOODPEND */
#define DOODPMED   4 /* IMAGE in tree DOODPEND */
#define DOODPBRD   5 /* IMAGE in tree DOODPEND */
#define DOODEFIN   6 /* IMAGE in tree DOODPEND */
#define DOODEMED   7 /* IMAGE in tree DOODPEND */
#define DOODEBRD   8 /* IMAGE in tree DOODPEND */
#define DOODPSOK  10 /* BUTTON in tree DOODPEND */
#define DOODCNCL  12 /* BUTTON in tree DOODPEND */
#define DOODPCLR  18 /* IBOX in tree DOODPEND */
#define DOODPSEL  23 /* BOXCHAR in tree DOODPEND */

#define DOODOVWR   0 /* Alert string */
/* [2][You are about|to write over|an existing file.][Ok| Cancel ] */

#define DOODNWDW   1 /* Alert string */
/* [3][ GEM does not have| any windows left,| Dr Doodle aborting][ Sorry ] */

#define ERASEWRN   2 /* Alert string */
/* [2][You are about|to erase your|picture.][ OK |CANCEL] */

#define ERASEBRD   0 /* Mouse cursor */

#define ERASEMED   1 /* Mouse cursor */

#define ERASEFIN   2 /* Mouse cursor */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD doodle_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD doodle_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD doodle_rsc_free(void);
#endif
