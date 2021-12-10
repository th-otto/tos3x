/* strcz.h */

#ifdef NEED_STRINGS

/* 206cz: e367c2-e36a02 (576 bytes) */

#if 0
/*
 * using seperate variables does not produce identical results,
 * so we have to use (another) hack
 */
char const More[] = "-D\240le-";
char const Endfile[] = "-Konec souboru-";
char const Cart[] = "ROM modul";
char const Cpystr[] = "Kop\241rovat soubor(y)";
char const Delstr[] = "Mazat soubor(y)";
char const Fileinfo[] = "Informace o souboru";
char const Foldinfo[] = "Informace o po\251ada\207i";
char const Movestr[] = "P\251en\202st soubor(y)";
char const Readerror[] = "-Chyba \207ten\241-";
char const Cantopen[] = "Nelze otev\251\241t";
char const Nomem[] = "Nen\241 voln\240 pam\210\237";
char const dempty1[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
char const Cachetxt[] = "  Cache                   [ ]";
char const Blttxt[] =   "  Blitter                 [ ]";
char const Nconflict[] = "Jm\202no ji\221 existuje!";
char const Rname[] = "P\251ejmenovat c\241lov\230 objekt";
char const Crenstr[] = "Kop\241rovat a p\251ejmenovat objekt(y)";
char const Mrenstr[] = "P\251en\202st a p\251ejmenovat objekt(y)";
char const Istr[] = " %L bajt\226 v %W objektu.";
char const Isels[] = " %L bajt\226 v %W vybran\230ch objektech.";
char const Isel[] = " %L bajt\226 v %W vybran\202m objektu.";
char const Istrs[] = " %L bajt\226 v %W objektech.";
char const Device[] = "Za\251\241zen\241";
char const Sinf[] = "Vyber .INF soubor";
char const Litem[] = "Hledej objekt";
char const dempty2[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

#else

char const czstr[] = "\
-D\240le-\0\
-Konec souboru-\0\
ROM modul\0\
Kop\241rovat soubor(y)\0\
Mazat soubor(y)\0\
Informace o souboru\0\
Informace o po\251ada\207i\0\
P\251en\202st soubor(y)\0\
-Chyba \207ten\241-\0\
Nelze otev\251\241t\0\
Nen\241 voln\240 pam\210\237\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
  Cache                   [ ]\0\
  Blitter                 [ ]\0\
Jm\202no ji\221 existuje!\0\
P\251ejmenovat c\241lov\230 objekt\0\
Kop\241rovat a p\251ejmenovat objekt(y)\0\
P\251en\202st a p\251ejmenovat objekt(y)\0\
 %L bajt\226 v %W objektu.\0\
 %L bajt\226 v %W vybran\230ch objektech.\0\
 %L bajt\226 v %W vybran\202m objektu.\0\
 %L bajt\226 v %W objektech.\0\
Za\251\241zen\241\0\
Vyber .INF soubor\0\
Hledej objekt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\
";

#endif

#else

extern char const czstr[];

#define More czstr+0
#define Endfile czstr+7
#define Cart czstr+23
#define Cpystr czstr+33
#define Delstr czstr+53
#define Fileinfo czstr+69
#define Foldinfo czstr+89
#define Movestr czstr+110
#define Readerror czstr+128
#define Cantopen czstr+142
#define Nomem czstr+156
#define Cachetxt czstr+208
#define Blttxt czstr+238
#define Nconflict czstr+268
#define Rname czstr+288
#define Crenstr czstr+314
#define Mrenstr czstr+348
#define Istr czstr+380
#define Isels czstr+404
#define Isel czstr+440
#define Istrs czstr+473
#define Device czstr+499
#define Sinf czstr+508
#define Litem czstr+526

#endif
