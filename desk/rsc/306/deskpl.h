/*
 * resource set indices for deskpl
 *
 * created by ORCS 2.14
 */

/*
 * Number of Strings:        450
 * Number of Bitblks:        1
 * Number of Iconblks:       14
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       63
 * Number of Free Strings:   52
 * Number of Free Images:    0
 * Number of Objects:        423
 * Number of Trees:          24
 * Number of Userblks:       0
 * Number of Images:         29
 * Total file size:          25168
 */

#undef RSC_NAME
#define RSC_NAME "deskpl"
#undef RSC_ID
#ifdef deskpl
#define RSC_ID deskpl
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 450
#define NUM_FRSTR 52
#define NUM_UD 0
#define NUM_IMAGES 29
#define NUM_BB 1
#define NUM_FRIMG 0
#define NUM_IB 14
#define NUM_CIB 0
#define NUM_TI 63
#define NUM_OBS 423
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
#define SPMED                             15 /* BUTTON in tree ADSETPREF */
#define SPHIGH                            16 /* BUTTON in tree ADSETPREF */
#define EXT1                              17 /* BUTTON in tree ADSETPREF */
#define EXT2                              18 /* BUTTON in tree ADSETPREF */
#define EXT3                              19 /* BUTTON in tree ADSETPREF */
#define SPOK                              21 /* BUTTON in tree ADSETPREF */
#define SPCANCEL                          22 /* BUTTON in tree ADSETPREF */

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
#define STITLE                             1 /* TEXT in tree SSCREEN */ /* max len 17 */
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
#define WOK                               14 /* BUTTON in tree INWICON */
#define WCANCEL                           16 /* BUTTON in tree INWICON */

#define HELP1                             20 /* form/dialog */
#define MOREHELP                          14 /* BUTTON in tree HELP1 */

#define HELP2                             21 /* form/dialog */

#define SEAFILE                           22 /* form/dialog */
#define SFILE                              2 /* FTEXT in tree SEAFILE */ /* max len 11 */
#define SEAOK                              3 /* BUTTON in tree SEAFILE */
#define SEACNCL                            4 /* BUTTON in tree SEAFILE */

#define SSYSTEM                           23 /* form/dialog */
#define AP1                                4 /* BUTTON in tree SSYSTEM */
#define WI1                                5 /* BUTTON in tree SSYSTEM */
#define DEFFULL                            8 /* BUTTON in tree SSYSTEM */
#define DEFFILE                            9 /* BUTTON in tree SSYSTEM */
#define MFBASE                            11 /* BOX in tree SSYSTEM */
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
/* [1][Ten dokument moŸesz jedynie|podejrze“ lub wydrukowa“.][PokaŸ|Drukuj|Anuluj] */

#define STNOWIND                           1 /* Alert string */
/* [1][Pulpit GEM nie moŸe otworzy“|wi”cej okien. Zamknij te|kt÷rych nie uŸywasz.][ Dalej ] */

#define STFOEXISTS                         2 /* Alert string */
/* [2][Nie moŸesz utworzy“ katalogu|o tej nazwie. Pon÷w i wprowadÿ|now— nazw” lub anuluj|tworzenie nowego katalogu.][ Pon÷w |Anuluj] */

#define STDISKFULL                         3 /* Alert string */
/* [1][ Na dysku %S: nie ma miejsca| koniecznego dla wykonania| tej operacji.][ Dalej ] */

#define STFO8DEE                           4 /* Alert string */
/* [1][Ten system nie moŸe utworzy“|lub otworzy“ katalogu, gdyŸ|jest on zbyt daleko w drzewie|katalogowym.][ Dalej ] */

#define STBADCOPY                          5 /* Alert string */
/* [1][ Nieprawid‘owa operacja! ][ Dalej ] */

#define TRSINFO                            6 /* Alert string */
/* [1][%S stosuje si” do|ca‘kowitego usuni”cia plik÷w|lub katalog÷w.][ Dalej ] */

#define FCNOMEM                            7 /* Alert string */
/* [3][Zabrak‘o pami”ci w systemie!][ Dalej ] */

#define FCFAIL                             8 /* Alert string */
/* [3][Powsta‘ b‘—d podczas kopiowania|lub formatowania. Dysk|docelowy jest zabezpieczony|lub uszkodzony.][ Pon÷w |Anuluj] */

#define FCSIZE                             9 /* Alert string */
/* [1][Na dysku dost”pne jest|%L bajt÷w wolnego|miejsca.][Kontynuuj| Wyjdÿ ] */

#define FCNOTYPE                          10 /* Alert string */
/* [3][Docelowy dysk nie jest tego|samego typu co Ÿr÷d‘owy.|Prosz” w‘oŸy“ inny dysk.][ Pon÷w |Przerwij] */

#define SAVETOP                           11 /* Alert string */
/* [1][ Zapisa“ pulpit ?][  Tak  | Anuluj ] */

#define PRINTTOP                          12 /* Alert string */
/* [1][Drukowa“ zrzut ekranu ? ][  Tak  | Anuluj ] */

#define ABORTCON                          13 /* Alert string */
/* [1][Przerwa“ t” operacj” ?][ Tak |Nie] */

#define NOICON                            14 /* Alert string */
/* [1][Przykro mi, Na pulpicie nie|moŸna zainstalowa“ wi”cej|ikon.][ Dalej ] */

#define NOAPP                             15 /* Alert string */
/* [1][Przykro mi, na pulpicie nie|moŸna zainstalowa“ wi”cej|program÷w.][ Dalej ] */

#define NODRIVE                           16 /* Alert string */
/* [1][Dysk %S: nie istnieje.][ Anuluj ] */

#define NOOUTPUT                          17 /* Alert string */
/* [1][Twoje urz—dzenie wyj◊ciowe|nie otrzymuje danych!][ Pon÷w | Przerwij ] */

#define CNTOPEN                           18 /* Alert string */
/* [1][Nie moge otworzy“ pliku|%S][ Pomi’ | Pon÷w |Przerwij] */

#define CNTCRTDR                          19 /* Alert string */
/* [1][Nie mog” utworzy“ katalogu|%S][ Pomi’ | Pon÷w |Przerwij] */

#define WRERROR                           20 /* Alert string */
/* [1][Powsta‘ b‘—d podczas|kopiowania pliku.][ Pomi’ |Przerwij] */

#define RDERROR                           21 /* Alert string */
/* [1][Powsta‘ b‘—d podczas|odczytu pliku ÿr÷d‘owego.][ Pomi’ |Przerwij] */

#define CNTCRTFL                          22 /* Alert string */
/* [1][Nie mog” utworzy“ pliku|%S][ Pomi’ | Pon÷w |Przerwij] */

#define CNTDELF                           23 /* Alert string */
/* [1][Nie mog” skasowa“ pliku|%S][ Pomi’ | Pon÷w |Przerwij] */

#define CNTDELD                           24 /* Alert string */
/* [1][Nie mog” skasowa“ katalogu|%S][ Pomi’ | Pon÷w |Przerwij] */

#define PRINTINF                          25 /* Alert string */
/* [1][%S|jest urz—dzeniem drukuj—cym.][ Dalej ] */

#define RENAME                            26 /* Alert string */
/* [1][ B‘—d zmiany nazwy! ][ Pon÷w |Anuluj] */

#define NOBUF                             27 /* Alert string */
/* [1][Nazwa ◊cieŸki za d‘uga.][ Dalej ] */

#define DELDISK                           28 /* Alert string */
/* [2][Czy chcesz skasowa“ wszystkie|pliki na dysku %S?][Tak| Nie ] */

#define NOTPRG                            29 /* Alert string */
/* [1][%S|nie jest programem.][ Dalej ] */

#define NOSETDIR                          30 /* Alert string */
/* [1][B‘—d w ustawieniu domy◊lnej |◊cieŸki.][ Dalej ] */

#define NOAUTO                            31 /* Alert string */
/* [1][B‘—d zapisu pliku AUTO.][ Dalej ] */

#define NOWINMEM                          32 /* Alert string */
/* [1][B‘—d u umiejscowieniu |pami”ci okna.][ Dalej ] */

#define NOTYPE                            33 /* Alert string */
/* [1][Niedozwolony typ piku.][ Dalej ] */

#define ICONTYPE                          34 /* Alert string */
/* [1][Prosz” wskaza“ typ ikony.][Pulpit|Okno|Anuluj] */

#define NODIRCOP                          35 /* Alert string */
/* [1][Nieprawid‘owe dzia‘anie na|katalogu.][ Pomi’ | Przerwij ] */

#define NOINSTAL                          36 /* Alert string */
/* [1][Tego elementu nie moŸna|instalowa“.][ Dalej ] */

#define NOMORE                            37 /* Alert string */
/* [1][Nie ma wi”cej plik÷w.][ Dalej ] */

#define NOID                              38 /* Alert string */
/* [1][Brak ID dysku.][ Pon÷w ] */

#define ILLMAT                            39 /* Alert string */
/* [1][Nieprawid‘owy format|pliku .INF][ Dalej ] */

#define NOCART                            40 /* Alert string */
/* [1][Brak magazynku ROM.][ Dalej ] */

#define NOMATCH                           41 /* Alert string */
/* [1][Brak odpowiedniego typu pliku.][ Dalej ] */

#define DELITEM                           42 /* Alert string */
/* [1][Chcesz usuna“ ikon” czy|skasowa“ plik?][Usu’|Skasuj| Anuluj ] */

#define MCFAIL                            43 /* Alert string */
/* [1][B‘—d czyszczenia pami”ci.][ Dalej ] */

#define FNOTFIND                          44 /* Alert string */
/* [1][%S nie znaleziony.][ Dalej ] */

#define SNEXT                             45 /* Alert string */
/* [1][Kontynuowa“ szukanie? ][Dalej| Anuluj ] */

#define NOHELP                            46 /* Alert string */
/* [1][Nie moŸna otworzy“ menu pomocy|w tej rozdzielczo◊ci.][ Dalej ] */

#define NOSEA                             47 /* Alert string */
/* [1][Nie moŸna przeszukiwa“|magazynku ROM.][ Dalej ] */

#define KEYUSED                           48 /* Alert string */
/* [1][Ten klawisz jest juŸ|zdefiniowany. Nadpisa“ go? ][Dalej| Anuluj ] */

#define CARTNOSH                          49 /* Alert string */
/* [1][Nie moŸna wy◊wietli“|informacji o magazynku ROM.][ Dalej ] */

#define BADKEY                            50 /* Alert string */
/* [1][Nieprawid‘owa warto◊“|klawisza funkcyjnego.][ Dalej ] */

#define XNFILE                            51 /* Alert string */
/* [1][%S nie znaleziony.|Chcesz usun—“ ikon” czy|zlokalizowa“ obiekt?][Usu’| WskaŸ |Anuluj] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD deskpl_rsc_load(void);
extern _WORD deskpl_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD deskpl_rsc_free(void);
#endif
