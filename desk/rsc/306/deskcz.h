/*
 * resource set indices for deskcz
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        420
 * Number of Bitblks:        1
 * Number of Iconblks:       14
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       46
 * Number of Free Strings:   52
 * Number of Free Images:    0
 * Number of Objects:        427
 * Number of Trees:          24
 * Number of Userblks:       0
 * Number of Images:         29
 * Total file size:          23734
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "deskcz"
#endif
#undef RSC_ID
#ifdef deskcz
#define RSC_ID deskcz
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 420
#define NUM_FRSTR 52
#define NUM_UD 0
#define NUM_IMAGES 29
#define NUM_BB 1
#define NUM_FRIMG 0
#define NUM_IB 14
#define NUM_CIB 0
#define NUM_TI 46
#define NUM_OBS 427
#define NUM_TREE 24
#endif



#define ADMENU                             0 /* menu */
#define DESKMENU                           3 /* TITLE in tree ADMENU */
#define FILEMENU                           4 /* TITLE in tree ADMENU */
#define VIEWMENU                           5 /* TITLE in tree ADMENU */
#define OPTNMENU                           6 /* TITLE in tree ADMENU */
#define ABOUITEM                           9 /* STRING in tree ADMENU */
#define OPENITEM                          18 /* STRING in tree ADMENU */
#define SHOWITEM                          19 /* STRING in tree ADMENU */
#define SEAMENU                           20 /* STRING in tree ADMENU */
#define DELMENU                           22 /* STRING in tree ADMENU */
#define NEWFITEM                          24 /* STRING in tree ADMENU */
#define CLSFITEM                          25 /* STRING in tree ADMENU */
#define CLSWITEM                          26 /* STRING in tree ADMENU */
#define BOTTOP                            27 /* STRING in tree ADMENU */
#define SELALL                            28 /* STRING in tree ADMENU */
#define EXTMENU                           29 /* STRING in tree ADMENU */
#define L3ITEM                            30 /* STRING in tree ADMENU */
#define FORMITEM                          31 /* STRING in tree ADMENU */
#define ICONITEM                          33 /* STRING in tree ADMENU */
#define TEXTITEM                          34 /* STRING in tree ADMENU */
#define L4ITEM                            35 /* STRING in tree ADMENU */
#define NAMEITEM                          36 /* STRING in tree ADMENU */
#define DATEITEM                          37 /* STRING in tree ADMENU */
#define SIZEITEM                          38 /* STRING in tree ADMENU */
#define TYPEITEM                          39 /* STRING in tree ADMENU */
#define NOSORT                            40 /* STRING in tree ADMENU */
#define SIZEFIT                           42 /* STRING in tree ADMENU */
#define SCOLTYPE                          43 /* STRING in tree ADMENU */
#define IDSKITEM                          46 /* STRING in tree ADMENU */
#define IAPPITEM                          47 /* STRING in tree ADMENU */
#define INSDISK                           48 /* STRING in tree ADMENU */
#define REMVICON                          49 /* STRING in tree ADMENU */
#define L5ITEM                            50 /* STRING in tree ADMENU */
#define PREFITEM                          51 /* STRING in tree ADMENU */
#define READINF                           52 /* STRING in tree ADMENU */
#define MEMMENU                           53 /* STRING in tree ADMENU */
#define SAVEITEM                          55 /* STRING in tree ADMENU */
#define PRINTITEM                         56 /* STRING in tree ADMENU */
#define SUPERITEM                         58 /* STRING in tree ADMENU */
#define BITBLT                            59 /* STRING in tree ADMENU */

#define ADFILEIN                           1 /* form/dialog */
#define INFTITLE                           1 /* TEXT in tree ADFILEIN */ /* max len 1 */
#define FIFILE                             2 /* FTEXT in tree ADFILEIN */ /* max len 24 */
#define FILEFT                             3 /* BOXCHAR in tree ADFILEIN */
#define FIRIGHT                            4 /* BOXCHAR in tree ADFILEIN */
#define FINAME                             5 /* FTEXT in tree ADFILEIN */ /* max len 11 */
#define FISIZE                             6 /* FTEXT in tree ADFILEIN */ /* max len 10 */
#define FIDATE                             7 /* FTEXT in tree ADFILEIN */ /* max len 6 */
#define FITIME                             8 /* FTEXT in tree ADFILEIN */ /* max len 6 */
#define FINFOLDS                           9 /* FTEXT in tree ADFILEIN */ /* max len 5 */
#define FINFILES                          10 /* FTEXT in tree ADFILEIN */ /* max len 5 */
#define FIRONLY                           11 /* BUTTON in tree ADFILEIN */
#define FIRWRITE                          12 /* BUTTON in tree ADFILEIN */
#define FIATTRS                           13 /* STRING in tree ADFILEIN */
#define FIOK                              14 /* BUTTON in tree ADFILEIN */
#define FISKIP                            15 /* BUTTON in tree ADFILEIN */
#define FICNCL                            16 /* BUTTON in tree ADFILEIN */

#define ADDISKIN                           2 /* form/dialog */
#define DIDRIVE                            2 /* FTEXT in tree ADDISKIN */ /* max len 1 */
#define DIVOLUME                           3 /* FTEXT in tree ADDISKIN */ /* max len 11 */
#define DINFOLDS                           4 /* FTEXT in tree ADDISKIN */ /* max len 5 */
#define DINFILES                           5 /* FTEXT in tree ADDISKIN */ /* max len 5 */
#define DIUSED                             6 /* FTEXT in tree ADDISKIN */ /* max len 10 */
#define DIAVAIL                            7 /* FTEXT in tree ADDISKIN */ /* max len 10 */
#define DIOK                               8 /* BUTTON in tree ADDISKIN */
#define DIQUIT                             9 /* BUTTON in tree ADDISKIN */

#define ADDINFO                            3 /* form/dialog */
#define DEOK                               9 /* BUTTON in tree ADDINFO */

#define ADOPENAP                           4 /* form/dialog */
#define APPLNAME                           1 /* FTEXT in tree ADOPENAP */ /* max len 11 */
#define APPLPARM                           3 /* FTEXT in tree ADOPENAP */ /* max len 38 */
#define APPLOK                             4 /* BUTTON in tree ADOPENAP */
#define APPLCNCL                           5 /* BUTTON in tree ADOPENAP */

#define ADINSDIS                           5 /* form/dialog */
#define DRID                               2 /* FTEXT in tree ADINSDIS */ /* max len 1 */
#define DRLABEL                            3 /* FTEXT in tree ADINSDIS */ /* max len 12 */
#define IDRIVE                             6 /* BUTTON in tree ADINSDIS */
#define ITRASH                             7 /* BUTTON in tree ADINSDIS */
#define IPRINTER                           8 /* BUTTON in tree ADINSDIS */
#define IBOX                               9 /* BOX in tree ADINSDIS */
#define IICON                             10 /* ICON in tree ADINSDIS */ /* max len 0 */
#define IUP                               11 /* BOXCHAR in tree ADINSDIS */
#define IDOWN                             13 /* BOXCHAR in tree ADINSDIS */
#define DROK                              14 /* BUTTON in tree ADINSDIS */
#define DRSKIP                            15 /* BUTTON in tree ADINSDIS */
#define DRCANCEL                          16 /* BUTTON in tree ADINSDIS */

#define ADINSAPP                           6 /* form/dialog */
#define APNAME                             2 /* FTEXT in tree ADINSAPP */ /* max len 11 */
#define ARGS                               3 /* FTEXT in tree ADINSAPP */ /* max len 11 */
#define APDFTYPE                           4 /* FTEXT in tree ADINSAPP */ /* max len 3 */
#define IKEY                               5 /* FTEXT in tree ADINSAPP */ /* max len 2 */
#define AUTOBOX                            7 /* BUTTON in tree ADINSAPP */
#define NORMALBOX                          8 /* BUTTON in tree ADINSAPP */
#define APDOS                             11 /* BUTTON in tree ADINSAPP */
#define APPARMS                           12 /* BUTTON in tree ADINSAPP */
#define APGEM                             13 /* BUTTON in tree ADINSAPP */
#define APPTP                             14 /* BUTTON in tree ADINSAPP */
#define INSAPP                            16 /* BUTTON in tree ADINSAPP */
#define INSWIN                            17 /* BUTTON in tree ADINSAPP */
#define INSFULL                           20 /* BUTTON in tree ADINSAPP */
#define INSFILE                           21 /* BUTTON in tree ADINSAPP */
#define APCANCEL                          23 /* BUTTON in tree ADINSAPP */
#define APOK                              24 /* BUTTON in tree ADINSAPP */
#define APREMOVE                          25 /* BUTTON in tree ADINSAPP */
#define APSKIP                            26 /* BUTTON in tree ADINSAPP */

#define ADMKDBOX                           7 /* form/dialog */
#define MKNAME                             1 /* FTEXT in tree ADMKDBOX */ /* max len 11 */
#define MKOK                               2 /* BUTTON in tree ADMKDBOX */
#define MKCNCL                             4 /* BUTTON in tree ADMKDBOX */

#define ADSETPREF                          8 /* form/dialog */
#define SPCDYES                            4 /* BUTTON in tree ADSETPREF */
#define SPCDNO                             5 /* BUTTON in tree ADSETPREF */
#define SPCCYES                            7 /* BUTTON in tree ADSETPREF */
#define SPCCNO                             8 /* BUTTON in tree ADSETPREF */
#define YWRITE                            10 /* BUTTON in tree ADSETPREF */
#define NWRITE                            11 /* BUTTON in tree ADSETPREF */
#define SPLOW                             14 /* BUTTON in tree ADSETPREF */
#define SPMEDIUM                          15 /* BUTTON in tree ADSETPREF */
#define SPHIGH                            16 /* BUTTON in tree ADSETPREF */
#define SPEXT1                            17 /* BUTTON in tree ADSETPREF */
#define SPEXT2                            18 /* BUTTON in tree ADSETPREF */
#define SPEXT3                            19 /* BUTTON in tree ADSETPREF */
#define SPOK                              21 /* BUTTON in tree ADSETPREF */
#define SPCANCEL                          22 /* BUTTON in tree ADSETPREF */

#define ADFORMAT                           9 /* form/dialog */
#define FCBOX4                             1 /* BOX in tree ADFORMAT */
#define FCBOXC                             2 /* BOX in tree ADFORMAT */
#define DESTDR                             3 /* BOXCHAR in tree ADFORMAT */
#define SRCDRB                             5 /* BOXCHAR in tree ADFORMAT */
#define SRCDRA                             6 /* BOXCHAR in tree ADFORMAT */
#define FCCOPY                             7 /* BUTTON in tree ADFORMAT */
#define FCFORMAT                           9 /* BUTTON in tree ADFORMAT */
#define FCBOXF                            10 /* BOX in tree ADFORMAT */
#define DRIVE                             11 /* STRING in tree ADFORMAT */
#define ADRIVE                            12 /* BOXCHAR in tree ADFORMAT */
#define BDRIVE                            13 /* BOXCHAR in tree ADFORMAT */
#define FCLABEL                           14 /* FTEXT in tree ADFORMAT */ /* max len 11 */
#define FCSINGLE                          16 /* BUTTON in tree ADFORMAT */
#define FCDOUBLE                          17 /* BUTTON in tree ADFORMAT */
#define FCHIGH                            18 /* BUTTON in tree ADFORMAT */
#define FCBARA                            19 /* BOX in tree ADFORMAT */
#define FCBARB                            22 /* BOX in tree ADFORMAT */
#define FCOK                              23 /* BUTTON in tree ADFORMAT */
#define FCCNCL                            24 /* BUTTON in tree ADFORMAT */

#define ADICON                            10 /* unknown form */
#define ICON0                              1 /* ICON in tree ADICON */ /* max len 12 */
#define ICON1                              2 /* ICON in tree ADICON */ /* max len 12 */
#define ICON2                              3 /* ICON in tree ADICON */ /* max len 12 */
#define ICON3                              4 /* ICON in tree ADICON */ /* max len 12 */
#define ICON4                              5 /* ICON in tree ADICON */ /* max len 12 */

#define PRINTFIL                          11 /* unknown form */
#define PFILE                              1 /* FTEXT in tree PRINTFIL */ /* max len 11 */

#define BACKGROUND                        12 /* unknown form */

#define SSCREEN                           13 /* form/dialog */
#define STITLE                             1 /* TEXT in tree SSCREEN */ /* max len 23 */
#define PAT0                               7 /* BOX in tree SSCREEN */
#define PAT1                               8 /* BOX in tree SSCREEN */
#define PAT2                               9 /* BOX in tree SSCREEN */
#define PAT3                              10 /* BOX in tree SSCREEN */
#define PAT4                              11 /* BOX in tree SSCREEN */
#define PAT5                              12 /* BOX in tree SSCREEN */
#define PAT6                              13 /* BOX in tree SSCREEN */
#define PAT7                              14 /* BOX in tree SSCREEN */
#define OUTBOX                            15 /* BOX in tree SSCREEN */
#define PATTERN                           16 /* BOX in tree SSCREEN */
#define COLOR0                            18 /* BOX in tree SSCREEN */
#define COLOR1                            19 /* BOX in tree SSCREEN */
#define COLOR2                            20 /* BOX in tree SSCREEN */
#define COLOR3                            21 /* BOX in tree SSCREEN */
#define COLOR4                            22 /* BOX in tree SSCREEN */
#define COLOR5                            23 /* BOX in tree SSCREEN */
#define COLOR6                            24 /* BOX in tree SSCREEN */
#define COLOR7                            25 /* BOX in tree SSCREEN */
#define COLOR8                            26 /* BOX in tree SSCREEN */
#define COLOR9                            27 /* BOX in tree SSCREEN */
#define COLOR10                           28 /* BOX in tree SSCREEN */
#define COLOR11                           29 /* BOX in tree SSCREEN */
#define COLOR12                           30 /* BOX in tree SSCREEN */
#define COLOR13                           31 /* BOX in tree SSCREEN */
#define COLOR14                           32 /* BOX in tree SSCREEN */
#define COLOR15                           33 /* BOX in tree SSCREEN */
#define SOK                               34 /* BUTTON in tree SSCREEN */
#define SCANCEL                           35 /* BUTTON in tree SSCREEN */
#define WINPREF                           36 /* BUTTON in tree SSCREEN */
#define DESKPREF                          37 /* BUTTON in tree SSCREEN */

#define FILEOPTI                          14 /* form/dialog */
#define OPTNAME                            2 /* FTEXT in tree FILEOPTI */ /* max len 11 */
#define OPTOK                              3 /* BUTTON in tree FILEOPTI */
#define OPTCANCEL                          4 /* BUTTON in tree FILEOPTI */

#define DLAUNCH                           15 /* form/dialog */
#define RUNNAME                            2 /* FTEXT in tree DLAUNCH */ /* max len 37 */
#define LTOS                               4 /* BUTTON in tree DLAUNCH */
#define LGRAPHIC                           5 /* BUTTON in tree DLAUNCH */
#define LAUNCHOK                           7 /* BUTTON in tree DLAUNCH */
#define LNCNCL                             8 /* BUTTON in tree DLAUNCH */

#define TEXTLINES                         16 /* unknown form */
#define TSTYLE                             1 /* TEXT in tree TEXTLINES */ /* max len 48 */

#define SAMENAME                          17 /* form/dialog */
#define SNAME                              1 /* TEXT in tree SAMENAME */ /* max len 1 */
#define FNAME                              2 /* FTEXT in tree SAMENAME */ /* max len 11 */
#define EDFNAME                            3 /* FTEXT in tree SAMENAME */ /* max len 11 */
#define COPY                               4 /* BUTTON in tree SAMENAME */
#define SKIP                               5 /* BUTTON in tree SAMENAME */
#define QUIT                               6 /* BUTTON in tree SAMENAME */

#define CPBOX                             18 /* form/dialog */
#define TITLE                              1 /* TEXT in tree CPBOX */ /* max len 1 */
#define NUMDIR                             2 /* FTEXT in tree CPBOX */ /* max len 5 */
#define NUMFILE                            3 /* FTEXT in tree CPBOX */ /* max len 5 */
#define CSDRIVE                            4 /* FTEXT in tree CPBOX */ /* max len 1 */
#define CDDRIVE                            5 /* FTEXT in tree CPBOX */ /* max len 1 */
#define HIDECBOX                           6 /* BOX in tree CPBOX */
#define CPFILE                             7 /* FTEXT in tree CPBOX */ /* max len 11 */
#define CPDIR                              8 /* FTEXT in tree CPBOX */ /* max len 11 */
#define DATEBOX                            9 /* BOX in tree CPBOX */
#define PYES                              11 /* BUTTON in tree CPBOX */
#define PNO                               12 /* BUTTON in tree CPBOX */
#define CHBOX1                            13 /* BOX in tree CPBOX */
#define OKCP                              14 /* BUTTON in tree CPBOX */
#define CCANCEL                           15 /* BUTTON in tree CPBOX */

#define INWICON                           19 /* form/dialog */
#define WNAME                              3 /* FTEXT in tree INWICON */ /* max len 11 */
#define WBOX                               5 /* BOX in tree INWICON */
#define WICON                              6 /* ICON in tree INWICON */ /* max len 0 */
#define WUP                                7 /* BOXCHAR in tree INWICON */
#define WDOWN                              8 /* BOXCHAR in tree INWICON */
#define WFOLDER                           10 /* BUTTON in tree INWICON */
#define WNONE                             11 /* BUTTON in tree INWICON */
#define WREMOVE                           12 /* BUTTON in tree INWICON */
#define WOK                               13 /* BUTTON in tree INWICON */
#define WCANCEL                           15 /* BUTTON in tree INWICON */
#define WSKIP                             16 /* BUTTON in tree INWICON */

#define HELP1                             20 /* form/dialog */
#define MOREHELP                          15 /* BUTTON in tree HELP1 */

#define HELP2                             21 /* form/dialog */

#define SEAFILE                           22 /* form/dialog */
#define SFILE                              2 /* FTEXT in tree SEAFILE */ /* max len 11 */
#define SEAOK                              3 /* BUTTON in tree SEAFILE */
#define SEACNCL                            4 /* BUTTON in tree SEAFILE */

#define SSYSTEM                           23 /* form/dialog */
#define WI1                                4 /* BUTTON in tree SSYSTEM */
#define AP1                                5 /* BUTTON in tree SSYSTEM */
#define DEFFILE                            8 /* BUTTON in tree SSYSTEM */
#define DEFFULL                            9 /* BUTTON in tree SSYSTEM */
#define SDLEFT                            12 /* BOXCHAR in tree SSYSTEM */
#define SDFILE                            13 /* FTEXT in tree SSYSTEM */ /* max len 17 */
#define SDRIGHT                           14 /* BOXCHAR in tree SSYSTEM */
#define SDKEY                             15 /* FTEXT in tree SSYSTEM */ /* max len 2 */
#define SDUP                              16 /* BOXCHAR in tree SSYSTEM */
#define SDDOWN                            17 /* BOXCHAR in tree SSYSTEM */
#define MKSTR                             21 /* BOXTEXT in tree SSYSTEM */ /* max len 30 */
#define MKUP                              22 /* BOXCHAR in tree SSYSTEM */
#define MKDOWN                            23 /* BOXCHAR in tree SSYSTEM */
#define MKKEY                             24 /* FTEXT in tree SSYSTEM */ /* max len 1 */
#define CLRKEYS                           25 /* BUTTON in tree SSYSTEM */
#define SDMEMORY                          26 /* FTEXT in tree SSYSTEM */ /* max len 10 */
#define SDOK                              27 /* BUTTON in tree SSYSTEM */
#define SDCANCEL                          28 /* BUTTON in tree SSYSTEM */

#define STNOAPPL                           0 /* Alert string */
/* [1][Tento soubor m–‘e b˜t pouze|zobrazen nebo vyti¨tˆn.][Zobrazit|Tisknout|Zpˆt] */

#define STNOWIND                           1 /* Alert string */
/* [1][GEM Desktop nem  v¡ce oken.|M–‘e¨ zav©¡t okno, kter‚|pr vˆ nepou‘¡v ¨.][  OK  ] */

#define STFOEXISTS                         2 /* Alert string */
/* [2][Nelze vytvo©it po©ada‡ s t¡mto|jm‚nem. Zmˆ¤ jm‚no nebo Zpˆt.][Znova|  Zpˆt  ] */

#define STDISKFULL                         3 /* Alert string */
/* [1][ Disk %S: nem  dost voln‚ho | m¡sta pro tuto operaci.][  OK  ] */

#define STFO8DEE                           4 /* Alert string */
/* [1][Tak hluboko v adres ©ov‚m|stromu nelze po©ada‡ vytvo©it|ani otev©¡t.][  OK  ] */

#define STBADCOPY                          5 /* Alert string */
/* [1][ Nedovolen  operace! ][  OK  ] */

#define TRSINFO                            6 /* Alert string */
/* [1][%S se pou‘¡v |k smaz n¡ soubor– a po©ada‡–.][  OK  ] */

#define FCNOMEM                            7 /* Alert string */
/* [3][Syst‚m nem  volnou pamˆŸ!][  OK  ] */

#define FCFAIL                             8 /* Alert string */
/* [3][Chyba p©i form tov n¡ nebo|p©i kop¡rov n¡. Nen¡ c¡lov˜|disk zaji¨tˆn proti z pisu ?][Znova|  Konec  ] */

#define FCSIZE                             9 /* Alert string */
/* [1][Tento disk m  %L bajt–|voln˜ch pro u‘ivatele.][D le| Odej¡t ] */

#define FCNOTYPE                          10 /* Alert string */
/* [3][Diskety nemaj¡ shodn˜ form t.|Vlo‘ spr vnˆ naform tovanou|disketu do c¡lov‚ jednotky.][Znova|  Konec  ] */

#define SAVETOP                           11 /* Alert string */
/* [1][Ulo‘it stav Desktopu?][OK|  Zpˆt  ] */

#define PRINTTOP                          12 /* Alert string */
/* [1][Tisknout obrazovku?][OK|  Zpˆt  ] */

#define ABORTCON                          13 /* Alert string */
/* [1][P©eru¨it operaci?][  Ano  |Ne] */

#define NOICON                            14 /* Alert string */
/* [1][Promi¤, nelze p©ihl sit|dal¨¡ ikonu na Desktop.][  OK  ] */

#define NOAPP                             15 /* Alert string */
/* [1][Promi¤, nelze p©ihl sit|dal¨¡ aplikaci na Desktop.][  OK  ] */

#define NODRIVE                           16 /* Alert string */
/* [1][Disk %S: neexistuje.][  Zpˆt  ] */

#define NOOUTPUT                          17 /* Alert string */
/* [1][V˜stupn¡ za©¡zen¡|nep©ij¡m  data!][Znova|  Zpˆt  ] */

#define CNTOPEN                           18 /* Alert string */
/* [1][Soubor %S|nelze otev©¡t.][D le|Znova| Konec ] */

#define CNTCRTDR                          19 /* Alert string */
/* [1][Po©ada‡ %S|nelze vytvo©it.][D le|Znova| Konec ] */

#define WRERROR                           20 /* Alert string */
/* [1][Chyba p©i kop¡rov n¡ souboru.][ P©esko‡ |Konec] */

#define RDERROR                           21 /* Alert string */
/* [1][Chyba p©i ‡ten¡ souboru.][D le|  Konec  ] */

#define CNTCRTFL                          22 /* Alert string */
/* [1][Soubor %S|nelze vytvo©it.][D le|Znova| Konec ] */

#define CNTDELF                           23 /* Alert string */
/* [1][Soubor %S|nelze smazat.][P©esko‡|Znova|Konec] */

#define CNTDELD                           24 /* Alert string */
/* [1][Po©ada‡ %S|nelze smazat.][P©esko‡|Znova|Konec] */

#define PRINTINF                          25 /* Alert string */
/* [1][%S je tisk rna.][  OK  ] */

#define RENAME                            26 /* Alert string */
/* [1][Chyba p©i p©ejmenov v n¡!][Znova|  Zpˆt  ] */

#define NOBUF                             27 /* Alert string */
/* [1][Cesta je p©¡li¨ dlouh .][  OK  ] */

#define DELDISK                           28 /* Alert string */
/* [2][Opravdu chce¨ smazat v¨echny|soubory na disku %S?][Ano|  Ne  ] */

#define NOTPRG                            29 /* Alert string */
/* [1][%S|nen¡ aplikace!][  OK  ] */

#define NOSETDIR                          30 /* Alert string */
/* [1][Standardn¡ po©ada‡|nelze otev©¡t.][  OK  ] */

#define NOAUTO                            31 /* Alert string */
/* [1][AUTO soubor nelze zapsat.][  OK  ] */

#define NOWINMEM                          32 /* Alert string */
/* [1][PamˆŸ okna nelze uvolnit.][  OK  ] */

#define NOTYPE                            33 /* Alert string */
/* [1][Nespr vn˜ typ souboru.][  OK  ] */

#define ICONTYPE                          34 /* Alert string */
/* [1][Vyber si typ ikony pros¡m.][Desktop|Okno|  Zpˆt  ] */

#define NODIRCOP                          35 /* Alert string */
/* [1][Nespr vn  operace s po©ada‡em.][D le|  Konec  ] */

#define NOINSTAL                          36 /* Alert string */
/* [1][Nelze p©ihl sit.][  OK  ] */

#define NOMORE                            37 /* Alert string */
/* [1][Dal¨¡ soubory nejsou.][  OK  ] */

#define NOID                              38 /* Alert string */
/* [1][Chyb¡ ozna‡en¡ disku.][  Znova  ] */

#define ILLMAT                            39 /* Alert string */
/* [1][›patn˜ form t INF. souboru.][  OK  ] */

#define NOCART                            40 /* Alert string */
/* [1][ROM modul nen¡.][  OK  ] */

#define NOMATCH                           41 /* Alert string */
/* [1][Soubor tohoto typu nenalezen.][  OK  ] */

#define DELITEM                           42 /* Alert string */
/* [1][Chce¨ odstranit ikony|nebo smazat soubory?][Odstranit|Smazat|Zpˆt] */

#define MCFAIL                            43 /* Alert string */
/* [1][Selhalo organizov n¡ pamˆti.][  OK  ] */

#define FNOTFIND                          44 /* Alert string */
/* [1][%S nenalezen.][  OK  ] */

#define SNEXT                             45 /* Alert string */
/* [1][Pokra‡ovat v hled n¡?][OK|  Zpˆt  ] */

#define NOHELP                            46 /* Alert string */
/* [1][Pomoc nem–‘e b˜t zobrazena|v tomto obrazov‚m rozli¨en¡.][  OK  ] */

#define NOSEA                             47 /* Alert string */
/* [1][V ROM modulu nelze hledat.][  OK  ] */

#define KEYUSED                           48 /* Alert string */
/* [1][Tato kl vesa je ji‘ p©i©azena.|P©epsat?][OK|  Zpˆt  ] */

#define CARTNOSH                          49 /* Alert string */
/* [1][Nedovolen  operace|pro ROM modul.][  OK  ] */

#define BADKEY                            50 /* Alert string */
/* [1][Nespr vn˜ vstup pro|funk‡n¡ kl vesu.][  OK  ] */

#define XNFILE                            51 /* Alert string */
/* [1][%S nenalezen.|Chce¨ odstranit ikonu nebo|naj¡t objekt?][Odstranit|Hledat|Zpˆt] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD deskcz_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD deskcz_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD deskcz_rsc_free(void);
#endif
