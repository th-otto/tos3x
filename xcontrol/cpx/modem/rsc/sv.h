/*
 * resource set indices for sv
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        29
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       6
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        27
 * Number of Trees:          1
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          976
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "sv"
#endif
#undef RSC_ID
#ifdef sv
#define RSC_ID sv
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 29
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 6
#define NUM_OBS 27
#define NUM_TREE 1
#endif



#define RS232                              0 /* free form */
#define RBOX                               2 /* BOX in tree RS232 */
#define BAUD                               3 /* BOXTEXT in tree RS232 */ /* max len 5 */
#define PBOX                               5 /* BOX in tree RS232 */
#define PARITY                             6 /* BOXTEXT in tree RS232 */ /* max len 4 */
#define CBOX                               8 /* BOX in tree RS232 */
#define BITSCHAR                           9 /* BOXTEXT in tree RS232 */ /* max len 1 */
#define BBOX                              11 /* BOX in tree RS232 */
#define STOPX                             12 /* BOXTEXT in tree RS232 */ /* max len 1 */
#define SBOX                              14 /* BOX in tree RS232 */
#define PORT                              15 /* BOXTEXT in tree RS232 */ /* max len 1 */
#define FBOX                              18 /* BOX in tree RS232 */
#define FLOW                              19 /* BOXTEXT in tree RS232 */ /* max len 4 */
#define OKBOX                             22 /* BOX in tree RS232 */
#define SAVEBOX                           23 /* BOX in tree RS232 */
#define SAVE                              24 /* BUTTON in tree RS232 */
#define RS232OK                           25 /* BUTTON in tree RS232 */
#define RS232CAN                          26 /* BUTTON in tree RS232 */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD sv_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD sv_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD sv_rsc_free(void);
#endif
