/*
 * resource set indices for deskita
 *
 * created by ORCS 2.14
 */

/*
 * Number of Strings:        547
 * Number of Bitblks:        1
 * Number of Iconblks:       14
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       58
 * Number of Free Strings:   120
 * Number of Free Images:    0
 * Number of Objects:        469
 * Number of Trees:          27
 * Number of Userblks:       0
 * Number of Images:         29
 * Total file size:          27596
 */

#undef RSC_NAME
#define RSC_NAME "deskita"
#undef RSC_ID
#ifdef deskita
#define RSC_ID deskita
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 547
#define NUM_FRSTR 120
#define NUM_UD 0
#define NUM_IMAGES 29
#define NUM_BB 1
#define NUM_FRIMG 0
#define NUM_IB 14
#define NUM_CIB 0
#define NUM_TI 58
#define NUM_OBS 469
#define NUM_TREE 27
#endif



#define ADMENU     0 /* menu */
#define DESKMENU   3 /* TITLE in tree ADMENU */
#define FILEMENU   4 /* TITLE in tree ADMENU */
#define VIEWMENU   5 /* TITLE in tree ADMENU */
#define OPTNMENU   6 /* TITLE in tree ADMENU */
#define ABOUITEM   9 /* STRING in tree ADMENU */
#define OPENITEM  18 /* STRING in tree ADMENU */
#define SHOWITEM  19 /* STRING in tree ADMENU */
#define SEAMENU   20 /* STRING in tree ADMENU */
#define DELMENU   22 /* STRING in tree ADMENU */
#define NEWFITEM  24 /* STRING in tree ADMENU */
#define CLSFITEM  25 /* STRING in tree ADMENU */
#define CLSWITEM  26 /* STRING in tree ADMENU */
#define BOTTOP    27 /* STRING in tree ADMENU */
#define SELALL    28 /* STRING in tree ADMENU */
#define EXTMENU   29 /* STRING in tree ADMENU */
#define L3ITEM    30 /* STRING in tree ADMENU */
#define FORMITEM  31 /* STRING in tree ADMENU */
#define ICONITEM  33 /* STRING in tree ADMENU */
#define TEXTITEM  34 /* STRING in tree ADMENU */
#define L4ITEM    35 /* STRING in tree ADMENU */
#define NAMEITEM  36 /* STRING in tree ADMENU */
#define DATEITEM  37 /* STRING in tree ADMENU */
#define SIZEITEM  38 /* STRING in tree ADMENU */
#define TYPEITEM  39 /* STRING in tree ADMENU */
#define NOSORT    40 /* STRING in tree ADMENU */
#define SIZEFIT   42 /* STRING in tree ADMENU */
#define SCOLTYPE  43 /* STRING in tree ADMENU */
#define IDSKITEM  46 /* STRING in tree ADMENU */
#define IAPPITEM  47 /* STRING in tree ADMENU */
#define INSDISK   48 /* STRING in tree ADMENU */
#define REMVICON  49 /* STRING in tree ADMENU */
#define L5ITEM    50 /* STRING in tree ADMENU */
#define PREFITEM  51 /* STRING in tree ADMENU */
#define VIDITEM   52 /* STRING in tree ADMENU */
#define READINF   53 /* STRING in tree ADMENU */
#define MEMMENU   54 /* STRING in tree ADMENU */
#define SAVEITEM  56 /* STRING in tree ADMENU */

#define ADFILEIN   1 /* form/dialog */
#define INFTITLE   1 /* TEXT in tree ADFILEIN */ /* max len 1 */
#define FIFILE     2 /* FTEXT in tree ADFILEIN */ /* max len 24 */
#define FILEFT     3 /* BOXCHAR in tree ADFILEIN */
#define FIRIGHT    4 /* BOXCHAR in tree ADFILEIN */
#define FINAME     5 /* FTEXT in tree ADFILEIN */ /* max len 11 */
#define FISIZE     6 /* FTEXT in tree ADFILEIN */ /* max len 10 */
#define FIDATE     7 /* FTEXT in tree ADFILEIN */ /* max len 8 */
#define FITIME     8 /* FTEXT in tree ADFILEIN */ /* max len 6 */
#define FINFOLDS   9 /* FTEXT in tree ADFILEIN */ /* max len 5 */
#define FINFILES  10 /* FTEXT in tree ADFILEIN */ /* max len 5 */
#define FIRONLY   11 /* BUTTON in tree ADFILEIN */
#define FIRWRITE  12 /* BUTTON in tree ADFILEIN */
#define FIATTRS   13 /* STRING in tree ADFILEIN */
#define FIOK      14 /* BUTTON in tree ADFILEIN */
#define FISKIP    15 /* BUTTON in tree ADFILEIN */
#define FICNCL    16 /* BUTTON in tree ADFILEIN */

#define ADDISKIN   2 /* form/dialog */
#define DIDRIVE    2 /* FTEXT in tree ADDISKIN */ /* max len 1 */
#define DIVOLUME   3 /* FTEXT in tree ADDISKIN */ /* max len 11 */
#define DINFOLDS   4 /* FTEXT in tree ADDISKIN */ /* max len 5 */
#define DINFILES   5 /* FTEXT in tree ADDISKIN */ /* max len 5 */
#define DIUSED     6 /* FTEXT in tree ADDISKIN */ /* max len 10 */
#define DIAVAIL    7 /* FTEXT in tree ADDISKIN */ /* max len 10 */
#define DIOK       8 /* BUTTON in tree ADDISKIN */
#define DIQUIT     9 /* BUTTON in tree ADDISKIN */

#define ADDINFO    3 /* form/dialog */
#define DEOK       9 /* BUTTON in tree ADDINFO */

#define ADOPENAP   4 /* form/dialog */
#define APPLNAME   1 /* FTEXT in tree ADOPENAP */ /* max len 11 */
#define APPLPARM   3 /* FTEXT in tree ADOPENAP */ /* max len 38 */
#define APPLOK     4 /* BUTTON in tree ADOPENAP */
#define APPLCNCL   5 /* BUTTON in tree ADOPENAP */

#define ADINSDIS   5 /* form/dialog */
#define DRID       2 /* FTEXT in tree ADINSDIS */ /* max len 1 */
#define DRLABEL    3 /* FTEXT in tree ADINSDIS */ /* max len 12 */
#define IDRIVE     6 /* BUTTON in tree ADINSDIS */
#define ITRASH     7 /* BUTTON in tree ADINSDIS */
#define IPRINTER   8 /* BUTTON in tree ADINSDIS */
#define IBOX       9 /* BOX in tree ADINSDIS */
#define IICON     10 /* ICON in tree ADINSDIS */ /* max len 0 */
#define IUP       11 /* BOXCHAR in tree ADINSDIS */
#define IDOWN     13 /* BOXCHAR in tree ADINSDIS */
#define DROK      14 /* BUTTON in tree ADINSDIS */
#define DRCNCL    15 /* BUTTON in tree ADINSDIS */
#define DRQUIT    16 /* BUTTON in tree ADINSDIS */

#define ADMKDBOX   6 /* form/dialog */
#define MKNAME     1 /* FTEXT in tree ADMKDBOX */ /* max len 11 */
#define MKOK       2 /* BUTTON in tree ADMKDBOX */
#define MKCNCL     4 /* BUTTON in tree ADMKDBOX */

#define ADFORMAT   7 /* form/dialog */
#define FCBOX4     1 /* BOX in tree ADFORMAT */
#define FCBOXC     2 /* BOX in tree ADFORMAT */
#define SRCDRA     4 /* BOXCHAR in tree ADFORMAT */
#define SRCDRB     5 /* BOXCHAR in tree ADFORMAT */
#define DESTDR     6 /* BOXCHAR in tree ADFORMAT */
#define FCCOPY     7 /* BUTTON in tree ADFORMAT */
#define LINEBOX    8 /* BOX in tree ADFORMAT */
#define FCBOXF     9 /* BOX in tree ADFORMAT */
#define DRIVE     10 /* STRING in tree ADFORMAT */
#define ADRIVE    11 /* BOXCHAR in tree ADFORMAT */
#define BDRIVE    12 /* BOXCHAR in tree ADFORMAT */
#define FCLABEL   13 /* FTEXT in tree ADFORMAT */ /* max len 11 */
#define SIDEBOX   14 /* BOX in tree ADFORMAT */
#define FCDOUBLE  15 /* BUTTON in tree ADFORMAT */
#define FCSINGLE  16 /* BUTTON in tree ADFORMAT */
#define FCHIGH    17 /* BUTTON in tree ADFORMAT */
#define FCFORMAT  18 /* BUTTON in tree ADFORMAT */
#define FCBARA    20 /* BOX in tree ADFORMAT */
#define FCBARB    22 /* BOX in tree ADFORMAT */
#define FCOK      23 /* BUTTON in tree ADFORMAT */
#define FCCNCL    24 /* BUTTON in tree ADFORMAT */

#define ADICON     8 /* form/dialog */
#define ICON0      1 /* ICON in tree ADICON */ /* max len 12 */
#define ICON1      2 /* ICON in tree ADICON */ /* max len 12 */
#define ICON2      3 /* ICON in tree ADICON */ /* max len 12 */
#define ICON3      4 /* ICON in tree ADICON */ /* max len 12 */
#define ICON4      5 /* ICON in tree ADICON */ /* max len 12 */

#define PRINTFIL   9 /* form/dialog */
#define PFILE      1 /* FTEXT in tree PRINTFIL */ /* max len 11 */

#define BACKGROU  10 /* form/dialog */

#define SSCREEN   11 /* form/dialog */
#define STITLE     1 /* TEXT in tree SSCREEN */ /* max len 22 */
#define DESKPREF   4 /* BUTTON in tree SSCREEN */
#define WINPREF    5 /* BUTTON in tree SSCREEN */
#define PAT0       9 /* BOX in tree SSCREEN */
#define PAT1      10 /* BOX in tree SSCREEN */
#define PAT2      11 /* BOX in tree SSCREEN */
#define PAT3      12 /* BOX in tree SSCREEN */
#define PAT4      13 /* BOX in tree SSCREEN */
#define PAT5      14 /* BOX in tree SSCREEN */
#define PAT6      15 /* BOX in tree SSCREEN */
#define PAT7      16 /* BOX in tree SSCREEN */
#define OUTBOX    17 /* BOX in tree SSCREEN */
#define PATTERN   18 /* BOX in tree SSCREEN */
#define COLOR0    20 /* BOX in tree SSCREEN */
#define COLOR1    21 /* BOX in tree SSCREEN */
#define COLOR2    22 /* BOX in tree SSCREEN */
#define COLOR3    23 /* BOX in tree SSCREEN */
#define COLOR4    24 /* BOX in tree SSCREEN */
#define COLOR5    25 /* BOX in tree SSCREEN */
#define COLOR6    26 /* BOX in tree SSCREEN */
#define COLOR7    27 /* BOX in tree SSCREEN */
#define COLOR8    28 /* BOX in tree SSCREEN */
#define COLOR9    29 /* BOX in tree SSCREEN */
#define COLOR10   30 /* BOX in tree SSCREEN */
#define COLOR11   31 /* BOX in tree SSCREEN */
#define COLOR12   32 /* BOX in tree SSCREEN */
#define COLOR13   33 /* BOX in tree SSCREEN */
#define COLOR14   34 /* BOX in tree SSCREEN */
#define COLOR15   35 /* BOX in tree SSCREEN */
#define SOK       36 /* BUTTON in tree SSCREEN */
#define SCANCEL   37 /* BUTTON in tree SSCREEN */

#define FILEOPTI  12 /* form/dialog */
#define OPTNAME    2 /* FTEXT in tree FILEOPTI */ /* max len 11 */
#define OPTOK      3 /* BUTTON in tree FILEOPTI */

#define DLAUNCH   13 /* form/dialog */
#define RUNNAME    2 /* FTEXT in tree DLAUNCH */ /* max len 37 */
#define LTOS       4 /* BUTTON in tree DLAUNCH */
#define LGRAPHIC   5 /* BUTTON in tree DLAUNCH */
#define LAUNCHOK   7 /* BUTTON in tree DLAUNCH */
#define LNCNCL     8 /* BUTTON in tree DLAUNCH */

#define TEXTLINE  14 /* form/dialog */
#define TSTYLE     1 /* TEXT in tree TEXTLINE */ /* max len 48 */

#define SAMENAME  15 /* form/dialog */
#define SNAME      1 /* TEXT in tree SAMENAME */ /* max len 1 */
#define FNAME      2 /* FTEXT in tree SAMENAME */ /* max len 11 */
#define EDFNAME    3 /* FTEXT in tree SAMENAME */ /* max len 11 */
#define COPY       4 /* BUTTON in tree SAMENAME */
#define SKIP       5 /* BUTTON in tree SAMENAME */
#define QUIT       6 /* BUTTON in tree SAMENAME */

#define CPBOX     16 /* form/dialog */
#define TITLE      1 /* TEXT in tree CPBOX */ /* max len 1 */
#define NUMDIR     2 /* FTEXT in tree CPBOX */ /* max len 5 */
#define NUMFILE    3 /* FTEXT in tree CPBOX */ /* max len 5 */
#define CSDRIVE    4 /* FTEXT in tree CPBOX */ /* max len 1 */
#define CDDRIVE    5 /* FTEXT in tree CPBOX */ /* max len 1 */
#define HIDECBOX   6 /* BOX in tree CPBOX */
#define CPFILE     7 /* FTEXT in tree CPBOX */ /* max len 11 */
#define CPDIR      8 /* FTEXT in tree CPBOX */ /* max len 11 */
#define DATEBOX    9 /* BOX in tree CPBOX */
#define PYES      11 /* BUTTON in tree CPBOX */
#define CHBOX1    13 /* BOX in tree CPBOX */
#define OKCP      14 /* BUTTON in tree CPBOX */
#define CCANCEL   15 /* BUTTON in tree CPBOX */

#define INWICON   17 /* form/dialog */
#define IBOX1      2 /* BOX in tree INWICON */
#define WNAME      3 /* FTEXT in tree INWICON */ /* max len 11 */
#define WBOX       5 /* BOX in tree INWICON */
#define WICON      6 /* ICON in tree INWICON */ /* max len 0 */
#define WUP        7 /* BOXCHAR in tree INWICON */
#define WDOWN      8 /* BOXCHAR in tree INWICON */
#define WFOLDER   10 /* BUTTON in tree INWICON */
#define WNONE     11 /* BUTTON in tree INWICON */
#define WSKIP     12 /* BUTTON in tree INWICON */
#define WREMOVE   13 /* BUTTON in tree INWICON */
#define WOK       14 /* BUTTON in tree INWICON */
#define WQUIT     16 /* BUTTON in tree INWICON */

#define HELP1     18 /* form/dialog */
#define MOREHELP  14 /* BUTTON in tree HELP1 */

#define HELP2     19 /* form/dialog */

#define SEAFILE   20 /* form/dialog */
#define SFILE      2 /* FTEXT in tree SEAFILE */ /* max len 11 */
#define SEAOK      3 /* BUTTON in tree SEAFILE */
#define SEACNCL    4 /* BUTTON in tree SEAFILE */

#define MNINSAPP  21 /* form/dialog */
#define MPFNAME    2 /* FTEXT in tree MNINSAPP */ /* max len 11 */
#define MPARGS     3 /* FTEXT in tree MNINSAPP */ /* max len 11 */
#define MPDTYPE    4 /* FTEXT in tree MNINSAPP */ /* max len 3 */
#define MPITITLE   5 /* TEXT in tree MNINSAPP */ /* max len 15 */
#define MPIBUTTN   6 /* BUTTON in tree MNINSAPP */
#define MPBTITLE   7 /* TEXT in tree MNINSAPP */ /* max len 13 */
#define MPBBUTTN   8 /* BUTTON in tree MNINSAPP */
#define MPFTITLE   9 /* TEXT in tree MNINSAPP */ /* max len 16 */
#define MPFBUTTN  10 /* BUTTON in tree MNINSAPP */
#define MPDTITLE  11 /* TEXT in tree MNINSAPP */ /* max len 11 */
#define MPDBUTTN  12 /* BUTTON in tree MNINSAPP */
#define MPPTITLE  13 /* TEXT in tree MNINSAPP */ /* max len 11 */
#define MPPBUTTN  14 /* BUTTON in tree MNINSAPP */
#define MPOK      15 /* BUTTON in tree MNINSAPP */
#define MPREMOVE  16 /* BUTTON in tree MNINSAPP */
#define MPCANCEL  17 /* BUTTON in tree MNINSAPP */
#define MPQUIT    18 /* BUTTON in tree MNINSAPP */

#define MNSETPRE  22 /* form/dialog */
#define SPDTITLE   3 /* TEXT in tree MNSETPRE */ /* max len 17 */
#define SPDBUTTN   4 /* BUTTON in tree MNSETPRE */
#define SPCTITLE   5 /* TEXT in tree MNSETPRE */ /* max len 14 */
#define SPCBUTTN   6 /* BUTTON in tree MNSETPRE */
#define SPOTITLE   7 /* TEXT in tree MNSETPRE */ /* max len 15 */
#define SPOBUTTN   8 /* BUTTON in tree MNSETPRE */
#define MNSPOK     9 /* BUTTON in tree MNSETPRE */
#define MNSPCAN   10 /* BUTTON in tree MNSETPRE */

#define MNSETVID  23 /* form/dialog */
#define SVCTITLE   2 /* TEXT in tree MNSETVID */ /* max len 8 */
#define SVCBUTTN   3 /* BUTTON in tree MNSETVID */
#define SVLTITLE   4 /* TEXT in tree MNSETVID */ /* max len 9 */
#define SVLBUTTN   5 /* BUTTON in tree MNSETVID */
#define SVDTITLE   6 /* TEXT in tree MNSETVID */ /* max len 17 */
#define SVDBUTTN   7 /* BUTTON in tree MNSETVID */
#define SVMODES    8 /* BUTTON in tree MNSETVID */
#define IBASE      9 /* IBOX in tree MNSETVID */
#define SVOK      10 /* BUTTON in tree MNSETVID */
#define SVCANCEL  11 /* BUTTON in tree MNSETVID */

#define MNVIDEO   24 /* form/dialog */
#define SVSTLOW    3 /* BUTTON in tree MNVIDEO */
#define SVSTMED    4 /* BUTTON in tree MNVIDEO */
#define SVSTHIGH   5 /* BUTTON in tree MNVIDEO */
#define SVTTOK     6 /* BUTTON in tree MNVIDEO */
#define SVTTCAN    7 /* BUTTON in tree MNVIDEO */

#define MNMENU    25 /* form/dialog */
#define MPIBASE    1 /* BOX in tree MNMENU */
#define MPINONE    2 /* STRING in tree MNMENU */
#define MPIF1      3 /* STRING in tree MNMENU */
#define MPIF2      4 /* STRING in tree MNMENU */
#define MPIF3      5 /* STRING in tree MNMENU */
#define MPIF4      6 /* STRING in tree MNMENU */
#define MPIF5      7 /* STRING in tree MNMENU */
#define MPIF6      8 /* STRING in tree MNMENU */
#define MPIF7      9 /* STRING in tree MNMENU */
#define MPIF8     10 /* STRING in tree MNMENU */
#define MPIF9     11 /* STRING in tree MNMENU */
#define MPIF10    12 /* STRING in tree MNMENU */
#define MPFI11    13 /* STRING in tree MNMENU */
#define MPIF12    14 /* STRING in tree MNMENU */
#define MPIF13    15 /* STRING in tree MNMENU */
#define MPIF14    16 /* STRING in tree MNMENU */
#define MPIF15    17 /* STRING in tree MNMENU */
#define MPIF16    18 /* STRING in tree MNMENU */
#define MPIF17    19 /* STRING in tree MNMENU */
#define MPIF18    20 /* STRING in tree MNMENU */
#define MPIF19    21 /* STRING in tree MNMENU */
#define MPIF20    22 /* STRING in tree MNMENU */
#define MPFBASE   23 /* BOX in tree MNMENU */
#define MPFTOS    24 /* STRING in tree MNMENU */
#define MPFTTP    25 /* STRING in tree MNMENU */
#define MPFGEM    26 /* STRING in tree MNMENU */
#define MPFGMTTP  27 /* STRING in tree MNMENU */
#define SDBASE    28 /* BOX in tree MNMENU */
#define SDAPP     29 /* STRING in tree MNMENU */
#define SDTOP     30 /* STRING in tree MNMENU */
#define SIBASE    31 /* BOX in tree MNMENU */
#define SIFULL    32 /* STRING in tree MNMENU */
#define SIFILE    33 /* STRING in tree MNMENU */
#define MPBBASE   34 /* BOX in tree MNMENU */
#define MPBAUTO   35 /* STRING in tree MNMENU */
#define MPBNORML  36 /* STRING in tree MNMENU */
#define SPBASE    37 /* BOX in tree MNMENU */
#define SPYES     38 /* STRING in tree MNMENU */
#define SPNO      39 /* STRING in tree MNMENU */
#define VIDBASE   40 /* BOX in tree MNMENU */
#define VID2      41 /* STRING in tree MNMENU */
#define VID4      42 /* STRING in tree MNMENU */
#define VID16     43 /* STRING in tree MNMENU */
#define VID256    44 /* STRING in tree MNMENU */
#define VIDTRUE   45 /* STRING in tree MNMENU */
#define SVIBASE   46 /* BOX in tree MNMENU */
#define SVION     47 /* STRING in tree MNMENU */
#define SVIOFF    48 /* STRING in tree MNMENU */
#define SVLBASE   49 /* BOX in tree MNMENU */
#define SVL40     50 /* STRING in tree MNMENU */
#define SVL80     51 /* STRING in tree MNMENU */

#define MNSYSTEM  26 /* form/dialog */
#define SDTITLE    2 /* TEXT in tree MNSYSTEM */ /* max len 22 */
#define SDBUTTON   3 /* BUTTON in tree MNSYSTEM */
#define SITITLE    4 /* TEXT in tree MNSYSTEM */ /* max len 20 */
#define SIBUTTON   5 /* BUTTON in tree MNSYSTEM */
#define MFBASE     7 /* BOX in tree MNSYSTEM */
#define MFLEFT     8 /* BOXCHAR in tree MNSYSTEM */
#define MFFILE     9 /* FTEXT in tree MNSYSTEM */ /* max len 17 */
#define MFRIGHT   10 /* BOXCHAR in tree MNSYSTEM */
#define MFKEY     11 /* FTEXT in tree MNSYSTEM */ /* max len 2 */
#define MFUP      12 /* BOXCHAR in tree MNSYSTEM */
#define MFDOWN    13 /* BOXCHAR in tree MNSYSTEM */
#define MKITEM    17 /* BOXTEXT in tree MNSYSTEM */ /* max len 30 */
#define MKUPS     18 /* BOXCHAR in tree MNSYSTEM */
#define MKDOWNS   19 /* BOXCHAR in tree MNSYSTEM */
#define MKKEYS    20 /* FTEXT in tree MNSYSTEM */ /* max len 1 */
#define MKCLEAR   21 /* BUTTON in tree MNSYSTEM */
#define MKMEMORY  22 /* FTEXT in tree MNSYSTEM */ /* max len 10 */
#define MOK       23 /* BUTTON in tree MNSYSTEM */
#define MCANCEL   24 /* BUTTON in tree MNSYSTEM */

#define MORE       0 /* Free string */
/* -Continua- */

#define ENDFILE    1 /* Free string */
/* -Fine del file- */

#define CART       2 /* Free string */
/* Cartridge */

#define CPYSTR     3 /* Free string */
/* COPIA FILE */

#define DELSTR     4 /* Free string */
/* CANCELLA FILE */

#define FILEINFO   5 /* Free string */
/* INFORMAZIONI FILE */

#define FOLDINFO   6 /* Free string */
/* INFORMAZIONI CARTELLA */

#define MOVESTR    7 /* Free string */
/* SPOSTA FILE */

#define READERRO   8 /* Free string */
/* -Errore in lettura- */

#define CANTOPEN   9 /* Free string */
/* Impossibile aprire  */

#define NOMEM     10 /* Free string */
/* Memoria insufficiente */

#define CACHETXT  11 /* Free string */
/*   Cache                       [ ] */

#define BLTTXT    12 /* Free string */
/*   Blitter                     [ ] */

#define NCONFLIC  13 /* Free string */
/* NOMI IN CONFLITTO! */

#define RNAME     14 /* Free string */
/* RINOMINA DESTINAZIONE */

#define CRENSTR   15 /* Free string */
/* COPIA E RINOMINA OGGETTI */

#define MRENSTR   16 /* Free string */
/* SPOSTA E RINOMINA OGGETTI */

#define ISTR      17 /* Free string */
/*  %L byte usati in %W oggetto. */

#define ISELS     18 /* Free string */
/*  %L byte usati in %W oggetti selezionati. */

#define ISEL      19 /* Free string */
/*  %L byte usati in %W oggetto selezionato. */

#define ISTRS     20 /* Free string */
/*  %L byte usati in %W oggetti. */

#define DEVICE    21 /* Free string */
/* DISPOSITIVO */

#define SINF      22 /* Free string */
/* Seleziona un file INF */

#define LITEM     23 /* Free string */
/* Seleziona Oggetto */

#define FSTOS     24 /* Free string */
/* TOS */

#define FSTOSTTP  25 /* Free string */
/* TOS con parametri */

#define FSGEM     26 /* Free string */
/* GEM */

#define FSGEMTTP  27 /* Free string */
/* GEM con parametri */

#define FSAPP     28 /* Free string */
/* Programma */

#define FSTOP     29 /* Free string */
/* Fin.attiva */

#define FSFULL    30 /* Free string */
/* Percorso */

#define FSFILE    31 /* Free string */
/* Nome del file */

#define FS40      32 /* Free string */
/* 40 */

#define FS80      33 /* Free string */
/* 80 */

#define FSYES     34 /* Free string */
/* Si */

#define FSNO      35 /* Free string */
/* No */

#define FS2C      36 /* Free string */
/* 2 Colori */

#define FS4C      37 /* Free string */
/* 4 Colori */

#define FS16C     38 /* Free string */
/* 16 Colori */

#define FS256C    39 /* Free string */
/* 256 Colori */

#define FSTRUEC   40 /* Free string */
/* True Color */

#define FSAUTO    41 /* Free string */
/* Auto */

#define FSNORMAL  42 /* Free string */
/* Normale */

#define FSON      43 /* Free string */
/* Si */

#define FSOFF     44 /* Free string */
/* No */

#define FSNONE    45 /* Free string */
/* Nessuno */

#define FSF1      46 /* Free string */
/* F1 */

#define FSF2      47 /* Free string */
/* F2 */

#define FSF3      48 /* Free string */
/* F3 */

#define FSF4      49 /* Free string */
/* F4 */

#define FSF5      50 /* Free string */
/* F5 */

#define FSF6      51 /* Free string */
/* F6 */

#define FSF7      52 /* Free string */
/* F7 */

#define FSF8      53 /* Free string */
/* F8 */

#define FSF9      54 /* Free string */
/* F9 */

#define FSF10     55 /* Free string */
/* F10 */

#define FSF11     56 /* Free string */
/* F11 */

#define FSF12     57 /* Free string */
/* F12 */

#define FSF13     58 /* Free string */
/* F13 */

#define FSF14     59 /* Free string */
/* F14 */

#define FSF15     60 /* Free string */
/* F15 */

#define FSF16     61 /* Free string */
/* F16 */

#define FSF17     62 /* Free string */
/* F17 */

#define FSF18     63 /* Free string */
/* F18 */

#define FSF19     64 /* Free string */
/* F19 */

#define FSF20     65 /* Free string */
/* F20 */

#define DLTEXT    66 /* Free string */
/*    Linea doppia:  */

#define ITEXT     67 /* Free string */
/* Interallacciato:  */

#define STNOAPPL  68 /* Alert string */
/* [1][Puoi solo visualizzare o|stampare questo documento.][Visualizza|Stampa|Annulla] */

#define STNOWIND  69 /* Alert string */
/* [1][Il piano di lavoro del GEM non|ha pió finestre libere.|Chiudi una finestra che non|stai utilizzando.][  OK  ] */

#define STFOEXIS  70 /* Alert string */
/* [2][Non ä possibile creare una|cartella con questo nome.|Cambia il nome, oppure|annulla l'operazione.][ Riprova |Annulla] */

#define STDISKFU  71 /* Alert string */
/* [1][ Il drive %S: non ha | spazio a sufficienza | per questa operazione.][  OK  ] */

#define STFO8DEE  72 /* Alert string */
/* [1][Non ä possibile creare o|accedere ad una cartella|con un percorso cosç lungo.][  OK  ] */

#define STBADCOP  73 /* Alert string */
/* [1][ Operazione non permessa!][  OK  ] */

#define TRSINFO   74 /* Alert string */
/* [1][%S cancella|in modo permanente i |file o le cartelle.][  OK  ] */

#define FCNOMEM   75 /* Alert string */
/* [3][Memoria insufficiente!][  OK  ] */

#define FCFAIL    76 /* Alert string */
/* [3][Errore durante la copia o la|formattazione. Il disco di|destinazione ä protetto dalla|scrittura o inutilizzabile.][ Riprova |Annulla] */

#define FCSIZE    77 /* Alert string */
/* [1][Questo disco ha %L|byte disponibili per l'utente.][ Continua | Esci ] */

#define FCNOTYPE  78 /* Alert string */
/* [3][Il disco di destinazione|non ä dello stesso tipo |del disco di origine.|Inserisci un altro disco.][ Riprova |Annulla] */

#define SAVETOP   79 /* Alert string */
/* [1][Vuoi salvare il|piano di lavoro?][  OK  | Annulla ] */

#define PRINTTOP  80 /* Alert string */
/* [1][Vuoi stampare lo schermo?][  OK  | Annulla ] */

#define ABORTCON  81 /* Alert string */
/* [1][Annulli questa operazione?][  Si  |No] */

#define NOICON    82 /* Alert string */
/* [1][Il piano di lavoro non puï|installare altre icone.][  OK  ] */

#define NOAPP     83 /* Alert string */
/* [1][Il piano di lavoro non puï|installare altri programmi.][  OK  ] */

#define NODRIVE   84 /* Alert string */
/* [1][Il drive %S: non esiste.][ Annulla ] */

#define NOOUTPUT  85 /* Alert string */
/* [1][Il dispositivo di emissione|non sta ricevendo i dati!][ Riprova | Annulla ] */

#define CNTOPEN   86 /* Alert string */
/* [1][Non ä possibile aprire il file|%S][Salta| Riprova |Annulla] */

#define CNTCRTDR  87 /* Alert string */
/* [1][Non ä possibile creare la|directory %S][Salta| Riprova |Annulla] */

#define WRERROR   88 /* Alert string */
/* [1][Errore durante la |copia di un file.][ Salta | Annulla ] */

#define RDERROR   89 /* Alert string */
/* [1][Errore durante la lettura|del file resource.][ Salta | Annulla ] */

#define CNTCRTFL  90 /* Alert string */
/* [1][Non ä possibile creare il file|%S][Salta| Riprova |Annulla] */

#define CNTDELF   91 /* Alert string */
/* [1][Non ä possibile cancellare il|file %S][Salta| Riprova |Annulla] */

#define CNTDELD   92 /* Alert string */
/* [1][Non ä possibile creare la|directory %S][Salta| Riprova |Annulla] */

#define PRINTINF  93 /* Alert string */
/* [1][%S|ä un dispositivo di stampa.][  OK  ] */

#define RENAME    94 /* Alert string */
/* [1][Cambio del nome errato!][ Riprova |Annulla] */

#define NOBUF     95 /* Alert string */
/* [1][Nome Percorso Troppo Lungo!][  OK  ] */

#define DELDISK   96 /* Alert string */
/* [2][Vuoi cancellare tutti|i file sul drive %S?][ Si | No ] */

#define NOTPRG    97 /* Alert string */
/* [1][%S|non ä un programma.][  OK  ] */

#define NOSETDIR  98 /* Alert string */
/* [1][Non ä possibile impostare|una directory di default.][  OK  ] */

#define NOAUTO    99 /* Alert string */
/* [1][Non ä possibile salvare|il file in modo AUTO.][  OK  ] */

#define NOWINMEM 100 /* Alert string */
/* [1][Non ä possibile allocare|memoria per la finestra.][  OK  ] */

#define NOTYPE   101 /* Alert string */
/* [1][Tipo di file non valido.][  OK  ] */

#define ICONTYPE 102 /* Alert string */
/* [1][Seleziona il tipo di icona.][Piano| Finestra |Annulla] */

#define NODIRCOP 103 /* Alert string */
/* [1][Operazione non permessa.][ Salta | Annulla ] */

#define NOINSTAL 104 /* Alert string */
/* [1][Oggetto non installabile.][ OK ] */

#define NOMORE   105 /* Alert string */
/* [1][Non ci sono altri file.][  OK  ] */

#define NOID     106 /* Alert string */
/* [1][ID drive non specificato.][ Riprova ] */

#define ILLMAT   107 /* Alert string */
/* [1][Formato del file .INF errato.][ OK ] */

#define NOCART   108 /* Alert string */
/* [1][Cartridge non presente.][ OK ] */

#define NOMATCH  109 /* Alert string */
/* [1][Tipo di file non|corrispondente.][ OK ] */

#define DELITEM  110 /* Alert string */
/* [1][Vuoi rimuovere le icone |o cancellare i file? ][Rimuovi| Cancella |Annulla] */

#define MCFAIL   111 /* Alert string */
/* [1][Non ä possibile recuperare|memoria.][ OK ] */

#define FNOTFIND 112 /* Alert string */
/* [1][%S non trovato.][ OK ] */

#define SNEXT    113 /* Alert string */
/* [1][Continuo la ricerca? ][OK| Annulla ] */

#define NOHELP   114 /* Alert string */
/* [1][Menu di aiuto incompatibile|con questa risoluzione.][ OK ] */

#define NOSEA    115 /* Alert string */
/* [1][Cartridge non accesibile.][ OK ] */

#define KEYUSED  116 /* Alert string */
/* [1][Questo tasto ä giÖ definito.|Vuoi sovrascriverlo?][OK| Annulla ] */

#define CARTNOSH 117 /* Alert string */
/* [1][Non ä possibile dare|informazioni sulla|cartridge.][ OK ] */

#define BADKEY   118 /* Alert string */
/* [1][Definizione errata|del tasto funzione.][ OK ] */

#define XNFILE   119 /* Alert string */
/* [1][%S non trovato!|Vuoi rimuovere l'icona|o selezionare l'oggetto?][Rimuovi|Seleziona|Annulla] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD deskita_rsc_load(void);
extern _WORD deskita_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD deskita_rsc_free(void);
#endif
