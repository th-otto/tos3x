/*
 * resource set indices for en
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        12
 * Number of Bitblks:        0
 * Number of Iconblks:       8
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       1
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        29
 * Number of Trees:          1
 * Number of Userblks:       0
 * Number of Images:         16
 * Total file size:          4960
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
#define NUM_STRINGS 12
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 16
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 8
#define NUM_CIB 0
#define NUM_TI 1
#define NUM_OBS 29
#define NUM_TREE 1
#endif



#define MAINTREE                           0 /* free form */
#define BASE                               3 /* BOX in tree MAINTREE */
#define BOX1                               4 /* BOX in tree MAINTREE */
#define ICON1                              5 /* ICON in tree MAINTREE */ /* max len 6 */
#define BOX2                               6 /* BOX in tree MAINTREE */
#define ICON2                              7 /* ICON in tree MAINTREE */ /* max len 4 */
#define BOX3                               8 /* BOX in tree MAINTREE */
#define ICON3                              9 /* ICON in tree MAINTREE */ /* max len 7 */
#define BOX4                              10 /* BOX in tree MAINTREE */
#define ICON4                             11 /* ICON in tree MAINTREE */ /* max len 7 */
#define BOX5                              12 /* BOX in tree MAINTREE */
#define ICON5                             13 /* ICON in tree MAINTREE */ /* max len 6 */
#define BOX6                              14 /* BOX in tree MAINTREE */
#define ICON6                             15 /* ICON in tree MAINTREE */ /* max len 7 */
#define BOX7                              16 /* BOX in tree MAINTREE */
#define ICON7                             17 /* ICON in tree MAINTREE */ /* max len 5 */
#define BOX8                              18 /* BOX in tree MAINTREE */
#define ICON8                             19 /* ICON in tree MAINTREE */ /* max len 5 */
#define COVER1                            20 /* IBOX in tree MAINTREE */
#define COVER2                            21 /* IBOX in tree MAINTREE */
#define COVER3                            22 /* IBOX in tree MAINTREE */
#define COVER4                            23 /* IBOX in tree MAINTREE */
#define COVER5                            24 /* IBOX in tree MAINTREE */
#define COVER6                            25 /* IBOX in tree MAINTREE */
#define COVER7                            26 /* IBOX in tree MAINTREE */
#define COVER8                            27 /* IBOX in tree MAINTREE */
#define SEXIT                             28 /* BUTTON in tree MAINTREE */




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
