/*
 * resource set indices for en
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        36
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       10
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        25
 * Number of Trees:          2
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          1348
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
#define NUM_STRINGS 36
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 10
#define NUM_OBS 25
#define NUM_TREE 2
#endif



#define MAINTREE                           0 /* free form */
#define TBASE                              2 /* BOX in tree MAINTREE */
#define T1                                 3 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T2                                 4 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T3                                 5 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T4                                 6 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T5                                 7 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T6                                 8 /* TEXT in tree MAINTREE */ /* max len 27 */
#define T7                                 9 /* TEXT in tree MAINTREE */ /* max len 27 */
#define XBASE                             10 /* BOX in tree MAINTREE */
#define XSLIDE                            11 /* BOX in tree MAINTREE */
#define XUP                               12 /* BOXCHAR in tree MAINTREE */
#define XDOWN                             13 /* BOXCHAR in tree MAINTREE */
#define DUMMY2                            14 /* BOX in tree MAINTREE */
#define XADD                              15 /* BUTTON in tree MAINTREE */
#define XDELETE                           16 /* BUTTON in tree MAINTREE */
#define XSAVE                             17 /* BUTTON in tree MAINTREE */
#define EEXIT                             18 /* BUTTON in tree MAINTREE */
#define DUMMY1                            19 /* BOXTEXT in tree MAINTREE */ /* max len 21 */

#define SUBTREE                            1 /* free form */
#define ETITLE                             1 /* BOXTEXT in tree SUBTREE */ /* max len 25 */
#define ETEXT                              2 /* FBOXTEXT in tree SUBTREE */ /* max len 40 */
#define EOK                                3 /* BUTTON in tree SUBTREE */
#define ECANCEL                            4 /* BUTTON in tree SUBTREE */




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
