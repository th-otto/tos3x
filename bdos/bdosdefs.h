/* common supervisor stack size (in words) */
#define SUPSIZ 1024
/* max open files in system */
#define OPNFILES 81
/* max current directories in use in system */
#define NCURDIR 40
/* number of block devices supported: A: ... Z: */
#define BLKDEVNUM 16
/* size of typeahead buffer */
#define KBBUFSZ 80
/* for compatibility with older versions: size of root array */
#define MAXQUICK 5

/*
 * set to 1 if we are on a 68010+ with 8-byte trap exception frames.
 * This will produce slightly better code in the gemdos dispatcher,
 * but won't run on 68000.
 * If 0, _longframe will be checked at runtime, but the BIOS
 * might still have other assumptions.
 */
#ifndef P68010
#define P68010  (TOSVERSION >= 0x300)
#endif

/*
 * set to 1 if we are on a 68030+ (else 0)
 * Used when invalidating caches might be neccessary
 */
#ifndef P68030
#define P68030  (TOSVERSION >= 0x300)
#endif
