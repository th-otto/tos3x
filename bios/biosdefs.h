/* common supervisor stack size (in words) (also defined in bdosdefs.h) */
#ifndef SUPSIZ
#define SUPSIZ 1024
#endif

#define NUMFLOPPIES 2

/* standard for floppy, hard disk */
#define SECTOR_SIZE     512

#define SERIAL_BUFSIZE 256

/* number of sectors for media change detection */
#if TOSVERSION >= 0x200
#define BPBSECT 6
#else
#define BPBSECT 1
#endif

/* media def has not changed */
#define MEDIANOCHANGE    0
/* media may have changed */
#define MEDIAMAYCHANGE   1
/* media def has changed */
#define MEDIACHANGE      2

#if TOSVERSION >= 0x400
#define NCOOKIES 32
#else
#define NCOOKIES 16
#endif

#define NVRAM_SUPPORT ((TOSVERSION >= 0x300) & !TP_50)

#define STBOOK ((TOSVERSION >= 0x207) & (TOSVERSION <= 0x208))

#define MSTECLK (((TOSVERSION >= 0x104) & (TOSVERSION < 0x300)) | TP_50)

/*
 * True if TT-MFP is assumed to be present;
 * otherwise checked at runtime
 */
#define TTMFP_ASSUMED (((TOSVERSION >= 0x300) & (TOSVERSION < 0x400)) & !TP_50)

/*
 * True to add support for TT-MFP
 */
#define TTMFP_SUPPORT (TTMFP_ASSUMED | (TP_50 & TP_55)) /* (TTMFP_ASSUMED | (PAK & HasTTMfp)) */

#define SCC_SUPPORT ((TOSVERSION >= 0x300) & (TOSVERSION < 0x400))

/*
 * used to mark some strange looking code segments
 * that are needed to create a binary equivalent image
 * of the TOS ROMs
 */
#ifndef BINEXACT
#define BINEXACT 1
#endif


/*
 * number of extra entries in bconmap table
 */
#if TTMFP_SUPPORT
#define rs232devs 4
#else
#define rs232devs 3
#endif

#define FASTRAMBASE $01000000

#define HD_SUPPORT (TOSVERSION >= 0x200)
