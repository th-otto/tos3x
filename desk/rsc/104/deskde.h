/*
 * resource set indices for deskde
 *
 * created by ORCS 2.16
 */

/*
 * Number of Strings:        237
 * Number of Bitblks:        1
 * Number of Iconblks:       5
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       30
 * Number of Free Strings:   37
 * Number of Free Images:    0
 * Number of Objects:        177
 * Number of Trees:          14
 * Number of Userblks:       0
 * Number of Images:         11
 * Total file size:          11350
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "deskde"
#endif
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
#define NUM_STRINGS 237
#define NUM_FRSTR 37
#define NUM_UD 0
#define NUM_IMAGES 11
#define NUM_BB 1
#define NUM_FRIMG 0
#define NUM_IB 5
#define NUM_CIB 0
#define NUM_TI 30
#define NUM_OBS 177
#define NUM_TREE 14
#endif



#define MENU1              0 /* menu */
#define DESKMENU           3 /* TITLE in tree MENU1 */
#define FILEMENU           4 /* TITLE in tree MENU1 */
#define VIEWMENU           5 /* TITLE in tree MENU1 */
#define OPTNMENU           6 /* TITLE in tree MENU1 */
#define ABOUITEM           9 /* STRING in tree MENU1 */
#define OPENITEM          18 /* STRING in tree MENU1 */
#define SHOWITEM          19 /* STRING in tree MENU1 */
#define NEWFITEM          21 /* STRING in tree MENU1 */
#define CLSFITEM          22 /* STRING in tree MENU1 */
#define CLSWITEM          23 /* STRING in tree MENU1 */
#define FORMITEM          25 /* STRING in tree MENU1 */
#define ICONITEM          27 /* STRING in tree MENU1 */
#define TEXTITEM          28 /* STRING in tree MENU1 */
#define NAMEITEM          30 /* STRING in tree MENU1 */
#define DATEITEM          31 /* STRING in tree MENU1 */
#define SIZEITEM          32 /* STRING in tree MENU1 */
#define TYPEITEM          33 /* STRING in tree MENU1 */
#define IDSKITEM          35 /* STRING in tree MENU1 */
#define IAPPITEM          36 /* STRING in tree MENU1 */
#define PREFITEM          38 /* STRING in tree MENU1 */
#define SAVEITEM          39 /* STRING in tree MENU1 */
#define PRINTITEM         40 /* STRING in tree MENU1 */
#define BITBLT            42 /* STRING in tree MENU1 */

#define ADFILEIN           1 /* form/dialog */

#define ADDISKIN           2 /* form/dialog */

#define ADDINFO            3 /* form/dialog */

#define ADOPENAP           4 /* form/dialog */

#define ADINSDIS           5 /* form/dialog */

#define ADDINSAPP          6 /* form/dialog */

#define CPBOX              7 /* form/dialog */

#define SAMENAME           8 /* form/dialog */

#define ADMKDBOX           9 /* form/dialog */

#define ADSETPREF         10 /* form/dialog */

#define ADFORMAT          11 /* form/dialog */

#define ADICON            12 /* free form */

#define PRINTFIL          13 /* free form */

#define STRNG1             0 /* Free string */
/*  %L Bytes in %W Dateien. */

#define STRNG2             1 /* Free string */
/* -MEHR- */

#define STRNG3             2 /* Free string */
/* -ENDE DER DATEI- */

#define STRNG4             3 /* Free string */
/* ROM MODUL */

#define STRNG5             4 /* Free string */
/* ORDNER/DATEIEN KOPIEREN */

#define STRNG6             5 /* Free string */
/* ORDNER/DATEIEN LôSCHEN */

#define STRNG7             6 /* Free string */
/* DATEI INFORMATION */

#define STRNG8             7 /* Free string */
/* ORDNER INFORMATION */

#define STRNG9             8 /* Free string */
/* ORDNER/DATEIEN VERSCHIEBEN */

#define STRNG10            9 /* Free string */
/* -LESEFEHLER- */

#define STRNG11           10 /* Free string */
/* KANN NICHT GEôFFNET WERDEN */

#define STRNG12           11 /* Free string */
/* NICHT GENöGEND SPEICHERPLATZ */

#define STNOAPPL          12 /* Alert string */
/* [1][Diese Datei kann nur gedruckt|oder angezeigt werden.][ANZEIGEN|DRUCKEN|ABBRUCH] */

#define STNOWIND          13 /* Alert string */
/* [1][Keine weiteren Fenster|vorhanden! Abhilfe durch|Schlieûen eines Fensters.][  OK  ] */

#define STNOOPEN          14 /* Alert string */
/* [1][Der Papierkorb lÑût sich|nicht îffnen.][  OK  ] */

#define STFORMAT          15 /* Alert string */
/* [3][Formatieren lîscht alle|Daten auf Laufwerk %S:.|Klicken Sie OK um zu|bestÑtigen.][OK|ABBRUCH] */

#define STNODRAG          16 /* Alert string */
/* [1][Dieses Bild darf nicht auf|ein anderes gelegt werden.][  OK  ] */

#define STNOBIN           17 /* Alert string */
/* [1][LAUFWERK NICHT IN DEN|PAPIERKORB!  Lîschen mit|'formatiere...' DATEI MenÅ.][  OK  ] */

#define STCOPY            18 /* Alert string */
/* [3][Kopieren von|Laufwerk %S: auf Laufwerk %S:|lîscht alle Daten auf|Laufwerk %S:. |BestÑtigen mit OK.][  OK  |ABBRUCH] */

#define STNOWIN           19 /* Alert string */
/* [1][Objekte aus Fenstern kînnen|nicht auf dem DESKTOP|abgelegt werden!][  OK  ] */

#define STNOBIN2          20 /* Alert string */
/* [1][Der Papierkorb kann nicht in|ein Fenster gelegt werden!][  OK  ] */

#define STFOEXISTS        21 /* Alert string */
/* [2][UngÅltiger Ordnername! Bitte|den Namen Ñndern oder Anlegen|abbrechen.][ NOCHMAL |ABBRUCH] */

#define STDISKFULL        22 /* Alert string */
/* [1][Dieses Laufwerk hat nicht|genÅgend Speicherplatz.][  OK  ] */

#define STFOF8DEE         23 /* Alert string */
/* [3][So tief im Indexpfad kînnen|keine Ordner angelegt oder|angesprochen werden.][  OK  ] */

#define STNOROM           24 /* Alert string */
/* [1][Dateien oder Ordner kînnen|nicht in das ROM-Modul oder|aus diesem kopiert werden.][  OK  ] */

#define STILLCOPY         25 /* Alert string */
/* [1][UngÅltige Kopieranweisung!][  OK  ] */

#define TRSINFO           26 /* Alert string */
/* [1][Werfen Sie nur die Objekte in|den Papierkorb, die Sie fÅr|IMMER lîschen wollen.][  OK  ] */

#define FCNOMEM           27 /* Alert string */
/* [3][Nicht mehr genÅgend Speicher!][  OK  ] */

#define FCFAIL            28 /* Alert string */
/* [3][Fehler beim Formatieren oder|Kopieren!|Eventuell Zieldiskette defekt.][ NOCHMAL |ABBRUCH] */

#define FCSIZE            29 /* Alert string */
/* [1][Diese Diskette hat|%L Bytes freien|Speicherplatz.][  OK  ] */

#define FCNOTYPE          30 /* Alert string */
/* [3][Disketten haben nicht das|gleiche Format! Bitte eine|korrekt formatierte Diskette|in das Ziellaufwerk einlegen.][ NOCHMAL |ABBRUCH] */

#define SAVETOP           31 /* Alert string */
/* [1][ |Arbeit sichern?][  OK  |ABBRUCH] */

#define PRINTTOP          32 /* Alert string */
/* [1][ |HARDCOPY?][  OK  |ABBRUCH] */

#define FCDISKONLY        33 /* Alert string */
/* [3][Funktion nur mit Disketten|mîglich!][  OK  ] */

#define ABORTCON          34 /* Alert string */
/* [1][Vorgang abbrechen?][ JA | NEIN ] */

#define NOICON            35 /* Alert string */
/* [1][Es kînnen keine weiteren|Bilder auf dem DESKTOP|installiert werden.][  OK  ] */

#define NOAPP             36 /* Alert string */
/* [1][Es kînnen keine weiteren|Anwendungen installiert|werden.][  OK  ] */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD deskde_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD deskde_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD deskde_rsc_free(void);
#endif
