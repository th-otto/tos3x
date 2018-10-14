/*
 * resource set indices for en
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        102
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       11
 * Number of Free Strings:   6
 * Number of Free Images:    0
 * Number of Objects:        130
 * Number of Trees:          2
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          4522
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
#define NUM_STRINGS 102
#define NUM_FRSTR 6
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 11
#define NUM_OBS 130
#define NUM_TREE 2
#endif



#define WCOLPANL                           0 /* free form */
#define SAVE                               2 /* BUTTON in tree WCOLPANL */
#define OK                                 4 /* BUTTON in tree WCOLPANL */
#define CANCEL                             5 /* BUTTON in tree WCOLPANL */
#define BGBOX                              6 /* IBOX in tree WCOLPANL */
#define BGTITLE                            7 /* BOX in tree WCOLPANL */
#define BGNAME                             8 /* BOXTEXT in tree WCOLPANL */ /* max len 3 */
#define BFULLER                            9 /* BOXCHAR in tree WCOLPANL */
#define BGINFO                            10 /* BOXTEXT in tree WCOLPANL */ /* max len 4 */
#define BGDATA                            11 /* IBOX in tree WCOLPANL */
#define BGWORK                            12 /* BOX in tree WCOLPANL */
#define BGSIZER                           13 /* BOXCHAR in tree WCOLPANL */
#define BGVBAR                            14 /* BOX in tree WCOLPANL */
#define BUP                               15 /* BOXCHAR in tree WCOLPANL */
#define BDN                               16 /* BOXCHAR in tree WCOLPANL */
#define BVELEV                            17 /* BOX in tree WCOLPANL */
#define BVSLID                            18 /* BOX in tree WCOLPANL */
#define BGHBAR                            19 /* BOX in tree WCOLPANL */
#define BLF                               20 /* BOXCHAR in tree WCOLPANL */
#define BRT                               21 /* BOXCHAR in tree WCOLPANL */
#define BHELEV                            22 /* BOX in tree WCOLPANL */
#define BHSLID                            23 /* BOX in tree WCOLPANL */
#define TBOX                              24 /* IBOX in tree WCOLPANL */
#define TTITLE                            25 /* BOX in tree WCOLPANL */
#define TCLOSER                           26 /* BOXCHAR in tree WCOLPANL */
#define TFULLER                           27 /* BOXCHAR in tree WCOLPANL */
#define TNAME                             28 /* BOXTEXT in tree WCOLPANL */ /* max len 8 */
#define TINFO                             29 /* BOXTEXT in tree WCOLPANL */ /* max len 5 */
#define TDATA                             30 /* IBOX in tree WCOLPANL */
#define TWORK                             31 /* BOX in tree WCOLPANL */
#define BORDSLID                          32 /* BOX in tree WCOLPANL */
#define BORDELEV                          33 /* BOXTEXT in tree WCOLPANL */ /* max len 2 */
#define TEXTSLID                          34 /* BOX in tree WCOLPANL */
#define TEXTELEV                          35 /* BOXTEXT in tree WCOLPANL */ /* max len 2 */
#define FILLSLID                          36 /* BOX in tree WCOLPANL */
#define FILLELEV                          37 /* BOXTEXT in tree WCOLPANL */ /* max len 2 */
#define MODE                              38 /* BOXTEXT in tree WCOLPANL */ /* max len 6 */
#define FILLS                             39 /* BOX in tree WCOLPANL */
#define FILL0                             40 /* BOX in tree WCOLPANL */
#define FILL1                             41 /* BOX in tree WCOLPANL */
#define FILL2                             42 /* BOX in tree WCOLPANL */
#define FILL3                             43 /* BOX in tree WCOLPANL */
#define FILL4                             44 /* BOX in tree WCOLPANL */
#define FILL5                             45 /* BOX in tree WCOLPANL */
#define FILL6                             46 /* BOX in tree WCOLPANL */
#define FILL7                             47 /* BOX in tree WCOLPANL */
#define BORD                              48 /* TEXT in tree WCOLPANL */ /* max len 7 */
#define TEXT                              49 /* TEXT in tree WCOLPANL */ /* max len 5 */
#define FILL                              50 /* TEXT in tree WCOLPANL */ /* max len 5 */
#define TVBAR                             51 /* BOX in tree WCOLPANL */
#define TUP                               52 /* BOXCHAR in tree WCOLPANL */
#define TDN                               53 /* BOXCHAR in tree WCOLPANL */
#define TVSLID                            54 /* BOX in tree WCOLPANL */
#define TVELEV                            55 /* BOX in tree WCOLPANL */
#define THBAR                             56 /* BOX in tree WCOLPANL */
#define TLF                               57 /* BOXCHAR in tree WCOLPANL */
#define TRT                               58 /* BOXCHAR in tree WCOLPANL */
#define THSLID                            59 /* BOX in tree WCOLPANL */
#define THELEV                            60 /* BOX in tree WCOLPANL */
#define TSIZER                            61 /* BOXCHAR in tree WCOLPANL */
#define FLOATER                           62 /* IBOX in tree WCOLPANL */

#define XLATE                              1 /* form/dialog */
#define USAOK                              3 /* BUTTON in tree XLATE */
#define USACAN                             4 /* BUTTON in tree XLATE */
#define USASAVE                            5 /* BUTTON in tree XLATE */
#define USAWIND                            6 /* BUTTON in tree XLATE */
#define USAINFO                            7 /* BUTTON in tree XLATE */
#define USABORD                            8 /* BUTTON in tree XLATE */
#define USATEXT                            9 /* BUTTON in tree XLATE */
#define USAFILL                           10 /* BUTTON in tree XLATE */
#define USAMODE                           11 /* BUTTON in tree XLATE */
#define FRGOK                             14 /* BUTTON in tree XLATE */
#define FRGCAN                            15 /* BUTTON in tree XLATE */
#define FRGSAVE                           16 /* BUTTON in tree XLATE */
#define FRGWIND                           17 /* BUTTON in tree XLATE */
#define FRGINFO                           18 /* BUTTON in tree XLATE */
#define FRGBORD                           19 /* BUTTON in tree XLATE */
#define FRGTEXT                           20 /* BUTTON in tree XLATE */
#define FRGFILL                           21 /* BUTTON in tree XLATE */
#define FRGMODE                           22 /* BUTTON in tree XLATE */
#define FRAOK                             25 /* BUTTON in tree XLATE */
#define FRACAN                            26 /* BUTTON in tree XLATE */
#define FRASAVE                           27 /* BUTTON in tree XLATE */
#define FRAWIND                           28 /* BUTTON in tree XLATE */
#define FRAINFO                           29 /* BUTTON in tree XLATE */
#define FRABORD                           30 /* BUTTON in tree XLATE */
#define FRATEXT                           31 /* BUTTON in tree XLATE */
#define FRAFILL                           32 /* BUTTON in tree XLATE */
#define FRAMODE                           33 /* BUTTON in tree XLATE */
#define SPAOK                             36 /* BUTTON in tree XLATE */
#define SPACAN                            37 /* BUTTON in tree XLATE */
#define SPASAVE                           38 /* BUTTON in tree XLATE */
#define SPAWIND                           39 /* BUTTON in tree XLATE */
#define SPAINFO                           40 /* BUTTON in tree XLATE */
#define SPABORD                           41 /* BUTTON in tree XLATE */
#define SPATEXT                           42 /* BUTTON in tree XLATE */
#define SPAFILL                           43 /* BUTTON in tree XLATE */
#define SPAMODE                           44 /* BUTTON in tree XLATE */
#define ITAOK                             47 /* BUTTON in tree XLATE */
#define ITACAN                            48 /* BUTTON in tree XLATE */
#define ITASAVE                           49 /* BUTTON in tree XLATE */
#define ITAWIND                           50 /* BUTTON in tree XLATE */
#define ITAINFO                           51 /* BUTTON in tree XLATE */
#define ITABORD                           52 /* BUTTON in tree XLATE */
#define ITATEXT                           53 /* BUTTON in tree XLATE */
#define ITAFILL                           54 /* BUTTON in tree XLATE */
#define ITAMODE                           55 /* BUTTON in tree XLATE */
#define SWEOK                             58 /* BUTTON in tree XLATE */
#define SWECAN                            59 /* BUTTON in tree XLATE */
#define SWESAVE                           60 /* BUTTON in tree XLATE */
#define SWEWIND                           61 /* BUTTON in tree XLATE */
#define SWEINFO                           62 /* BUTTON in tree XLATE */
#define SWEBORD                           63 /* BUTTON in tree XLATE */
#define SWETEXT                           64 /* BUTTON in tree XLATE */
#define SWEFILL                           65 /* BUTTON in tree XLATE */
#define SWEMODE                           66 /* BUTTON in tree XLATE */

#define ALERT                              0 /* Alert string */
/* [1][You can not set the|colors of windows|in this TOS version.][  OK  ] */

#define FRGLERT                            1 /* Alert string */
/* [1][In dieser TOS-Version|k”nnen keine Fenster-|farben gesetzt werden.][  OK  ] */

#define FRALERT                            2 /* Alert string */
/* [1][Vous ne pouvez pas d‚finir|les couleurs des fenˆtres dans|cette version du TOS.][ CONFIRME ] */

#define SPALERT                            3 /* Alert string */
/* [1][No es posible configurar|los colores de las ventanas|con esta versi¢n de TOS.][CONFIRMAR] */

#define ITALERT                            4 /* Alert string */
/* [1][Non Š possibile impostare|i colori delle finestre in|questa versione del TOS.][  OK  ] */

#define SWELERT                            5 /* Alert string */
/* [1][Det „r inte m”jligt att „ndra|f„rgerna p† f”nstren i denna|TOS-version.][  OK  ] */




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
