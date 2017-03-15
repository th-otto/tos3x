/*
 * resource set indices for test
 *
 * created by ORCS 2.14
 */

/*
 * Number of Strings:        193
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       8
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        199
 * Number of Trees:          6
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          7440
 */

#undef RSC_NAME
#define RSC_NAME "test"
#undef RSC_ID
#ifdef test
#define RSC_ID test
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 193
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 8
#define NUM_OBS 199
#define NUM_TREE 6
#endif



#define MENU1      0 /* form/dialog */
#define DELETE     5 /* STRING in tree MENU1 */
#define FLOPPY    14 /* STRING in tree MENU1 */

#define BIGBOX     1 /* form/dialog */
#define T1         2 /* TEXT in tree BIGBOX */ /* max len 9 */
#define TITLE1     3 /* BOXTEXT in tree BIGBOX */ /* max len 5 */
#define T2         4 /* TEXT in tree BIGBOX */ /* max len 9 */
#define TITLE2     5 /* BOXTEXT in tree BIGBOX */ /* max len 5 */
#define T3         6 /* TEXT in tree BIGBOX */ /* max len 9 */
#define TITLE3     7 /* BOXTEXT in tree BIGBOX */ /* max len 5 */
#define T4         8 /* TEXT in tree BIGBOX */ /* max len 9 */
#define TITLE4     9 /* BOXTEXT in tree BIGBOX */ /* max len 5 */
#define XEXIT     10 /* BUTTON in tree BIGBOX */

#define COUNTRY    2 /* form/dialog */
#define ARKANSAS   4 /* STRING in tree COUNTRY */
#define IDAHO     12 /* STRING in tree COUNTRY */
#define IOWA      15 /* STRING in tree COUNTRY */
#define MAINE     19 /* STRING in tree COUNTRY */
#define MICHIGAN  22 /* STRING in tree COUNTRY */
#define NEBRASKA  26 /* STRING in tree COUNTRY */

#define OTHER      3 /* form/dialog */
#define MODEM      1 /* BOX in tree OTHER */
#define MODEMA     2 /* STRING in tree OTHER */
#define QUOTES    18 /* BOX in tree OTHER */
#define QUOTE1    19 /* STRING in tree OTHER */
#define DISNEY    21 /* BOX in tree OTHER */
#define MICKEY    22 /* STRING in tree OTHER */

#define MENUBAR    4 /* menu */
#define GTITLE     5 /* TITLE in tree MENUBAR */
#define ETITLE     6 /* TITLE in tree MENUBAR */
#define CTITLE     7 /* TITLE in tree MENUBAR */
#define MITEM     10 /* STRING in tree MENUBAR */
#define FNEW      19 /* STRING in tree MENUBAR */
#define FQUIT     28 /* STRING in tree MENUBAR */
#define PASTE     39 /* STRING in tree MENUBAR */

#define TREE6      5 /* form/dialog */
#define MONTH      1 /* BOX in tree TREE6 */
#define JANUARY    2 /* STRING in tree TREE6 */
#define PLANETS   14 /* BOX in tree TREE6 */
#define SUN       15 /* STRING in tree TREE6 */
#define MUSIC     25 /* BOX in tree TREE6 */
#define YAZ       26 /* STRING in tree TREE6 */
#define WEEK      35 /* BOX in tree TREE6 */
#define MONDAY    36 /* STRING in tree TREE6 */
#define NUMBERS   43 /* BOX in tree TREE6 */
#define XONE      44 /* STRING in tree TREE6 */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD test_rsc_load(void);
extern _WORD test_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD test_rsc_free(void);
#endif
