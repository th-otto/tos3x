/*
 * resource set indices for en
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        27
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       6
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        25
 * Number of Trees:          1
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          916
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "en"
#endif
#undef RSC_ID
#ifdef en
#define RSC_ID en
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 27
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 6
#define NUM_OBS 25
#define NUM_TREE 1
#endif



#define PRINTER                            0 /* free form */
#define BOX0                               2 /* BOX in tree PRINTER */
#define PTYPE                              3 /* BOXTEXT in tree PRINTER */ /* max len 3 */
#define BOX1                               5 /* BOX in tree PRINTER */
#define COLOR                              6 /* BOXTEXT in tree PRINTER */ /* max len 3 */
#define BOX2                               8 /* BOX in tree PRINTER */
#define PIXLINE                            9 /* BOXTEXT in tree PRINTER */ /* max len 4 */
#define BOX3                              11 /* BOX in tree PRINTER */
#define QUALITY                           12 /* BOXTEXT in tree PRINTER */ /* max len 5 */
#define BOX4                              14 /* BOX in tree PRINTER */
#define PORT                              15 /* BOXTEXT in tree PRINTER */ /* max len 7 */
#define BOX5                              17 /* BOX in tree PRINTER */
#define PAPER                             18 /* BOXTEXT in tree PRINTER */ /* max len 4 */
#define BOX6                              20 /* BOX in tree PRINTER */
#define PSAVE                             21 /* BUTTON in tree PRINTER */
#define BOX7                              22 /* BOX in tree PRINTER */
#define POK                               23 /* BUTTON in tree PRINTER */
#define PCANCEL                           24 /* BUTTON in tree PRINTER */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD en_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD en_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD en_rsc_free(void);
#endif
