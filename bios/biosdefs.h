#define NUMFLOPPIES 2

/* standard for floppy, hard disk */
#define SECTOR_SIZE     512

#define SERIAL_BUFSIZE 256

/* number of sectors for media change detection */
#define BPBSECT 6

/* media def has not changed */
#define MEDIANOCHANGE    0
/* media may have changed */
#define MEDIAMAYCHANGE   1
/* media def has changed */
#define MEDIACHANGE      2

#define NCOOKIES 16

/*
 * set to 1 if we are on a 68010+ with 8-byte trap exception frames
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

#define NVRAM_SUPPORT (TOSVERSION >= 0x300)

#define STBOOK ((TOSVERSION >= 0x207) & (TOSVERSION <= 0x208))

#define MSTECLK ((TOSVERSION >= 0x200) & (TOSVERSION < 0x300))

#define TTMFP_SUPPORT ((TOSVERSION >= 0x300) & (TOSVERSION < 0x400))

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
