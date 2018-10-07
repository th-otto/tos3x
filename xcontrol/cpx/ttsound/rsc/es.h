/*
 * resource set indices for es
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        22
 * Number of Bitblks:        8
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       5
 * Number of Free Strings:   0
 * Number of Free Images:    7
 * Number of Objects:        83
 * Number of Trees:          1
 * Number of Userblks:       0
 * Number of Images:         8
 * Total file size:          5912
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "es"
#endif
#undef RSC_ID
#ifdef es
#define RSC_ID es
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 22
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 8
#define NUM_BB 8
#define NUM_FRIMG 7
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 5
#define NUM_OBS 83
#define NUM_TREE 1
#endif



#define SOUND                              0 /* free form */
#define B1                                 4 /* BOX in tree SOUND */
#define VR                                 5 /* BOXCHAR in tree SOUND */
#define VL                                 6 /* BOXCHAR in tree SOUND */
#define VBASE                              7 /* BOX in tree SOUND */
#define VSLIDER                            8 /* BOXTEXT in tree SOUND */ /* max len 2 */
#define B0                                 9 /* BOX in tree SOUND */
#define VSBASE                            10 /* BOX in tree SOUND */
#define VS38                              11 /* BOX in tree SOUND */
#define VS37                              12 /* BOX in tree SOUND */
#define VS36                              13 /* BOX in tree SOUND */
#define VS35                              14 /* BOX in tree SOUND */
#define VS34                              15 /* BOX in tree SOUND */
#define VS33                              16 /* BOX in tree SOUND */
#define VS32                              17 /* BOX in tree SOUND */
#define VS31                              18 /* BOX in tree SOUND */
#define VS30                              19 /* BOX in tree SOUND */
#define VS29                              20 /* BOX in tree SOUND */
#define VS28                              21 /* BOX in tree SOUND */
#define VS27                              22 /* BOX in tree SOUND */
#define VS26                              23 /* BOX in tree SOUND */
#define VS25                              24 /* BOX in tree SOUND */
#define VS24                              25 /* BOX in tree SOUND */
#define VS23                              26 /* BOX in tree SOUND */
#define VS22                              27 /* BOX in tree SOUND */
#define VS21                              28 /* BOX in tree SOUND */
#define VS20                              29 /* BOX in tree SOUND */
#define VS19                              30 /* BOX in tree SOUND */
#define VS18                              31 /* BOX in tree SOUND */
#define VS17                              32 /* BOX in tree SOUND */
#define VS16                              33 /* BOX in tree SOUND */
#define VS15                              34 /* BOX in tree SOUND */
#define VS14                              35 /* BOX in tree SOUND */
#define VS13                              36 /* BOX in tree SOUND */
#define VS12                              37 /* BOX in tree SOUND */
#define VS11                              38 /* BOX in tree SOUND */
#define VS10                              39 /* BOX in tree SOUND */
#define VS9                               40 /* BOX in tree SOUND */
#define VS8                               41 /* BOX in tree SOUND */
#define VS7                               42 /* BOX in tree SOUND */
#define VS6                               43 /* BOX in tree SOUND */
#define VS5                               44 /* BOX in tree SOUND */
#define VS4                               45 /* BOX in tree SOUND */
#define VS3                               46 /* BOX in tree SOUND */
#define VS2                               47 /* BOX in tree SOUND */
#define VS1                               48 /* BOX in tree SOUND */
#define VS0                               49 /* BOX in tree SOUND */
#define HDMASK                            50 /* BOX in tree SOUND */
#define HDBASE                            51 /* IMAGE in tree SOUND */
#define B4                                52 /* BOX in tree SOUND */
#define BBASE                             53 /* BOX in tree SOUND */
#define BSLIDER                           54 /* BOX in tree SOUND */
#define BL                                55 /* BOXCHAR in tree SOUND */
#define BALNUM1                           57 /* BOXTEXT in tree SOUND */ /* max len 2 */
#define BALNUM2                           58 /* BOXTEXT in tree SOUND */ /* max len 2 */
#define BR                                60 /* BOXCHAR in tree SOUND */
#define B5                                61 /* BOX in tree SOUND */
#define B6                                63 /* BOX in tree SOUND */
#define BASSBASE                          64 /* BOX in tree SOUND */
#define BASSSLID                          65 /* BOXTEXT in tree SOUND */ /* max len 2 */
#define RBASS                             66 /* BOXCHAR in tree SOUND */
#define LBASS                             67 /* BOXCHAR in tree SOUND */
#define B7                                68 /* BOX in tree SOUND */
#define B8                                70 /* BOX in tree SOUND */
#define TREBBASE                          71 /* BOX in tree SOUND */
#define TREBSLID                          72 /* BOXTEXT in tree SOUND */ /* max len 2 */
#define LTREB                             73 /* BOXCHAR in tree SOUND */
#define RTREB                             74 /* BOXCHAR in tree SOUND */
#define B9                                75 /* BOX in tree SOUND */
#define B10                               77 /* BOX in tree SOUND */
#define B2                                78 /* BOX in tree SOUND */
#define RSAVE                             79 /* BUTTON in tree SOUND */
#define B3                                80 /* BOX in tree SOUND */
#define BOK                               81 /* BUTTON in tree SOUND */
#define CANCEL                            82 /* BUTTON in tree SOUND */

#define HD1                                0 /* Free image */

#define HD2                                1 /* Free image */

#define HD3                                2 /* Free image */

#define HD4                                3 /* Free image */

#define HD5                                4 /* Free image */

#define HD6                                5 /* Free image */

#define HD7                                6 /* Free image */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD es_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD es_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD es_rsc_free(void);
#endif
