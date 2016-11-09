/*
 * resource set indices for deskde
 *
 * created by ORCS 2.14
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
 * Total file size:          24250
 */

#undef RSC_NAME
#define RSC_NAME "deskde"
#undef RSC_ID
#ifdef deskde
#define RSC_ID deskde
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
#define SAVEITEM                          55 /* STRING in tree ADMENU */
#define PRINTITEM                         56 /* STRING in tree ADMENU */
#define BITBLT                            58 /* STRING in tree ADMENU */
#define SUPERITEM                         59 /* STRING in tree ADMENU */

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

#define MNINSAPP                           6 /* form/dialog */
#define MPFNAME                            2 /* FTEXT in tree MNINSAPP */ /* max len 11 */
#define MPFARGS                            3 /* FTEXT in tree MNINSAPP */ /* max len 11 */
#define MPDTYPE                            4 /* FTEXT in tree MNINSAPP */ /* max len 3 */
#define MPITITLE                           5 /* FTEXT in tree MNINSAPP */ /* max len 2 */
#define MNAUTO                             7 /* BUTTON in tree MNINSAPP */
#define MNNORMAL                           8 /* BUTTON in tree MNINSAPP */
#define MNTOS                             11 /* BUTTON in tree MNINSAPP */
#define MNTTP                             12 /* BUTTON in tree MNINSAPP */
#define MNAPP                             13 /* BUTTON in tree MNINSAPP */
#define MNGTP                             14 /* BUTTON in tree MNINSAPP */
#define MNDIRAPP                          16 /* BUTTON in tree MNINSAPP */
#define MNDIRWIN                          17 /* BUTTON in tree MNINSAPP */
#define MNPATH                            20 /* BUTTON in tree MNINSAPP */
#define MNNAME                            21 /* BUTTON in tree MNINSAPP */
#define MPOK                              23 /* BUTTON in tree MNINSAPP */
#define MPREMOVE                          24 /* BUTTON in tree MNINSAPP */
#define MPSKIP                            25 /* BUTTON in tree MNINSAPP */
#define MPCANCEL                          26 /* BUTTON in tree MNINSAPP */

#define ADMKDBOX                           7 /* form/dialog */
#define MKNAME                             1 /* FTEXT in tree ADMKDBOX */ /* max len 11 */
#define MKOK                               2 /* BUTTON in tree ADMKDBOX */
#define MKCNCL                             4 /* BUTTON in tree ADMKDBOX */

#define MNSETPREF                          8 /* form/dialog */
#define SPDTITLE                           3 /* STRING in tree MNSETPREF */
#define SPDYES                             4 /* BUTTON in tree MNSETPREF */
#define SPDNO                              5 /* BUTTON in tree MNSETPREF */
#define SPCTITLE                           6 /* STRING in tree MNSETPREF */
#define SPCYES                             7 /* BUTTON in tree MNSETPREF */
#define SPCNO                              8 /* BUTTON in tree MNSETPREF */
#define SPOTITLE                           9 /* STRING in tree MNSETPREF */
#define SPOYES                            10 /* BUTTON in tree MNSETPREF */
#define SPONO                             11 /* BUTTON in tree MNSETPREF */
#define SVSTLOW                           14 /* BUTTON in tree MNSETPREF */
#define SVSTMED                           15 /* BUTTON in tree MNSETPREF */
#define SVSTHIGH                          16 /* BUTTON in tree MNSETPREF */
#define SVTTMED                           17 /* BUTTON in tree MNSETPREF */
#define SVTTHIGH                          18 /* BUTTON in tree MNSETPREF */
#define SVTTLOW                           19 /* BUTTON in tree MNSETPREF */
#define MNSPOK                            21 /* BUTTON in tree MNSETPREF */
#define MNSPCAN                           22 /* BUTTON in tree MNSETPREF */

#define ADFORMAT                           9 /* form/dialog */
#define FCBOX4                             1 /* BOX in tree ADFORMAT */
#define FCBOXC                             2 /* BOX in tree ADFORMAT */
#define DESTDR                             3 /* BOXCHAR in tree ADFORMAT */
#define SRCDRB                             5 /* BOXCHAR in tree ADFORMAT */
#define SRCDRA                             6 /* BOXCHAR in tree ADFORMAT */
#define FCCOPY                             7 /* BUTTON in tree ADFORMAT */
#define LINEBOX                            8 /* BOX in tree ADFORMAT */
#define FCFORMAT                           9 /* BUTTON in tree ADFORMAT */
#define FCBOXF                            10 /* BOX in tree ADFORMAT */
#define DRIVE                             11 /* STRING in tree ADFORMAT */
#define ADRIVE                            12 /* BOXCHAR in tree ADFORMAT */
#define BDRIVE                            13 /* BOXCHAR in tree ADFORMAT */
#define FCLABEL                           14 /* FTEXT in tree ADFORMAT */ /* max len 11 */
#define SIDEBOX                           15 /* BOX in tree ADFORMAT */
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
#define STITLE                             1 /* TEXT in tree SSCREEN */ /* max len 11 */
#define DESKPREF                           4 /* STRING in tree SSCREEN */
#define WINPREF                            5 /* STRING in tree SSCREEN */
#define PAT0                               9 /* BOX in tree SSCREEN */
#define PAT1                              10 /* BOX in tree SSCREEN */
#define PAT2                              11 /* BOX in tree SSCREEN */
#define PAT3                              12 /* BOX in tree SSCREEN */
#define PAT4                              13 /* BOX in tree SSCREEN */
#define PAT5                              14 /* BOX in tree SSCREEN */
#define PAT6                              15 /* BOX in tree SSCREEN */
#define PAT7                              16 /* BOX in tree SSCREEN */
#define OUTBOX                            17 /* BOX in tree SSCREEN */
#define PATTERN                           18 /* BOX in tree SSCREEN */
#define COLOR0                            20 /* BOX in tree SSCREEN */
#define COLOR1                            21 /* BOX in tree SSCREEN */
#define COLOR2                            22 /* BOX in tree SSCREEN */
#define COLOR3                            23 /* BOX in tree SSCREEN */
#define COLOR4                            24 /* BOX in tree SSCREEN */
#define COLOR5                            25 /* BOX in tree SSCREEN */
#define COLOR6                            26 /* BOX in tree SSCREEN */
#define COLOR7                            27 /* BOX in tree SSCREEN */
#define COLOR8                            28 /* BOX in tree SSCREEN */
#define COLOR9                            29 /* BOX in tree SSCREEN */
#define COLOR10                           30 /* BOX in tree SSCREEN */
#define COLOR11                           31 /* BOX in tree SSCREEN */
#define COLOR12                           32 /* BOX in tree SSCREEN */
#define COLOR13                           33 /* BOX in tree SSCREEN */
#define COLOR14                           34 /* BUTTON in tree SSCREEN */
#define COLOR15                           35 /* BUTTON in tree SSCREEN */
#define SOK                               36 /* BUTTON in tree SSCREEN */
#define SCANCEL                           37 /* BUTTON in tree SSCREEN */

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
#define IBOX1                              2 /* BOX in tree INWICON */
#define WNAME                              3 /* FTEXT in tree INWICON */ /* max len 11 */
#define WBOX                               5 /* BOX in tree INWICON */
#define WICON                              6 /* ICON in tree INWICON */ /* max len 0 */
#define WUP                                7 /* BOXCHAR in tree INWICON */
#define WDOWN                              8 /* BOXCHAR in tree INWICON */
#define WFOLDER                           10 /* BUTTON in tree INWICON */
#define WNONE                             11 /* BUTTON in tree INWICON */
#define WSKIP                             12 /* BUTTON in tree INWICON */
#define WREMOVE                           13 /* BUTTON in tree INWICON */
#define WOK                               14 /* STRING in tree INWICON */
#define WQUIT                             16 /* BUTTON in tree INWICON */

#define HELP1                             20 /* form/dialog */
#define MOREHELP                          14 /* STRING in tree HELP1 */

#define HELP2                             21 /* form/dialog */

#define SEAFILE                           22 /* form/dialog */
#define SFILE                              2 /* FTEXT in tree SEAFILE */ /* max len 11 */
#define SEAOK                              3 /* BUTTON in tree SEAFILE */
#define SEACNCL                            4 /* BUTTON in tree SEAFILE */

#define MNSYSTEM                          23 /* form/dialog */
#define SDTITLE                            2 /* STRING in tree MNSYSTEM */
#define SDAPP                              4 /* BUTTON in tree MNSYSTEM */
#define SDWIN                              5 /* BUTTON in tree MNSYSTEM */
#define SITITLE                            6 /* STRING in tree MNSYSTEM */
#define SIPATH                             8 /* BUTTON in tree MNSYSTEM */
#define SINAME                             9 /* BUTTON in tree MNSYSTEM */
#define MFBASE                            11 /* BOX in tree MNSYSTEM */
#define MFLEFT                            12 /* BOXCHAR in tree MNSYSTEM */
#define MFFILE                            13 /* FTEXT in tree MNSYSTEM */ /* max len 17 */
#define MFRIGHT                           14 /* BOXCHAR in tree MNSYSTEM */
#define MFKEY                             15 /* FTEXT in tree MNSYSTEM */ /* max len 2 */
#define MFUP                              16 /* BOXCHAR in tree MNSYSTEM */
#define MFDOWN                            17 /* BOXCHAR in tree MNSYSTEM */
#define MKITEM                            21 /* BOXTEXT in tree MNSYSTEM */ /* max len 30 */
#define MKUPS                             22 /* BOXCHAR in tree MNSYSTEM */
#define MKDOWNS                           23 /* BOXCHAR in tree MNSYSTEM */
#define MKKEYS                            24 /* FTEXT in tree MNSYSTEM */ /* max len 1 */
#define MKCLEAR                           25 /* BUTTON in tree MNSYSTEM */
#define MKMEMORY                          26 /* FTEXT in tree MNSYSTEM */ /* max len 10 */
#define MOK                               27 /* BUTTON in tree MNSYSTEM */
#define MCANCEL                           28 /* BUTTON in tree MNSYSTEM */

#define STNOAPPL                           0 /* Alert string */
/* [1][Diese Datei kann nur gedruckt|oder angezeigt werden.][Anzeigen|Drucken|Abbruch] */

#define STNOWIND                           1 /* Alert string */
/* [1][Keine weiteren Fenster|vorhanden! Abhilfe durch|Schlieûen eines Fensters.|         ][  OK  ] */

#define STFOEXISTS                         2 /* Alert string */
/* [2][UngÅltiger Ordnername! Bitte|den Namen Ñndern oder den|Vorgang abbrechen.|  ][ Nochmal |Abbruch] */

#define STDISKFULL                         3 /* Alert string */
/* [1][ Laufwerk %S: hat nicht | genÅgend Speicherplatz fÅr| diese Operation.][  OK  ] */

#define STFO8DEE                           4 /* Alert string */
/* [1][So tief im Indexpfad kînnen|keine Ordner angelegt oder|angesprochen werden.][  OK  ] */

#define STBADCOPY                          5 /* Alert string */
/* [1][ UngÅltige Operation! ][  OK  ] */

#define TRSINFO                            6 /* Alert string */
/* [1][%S wird benutzt um|Dateien und Ordner zu|lîschen.][  OK  ] */

#define FCNOMEM                            7 /* Alert string */
/* [3][Kein Systemspeicher mehr frei!][  OK  ] */

#define FCFAIL                             8 /* Alert string */
/* [3][Fehler beim Formatieren oder|Kopieren!|Eventuell Zieldiskette defekt.|    ][ Nochmal |Abbruch] */

#define FCSIZE                             9 /* Alert string */
/* [1][Diese Diskette hat|%L Bytes freien|Speicherplatz.][ Weiter | Abbruch ] */

#define FCNOTYPE                          10 /* Alert string */
/* [3][Disketten haben nicht das|gleiche Format! Bitte eine|korrekt formatierte Diskette|in das Ziellaufwerk einlegen.][ Nochmal | Abbruch ] */

#define SAVETOP                           11 /* Alert string */
/* [1][Arbeit sichern?][  OK  | Abbruch ] */

#define PRINTTOP                          12 /* Alert string */
/* [1][Bildschirm drucken?][  OK  | Abbruch ] */

#define ABORTCON                          13 /* Alert string */
/* [1][Funktion abbrechen?][ Ja | Nein ] */

#define NOICON                            14 /* Alert string */
/* [1][Verzeihung, es kînnen keine|weiteren Ikonen angemeldet|werden.][  OK  ] */

#define NOAPP                             15 /* Alert string */
/* [1][Verzeihung, es kînnen keine|weiteren Anwendungen ange-|meldet werden.][  OK  ] */

#define NODRIVE                           16 /* Alert string */
/* [1][Laufwerk %S: existiert nicht.][ Abbruch ] */

#define NOOUTPUT                          17 /* Alert string */
/* [1][AusgabegerÑt antwortet|nicht!][ Nochmal | Abbruch ] */

#define CNTOPEN                           18 /* Alert string */
/* [1][Datei %S kann|nicht geîffnet werden.][Weiter|Nochmal|Abbruch] */

#define CNTCRTDR                          19 /* Alert string */
/* [1][Verzeichnis %S|kann nicht angelegt werden.][Weiter|Nochmal|Abbruch] */

#define WRERROR                           20 /* Alert string */
/* [1][Fehler beim Kopieren|einer Datei.][ Weiter | Abbruch ] */

#define RDERROR                           21 /* Alert string */
/* [1][Fehler beim Lesen|einer Datei.][ Weiter | Abbruch ] */

#define CNTCRTFL                          22 /* Alert string */
/* [1][Datei %S kann|nicht angelegt werden.][Weiter|Nochmal|Abbruch] */

#define CNTDELF                           23 /* Alert string */
/* [1][Datei %S kann|nicht gelîscht werden.][Weiter|Nochmal|Abbruch] */

#define CNTDELD                           24 /* Alert string */
/* [1][Verzeichnis %S|kann nicht gelîscht werden.][Weiter|Nochmal|Abbruch] */

#define PRINTINF                          25 /* Alert string */
/* [1][%S|ist ein Drucker.][  OK  ] */

#define RENAME                            26 /* Alert string */
/* [1][Fehler beim Umbenennen!][ Nochmal | Abbruch ] */

#define NOBUF                             27 /* Alert string */
/* [1][Pfadname zu lang.][  OK  ] */

#define DELDISK                           28 /* Alert string */
/* [2][Wollen Sie wirklich alle|Dateien auf Laufwerk %S|lîschen?][ Ja | Nein ] */

#define NOTPRG                            29 /* Alert string */
/* [1][%S|ist keine Anwendung!][  OK  ] */

#define NOSETDIR                          30 /* Alert string */
/* [1][Standardverzeichnis konnte|nicht geîffnet werden.][  OK  ] */

#define NOAUTO                            31 /* Alert string */
/* [1][AUTO-Datei konnte nicht|geschrieben werden.][  OK  ] */

#define NOWINMEM                          32 /* Alert string */
/* [1][Fensterspeicher konnte nicht|angelegt werden.][  OK  ] */

#define NOTYPE                            33 /* Alert string */
/* [1][UngÅltiger Dateityp.][  OK  ] */

#define ICONTYPE                          34 /* Alert string */
/* [1][Bitte Ikonentyp wÑhlen.][Desktop|Fenster|Abbruch] */

#define NODIRCOP                          35 /* Alert string */
/* [1][UngÅltige Verzeichnisfunktion.][ Weiter | Abbruch ] */

#define NOINSTAL                          36 /* Alert string */
/* [1][Nicht installierbar.][ OK ] */

#define NOMORE                            37 /* Alert string */
/* [1][Keine weiteren Dateien.][  OK  ] */

#define NOID                              38 /* Alert string */
/* [1][Keine Laufwerkskennung.][ Nochmal ] */

#define ILLMAT                            39 /* Alert string */
/* [1][UngÅltiges .INF-Dateiformat.][ OK ] */

#define NOCART                            40 /* Alert string */
/* [1][Kein ROM-Modul.][ OK ] */

#define NOMATCH                           41 /* Alert string */
/* [1][Keine Datei diesen Typs|gefunden.][ OK ] */

#define DELITEM                           42 /* Alert string */
/* [1][Wollen Sie Ikonen entfernen|oder Dateien lîschen?][Entfernen|Lîschen|Abbruch] */

#define MCFAIL                            43 /* Alert string */
/* [1][Speicherreorganisation nicht|durchfÅhrbar.][ OK ] */

#define FNOTFIND                          44 /* Alert string */
/* [1][%S nicht gefunden.][ OK ] */

#define SNEXT                             45 /* Alert string */
/* [1][Suche fortsetzen?][OK| Abbruch ] */

#define NOHELP                            46 /* Alert string */
/* [1][Das HilfsmenÅ kann in dieser|Auflîsung nicht dargestellt|werden.][ OK ] */

#define NOSEA                             47 /* Alert string */
/* [1][In einem ROM-Modul kann nicht|gesucht werden.][ OK ] */

#define KEYUSED                           48 /* Alert string */
/* [1][Die Taste ist bereits belegt.|öberschreiben?][OK| Abbruch ] */

#define CARTNOSH                          49 /* Alert string */
/* [1][UngÅltige Funktion fÅr|ROM-Modul.][ OK ] */

#define BADKEY                            50 /* Alert string */
/* [1][UngÅltiger Eintrag fÅr|Funktionstaste.][ OK ] */

#define XNFILE                            51 /* Alert string */
/* [1][%S nicht gefunden.|lkone entfernen oder|das Objekt suchen.][Entfernen|Suchen|Abbruch] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD deskde_rsc_load(void);
extern _WORD deskde_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD deskde_rsc_free(void);
#endif
