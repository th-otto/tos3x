/*
 * resource set indices for it
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        46
 * Number of Bitblks:        4
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       10
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        66
 * Number of Trees:          2
 * Number of Userblks:       0
 * Number of Images:         4
 * Total file size:          2830
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "it"
#endif
#undef RSC_ID
#ifdef it
#define RSC_ID it
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 46
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 4
#define NUM_BB 4
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 10
#define NUM_OBS 66
#define NUM_TREE 2
#endif



#define GENERAL                            0 /* free form */
#define B9                                 2 /* BOX in tree GENERAL */
#define LREPEAT                            3 /* BOXCHAR in tree GENERAL */
#define RREPEAT                            4 /* BOXCHAR in tree GENERAL */
#define REPTBASE                           5 /* BOX in tree GENERAL */
#define KREPEAT                            6 /* BOXTEXT in tree GENERAL */ /* max len 4 */
#define B8                                 7 /* BOX in tree GENERAL */
#define B7                                 9 /* BOX in tree GENERAL */
#define RESPBASE                          10 /* BOX in tree GENERAL */
#define KRESPONS                          11 /* BOXTEXT in tree GENERAL */ /* max len 4 */
#define RRESP                             12 /* BOXCHAR in tree GENERAL */
#define LRESP                             13 /* BOXCHAR in tree GENERAL */
#define B6                                14 /* BOX in tree GENERAL */
#define B1                                16 /* BOX in tree GENERAL */
#define BELLICON                          18 /* IMAGE in tree GENERAL */
#define BELL                              19 /* IBOX in tree GENERAL */
#define B2                                20 /* BOX in tree GENERAL */
#define KEYICON                           22 /* IMAGE in tree GENERAL */
#define KEYCLICK                          23 /* IBOX in tree GENERAL */
#define TARGET2                           24 /* BOX in tree GENERAL */
#define B5                                25 /* BOX in tree GENERAL */
#define DOUBLE                            26 /* BOXTEXT in tree GENERAL */ /* max len 13 */
#define CLICKER                           27 /* BOX in tree GENERAL */
#define M0                                28 /* BOXCHAR in tree GENERAL */
#define M1                                29 /* BOXCHAR in tree GENERAL */
#define M2                                30 /* BOXCHAR in tree GENERAL */
#define M3                                31 /* BOXCHAR in tree GENERAL */
#define M4                                32 /* BOXCHAR in tree GENERAL */
#define GCANCEL                           36 /* BUTTON in tree GENERAL */
#define GOK                               37 /* BUTTON in tree GENERAL */
#define B4                                38 /* BOX in tree GENERAL */
#define GSAVE                             39 /* BUTTON in tree GENERAL */
#define B0                                40 /* BOX in tree GENERAL */
#define STATUS                            41 /* BUTTON in tree GENERAL */
#define B3                                42 /* BOX in tree GENERAL */
#define BLK1                              43 /* BOX in tree GENERAL */
#define BLK1TXT                           44 /* BOXTEXT in tree GENERAL */ /* max len 11 */
#define BLK1BUTT                          45 /* BOXTEXT in tree GENERAL */ /* max len 2 */
#define BLK2                              46 /* BOX in tree GENERAL */
#define BLK2TXT                           47 /* BOXTEXT in tree GENERAL */ /* max len 11 */
#define BLK2BUTT                          48 /* BOXTEXT in tree GENERAL */ /* max len 2 */

#define PARTZ                              1 /* free form */
#define VERSION                            4 /* FTEXT in tree PARTZ */ /* max len 4 */
#define DATE1                              5 /* FTEXT in tree PARTZ */ /* max len 8 */
#define STFREE                             8 /* STRING in tree PARTZ */
#define FREETXT                            9 /* STRING in tree PARTZ */
#define TTFREE                            10 /* STRING in tree PARTZ */
#define TOTALTXT                          11 /* STRING in tree PARTZ */
#define TTTOTAL                           12 /* STRING in tree PARTZ */
#define STATOK                            15 /* BUTTON in tree PARTZ */
#define DATE2                             16 /* FTEXT in tree PARTZ */ /* max len 8 */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD it_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD it_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD it_rsc_free(void);
#endif
