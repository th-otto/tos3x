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