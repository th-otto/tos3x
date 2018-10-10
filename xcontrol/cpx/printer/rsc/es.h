/*
 * resource set indices for es
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
 * Total file size:          928
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
#define PTYPE                              3 /* BOXTEXT in tree PRINTER */ /* max len 3 */
#define COLOR                              6 /* BOXTEXT in tree PRINTER */ /* max len 3 */
#define PIXLINE                            9 /* BOXTEXT in tree PRINTER */ /* max len 4 */
#define QUALITY                           12 /* BOXTEXT in tree PRINTER */ /* max len 5 */
#define PSAVE                             15 /* BUTTON in tree PRINTER */
#define PORT                              17 /* BOXTEXT in tree PRINTER */ /* max len 7 */
#define PAPER                             20 /* BOXTEXT in tree PRINTER */ /* max len 4 */
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
extern _WORD es_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD es_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD es_rsc_free(void);
#endif
