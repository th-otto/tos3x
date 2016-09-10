#ifndef FS_H
#define FS_H

#include "pd.h"

/*
 *  Type declarations
 */

#define FTAB    struct _ftab
#define OFD     struct _ofd
#define FCB     struct _fcb
#define DND     struct _dnd
#define FH      int16_t    /*  file handle    */
#define DMD     struct _dmd
#define BCB struct _bcb


#define SLASH '\\'

#define SUPSIZ 1024     /* common supervisor stack size (in words) */
#define OPNFILES 81     /* max open files in system */
#define NCURDIR 40      /* max current directories in use in system */
#define KBBUFSZ 128     /* size of typeahead buffer -- must be power of 2!! */
#define KBBUFMASK       (KBBUFSZ-1)

/*
 *  code macros
 */

#define MGET(x) ((x *) xmgetblk((sizeof(x) + 15)>>4))
#define rwabs(a,b,c,d,e) if((rwerr=Rwabs(a,b,c,d,e))!=0){errdrv=e;xlongjmp(errbuf,rwerr);}


#define FA_NORM (FA_ARCH|FA_HIDDEN|FA_RDONLY|FA_SYSTEM)


/*
 *  OFD - open file descriptor
 *
 *  architectural restriction: for compatibility with FOLDRnnn.PRG,
 *  this structure must not exceed 64 bytes in length
 */

OFD
{
	/*  0 */ OFD   *o_link;		/*	link to next OFD					*/
	/*  4 */ uint16_t o_flag;
					/* the following 3 items must be as in FCB: */
	/*  6 */ DOSTIME o_td;		/*	creation time/date: little-endian!	*/
	/* 10 */ CLNO  o_strtcl; 	/*	starting cluster number 			*/
	/* 12 */ int32_t  o_fileln;	/*  length of file in bytes			    */

	/* 16 */ DMD   *o_dmd;		/*	link to media descr 				*/
	/* 20 */ DND   *o_dnode; 	/*	link to dir for this file			*/
	/* 24 */ OFD   *o_dirfil;	/*	OFD for dir for this file			*/
	/* 28 */ int32_t  o_dirbyt;	/*  pos in dir of this files fcb (dcnt) */

	/* 32 */ int32_t  o_bytnum;	/* byte pointer within file			    */
	/* 36 */ CLNO  o_curcl;		/* current cluster number for file		*/
	/* 38 */ RECNO o_currec; 	/* current record number for file		*/
	/* 40 */ uint16_t o_curbyt;	/* byte pointer within current cluster  */
	/* 42 */ int16_t  o_usecnt;	/* use count for inherited files 	    */
	/* 44 */ OFD   *o_thread;	/* mulitple open thread list			*/
	/* 48 */ uint16_t o_mod; 	/* mode file opened in (see below)	    */
	/* 50 */ 
};


/*
 * bit usage in o_mod
 *
 * bits 8-15 are internal-use only
 * bits 0-7 correspond to bit usage in the Fopen() system call
 * note: bits 4-7 are only used if GEMDOS file-sharing/record-locking
 *       is implemented
 */
#define INH_MODE    0x80    /* bit 7 is inheritance flag (not yet implemented) */
#define MODE_FSM    0x70    /* bits 4-6 are file sharing mode (not yet implemented) */
#define MODE_FAC    0x03    /* bits 0-1 are file access code: (same as O_ACCMODE) */
#define RO_MODE        0	/* Open read-only - same as O_RDONLY */
#define WO_MODE        1	/* Open write-only - same as O_WRONLY */
#define RW_MODE        2	/* Open read/write - same as O_RDWR */
#define VALID_FOPEN_BITS    MODE_FAC    /* currently-valid bits for Fopen() */

/*
 * O_DIRTY - Dirty Flag
 *
 * T: OFD is dirty, because of chg to startcl, length, time, etc.
 */
#define O_DIRTY         1



/*
 *  FCB - File Control Block
 *
 *  architectural restriction: this is the structure of the
 *  directory entry on disk, compatible with MSDOS etc
 */

FCB
{
    char f_name[11];
    char f_attrib;
    char f_fill[10];
    DOSTIME f_td;           /* time, date */
    CLNO f_clust;
    int32_t f_fileln;
};


/*
 *  DND - Directory Node Descriptor
 *
 *  architectural restriction: for compatibility with FOLDRnnn.PRG,
 *  this structure must not exceed 64 bytes in length
 */

DND /* directory node descriptor */
{
	/*  0 */ char d_name[11];	/*	directory name						*/
	/* 11 */ char d_fill;		/*	attributes? 						*/
	/* 12 */ uint16_t d_flag;
	/* 14 */ CLNO d_strtcl;		/*	starting cluster number of dir		*/

	/* 16 */ DOSTIME d_td;		/*	time/date: little-endian!			*/
	/* 20 */ OFD  *d_ofd;		/*	open file descr for this dir		*/
	/* 24 */ DND  *d_parent; 	/*	parent dir (..) 					*/
	/* 28 */ DND  *d_left;		/*	1st child							*/

	/* 32 */ DND  *d_right;		/*	sibling in same dir 				*/
	/* 36 */ DMD  *d_drv;		/*	for drive							*/
	/* 40 */ OFD  *d_dirfil;
	/* 44 */ int32_t d_dirpos;

	/* 48 */ int32_t d_scan; 	/*	current posn in dir for DND tree	*/
	/* 52 */ OFD  *d_files;		/* open files on this node				*/
	/* 56 */ int16_t d_usecount; /*	Fsfirst/Fsnext's in progress		*/
	/* 58 */ 
};

/*
 * bit usage in d_flag
 */
#define	B_16	1				/* device has 16-bit FATs	*/
#define	B_FIX	2				/* device has fixed media	*/
#define DND_LOCKED  0x8000  /* DND may not be scavenged (see     */
                            /* free_available_dnds() in fsdir.c) */


/*
 * FTAB - Open File Table Entry
 */

/* point these at OFDs when needed */
FTAB
{
    OFD *f_ofd;
    PD  *f_own;         /* file owners */
    int16_t f_use;      /* use count */
};


/******************************************
**
** BDOS level character device file handles
**
*******************************************
*/

#define	H_Null		(-1)		/* not passed through to BIOS	*/
#define	H_Print		(-2)
#define	H_Aux		(-3)
#define	H_Console	(-4)
#define	H_Clock		(-5)
#define	H_Mouse		(-6)


/****************************************
**
** Character device handle conversion
** (BDOS-type handle to BIOS-type handle)
**
*****************************************
*/

#define HXFORM(h)	bios_dev[-h-2]

/**********************
 *
 * BIOS function macros
 *
 **********************
*/

#define getmpb(a)   	trap13(0x00,a)
#define bconstat(a) 	trap13(0x01,a)		/* Character Input Status   */
#define bconin(a)		trap13(0x02,a)
#define bconout(a,b)	trap13(0x03,a,b)
#define Rwabs(a,b,c,d,e) trap13(0x04,a,b,c,d,e)
#define Getbpb(d)		(BPB *)trap13(0x07,d)	/* Get BIOS Parameter Block */
#define bconostat(a)	trap13(0x08,a)		/* Character Output Status  */
#define	Drvmap()		trap13(0x0A)		/* Get Drive Map	    */
#define CIOCR(d,l,b)	trap13(0x0C,d,l,b)	/* Char IOCtl Read	    */
#define CIOCW(d,l,b)	trap13(0x0D,d,l,b)	/* Char IOCtl Write	    */
#define DIOCR(d,l,b)	trap13(0x0E,d,l,b)	/* Disk IOCtl Read	    */
#define DIOCW(d,l,b)	trap13(0x0F,d,l,b)	/* Disk IOCtl Write	    */
#define CVE(d,a)		trap13(0x10,d,a)	/* Char Vector Exchange	    */


/**********************
 *
 * F_IOCtl subfunctions
 *
 **********************
*/

#define	XCVECTOR	-1			/* Exchange vector	    */
#define	GETINFO		0			/* Get device info	    */
#define	SETINFO		1			/* NOT IMPLEMENTED	    */
#define	CREADC		2			/* Character read control   */
#define	CWRITEC		3			/* Character write control  */
#define	DREADC		4			/* Disk read control	    */
#define	DWRITEC		5			/* Disk write control	    */
#define	INSTAT		6			/* Input status		    */
#define	OUTSTAT		7			/* Output status	    */
#define	REMEDIA		8			/* Removeable indication    */

/*************************
 *
 * Device information bits
 *
 *************************
*/

#define	Is_Console	0x0003			/* Both stdin & stdout	    */
#define	Is_NUL		0x0004
#define	Is_Clock	0x0008
#define	Is_Character	0x00C0			/* Character is binary now  */
#define Does_IOCtl	0x4000


/*
 * DTAINFO - Information stored in the dta by srch-frst for use by srch-nxt.
 * bytes 0-20 are reserved by o/s, and are used by sfirst/snext.  beyond
 * that, contents are published in programmer's guide.
 */
#define DTAINFO struct DtaInfo
DTAINFO
{
    char  dt_name[12];          /*  file name: filename.typ     00-11   */
    int32_t  dt_pos;            /*  dir position                12-15   */
    DND   *dt_dnd;              /*  pointer to DND              16-19   */
    char  dt_attr;              /*  attributes of file          20      */
                                /*  --  below must not change -- [1]    */
    char  dt_fattr;             /*  attrib from fcb             21      */
    DOSTIME dt_td;              /*  time, date fields from fcb  22-25   */
    int32_t  dt_fileln;         /*  file length field from fcb  26-29   */
    char  dt_fname[14];         /*  file name from fcb          30-43   */
};                              /*    includes null terminator          */


/* ERROR - error return code */
typedef	int32_t ERROR;
 
typedef	ERROR (*PFE) PROTO((NOTHING));			/* ptr to func ret err */
typedef	int32_t (*PFL) PROTO((NOTHING));		/* ptr to func ret long */

/*******************************
 *
 *  External Declarations
 *
 *******************************
*/

/* External Declarations */


extern	DND	*dirtbl[];
extern	DMD	*drvtbl[];
extern	char diruse[];
extern	int16_t	drvsel;
extern	int16_t logmsk[];
extern	FTAB sft[];
extern	ERROR rwerr;
extern	int16_t	errdrv;
extern	BCB	*bufl[2];		/*  in bios main.c		*/
extern	uint16_t time, date;
extern	int16_t	bios_dev[];		/*  in fsfioctl.c		*/
extern	ERROR errbuf[3];			/*  sup.c  */
extern int const nday[];						/* declared in sup.c */
extern int add[];
extern int remove[];

/********************************
 *
 *  Forward Declarations
 *
 ********************************
*/

int32_t	trap13 PROTO((int16_t, ...));
ERROR oscall PROTO((int16_t, ...));
VOID swp68 PROTO((uint16_t *p));
VOID swp68l PROTO((int32_t *p));

ERROR ixcreat PROTO((const char *fname, int8_t attr));
ERROR ixopen PROTO((const char *fname, int16_t mode));



ERROR ixsfirst PROTO((const char *name, int16_t att, DTAINFO *addr));
FCB *dirinit PROTO((DND *dn));
VOID builds PROTO((const char *src, char *dst));
char *dopath PROTO((DND *p, char *buf, int *len));
DND *findit PROTO((const char *name, const char **sp, int dflag));
DND	*makdnd PROTO((DND *, FCB *b));
int getpath PROTO((const char *p, char *d, int dirspec));
BOOLEAN match PROTO((const char *s1, const char *s2));
VOID makbuf PROTO((FCB *f, DTAINFO *dt));
int xcmps PROTO((const char *s, const char *d));
VOID freednd PROTO((DND *dn));
int namlen PROTO((const char *s11));

VOID flush PROTO((BCB *b));
char *getrec PROTO((int recn, DMD *dm, int wrtflg));
char *packit PROTO((const char *s, char *d));

ERROR ckdrv PROTO((int d));
DMD *getdmd PROTO((int drv));
int xlog2 PROTO((int n));

RECNO cl2rec PROTO((CLNO cl, DMD *dm));
VOID clfix PROTO((CLNO cl, CLNO link, DMD *dm));
CLNO getcl PROTO((int cl, DMD *dm));
int nextcl PROTO((OFD *p, int wrtflg));

long ixforce PROTO((int16_t std, int16_t h, PD *p));
int16_t syshnd PROTO((int16_t h));
VOID ixdirdup PROTO((int16_t h, int16_t dn, PD *p));
FH ffhndl PROTO((NOTHING));

typedef VOID (*xfer) PROTO((int, char *, char *));

ERROR xrw PROTO((int wrtflg, OFD *p, long len, char *ubufr, xfer bufxfr));
ERROR ixlseek PROTO((OFD *p, long n));
ERROR ixread PROTO((OFD *p, long len, VOIDPTR ubufr));
ERROR ixwrite PROTO((OFD *p, long len, VOIDPTR ubufr));

VOID xfr2usr PROTO((int, char *, char *));
VOID usr2xfr PROTO((int, char *, char *));
int uc PROTO((char c));
OFD *makofd PROTO((DND *p));
OFD *getofd PROTO((FH h));
int16_t divmod PROTO((int16_t *modp, int32_t divdnd, int16_t divsor));
ERROR F_IOCtl PROTO((int fn, FH h, int n, VOIDPTR buf));
int Chk_Drv PROTO((int16_t *d));

FCB *scan PROTO((DND *dnd, const char *n, int16_t att, int32_t *posp));
VOID sftdel PROTO((FTAB *sftp));

ERROR opnfil PROTO((FCB *f, DND *dn, int16_t mod));
ERROR makopn PROTO((FCB *f, DND *dn, FH h, int16_t mod));
ERROR ixclose PROTO((OFD *fd, int part));
ERROR ixdel PROTO((DND *dn, FCB *f, long pos));
BOOLEAN match1 PROTO((const char *ref, const char *test));

VOIDPTR xmgetblk PROTO((int i));
VOID xmfreblk PROTO((VOIDPTR m));

ERROR xpgmld PROTO((const char *s, PD *p));

VOID ixterm PROTO((PD *r));

VOID tabout PROTO((FH h, int ch));
int32_t constat PROTO((FH h));
int32_t conin PROTO((FH h));
int cgets PROTO((FH h, int maxlen, char *buf));

int contains_dots PROTO((const char *name, char ill));

/********************************
 *
 *  DOS entry points
 *
 ********************************
*/

ERROR xcreat PROTO((const char *fname, int8_t attr));
ERROR xopen PROTO((const char *fname, int16_t mode));
ERROR xmkdir PROTO((const char *s));
ERROR xrmdir PROTO((const char *p));
char xchmod PROTO((const char *p, int16_t wrt, char mod));
ERROR xsfirst PROTO((const char *name, int16_t att));
ERROR xsnext PROTO((NOTHING));
ERROR xgsdtof PROTO((uint16_t *buf, FH h, int16_t wrt));
ERROR xrename PROTO((int16_t n, const char *p1, const char *p2));
ERROR xchdir PROTO((const char *p));
ERROR xgetdir PROTO((char *buf, int16_t drv));
ERROR xgetfree PROTO((int32_t *buf, int16_t drv));
ERROR xforce PROTO((FH std, FH h));
ERROR xdup PROTO((FH h));
ERROR xlseek PROTO((long n, FH h, int16_t flg));
ERROR xread PROTO((FH h, long len, VOIDPTR ubufr));
ERROR xwrite PROTO((FH h, long len, VOIDPTR ubufr));
DTA *xgetdta PROTO((NOTHING));
VOID xsetdta PROTO((DTA *addr));
ERROR xsetdrv PROTO((int16_t drv));
ERROR xgetdrv PROTO((NOTHING));
ERROR xclose PROTO((FH h));
ERROR xunlink PROTO((const char *name));
ERROR xexec PROTO((int16_t flg, char *s, char *t, char *v));
VOID x0term PROTO((NOTHING));
VOID xterm PROTO((uint16_t rc));
VOID xtermres PROTO((int32_t blkln, int16_t rc));
int32_t xgetdate PROTO((NOTHING));
int32_t xsetdate PROTO((int16_t d));
int32_t xgettime PROTO((NOTHING));
int32_t xsettime PROTO((int16_t t));
int32_t xconstat PROTO((NOTHING));
int32_t xconostat PROTO((NOTHING));
int32_t xprtostat PROTO((NOTHING));
int32_t xauxistat PROTO((NOTHING));
int32_t xauxostat PROTO((NOTHING));
VOID xtabout PROTO((int16_t ch));
int16_t xauxout PROTO((int16_t ch));
int32_t xprtout PROTO((int16_t ch));
int32_t x7in PROTO((NOTHING));
int32_t xconin PROTO((NOTHING));
int32_t x8in PROTO((NOTHING));
int32_t xauxin PROTO((NOTHING));
int32_t rawconio PROTO((int16_t parm));
VOID xprt_line PROTO((const char *p));
VOID readline PROTO((char *p));
ERROR xmalloc PROTO((int32_t amount));
ERROR xmfree PROTO((int32_t addr));
ERROR xsetblk PROTO((int16_t n, VOIDPTR blk, int32_t len));
ERROR xmaddalt PROTO((char *start, int32_t len));
ERROR xmxalloc PROTO((int32_t amount, int16_t mode));



/*
 * FAT chain defines
 */
#define FREECLUSTER     0x0000
#define ENDOFCHAIN      0xffff                  /* our end-of-chain marker */
#define endofchain(a)   (((a)&0xfff8)==0xfff8)  /* in case file was created by someone else */


/* Misc. defines */
                    /* the following are used for the second arg to ixclose() */
#define CL_DIR  0x0002      /* this is a directory file, flush, do not free */
#define CL_FULL 0x0004      /* even though it's a directory, full close */


#endif /* FS_H */
