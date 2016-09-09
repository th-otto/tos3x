/*	bios.h - bios defines						*/

/*
 *
 * MODIFICATION HISTORY
 *	14 Oct 85	KTB create from fs.h
 *
 *
 * NAMES
 *
 *	SCC Steve C. Cavender
 *	KTB Karl T. Braun (kral)
 */

#ifndef PD_H
#include "pd.h"
#endif
#ifndef FS_H
#include "fs.h"
#endif

/*
 *	Bios Function Numbers
 */

#define B_MDCHG 	9		/*	media change		*/

/*
 * BIOS level character device handles
 */

#define BFHPRN	0
#define BFHAUX	1
#define BFHCON	2
#define BFHCLK	-1
#define BFHMOU	-2


/*
 *	return codes
 */

#define DEVREADY	-1L 	/*	device ready		*/
#define DEVNOTREADY 0L		/*	device not ready		*/
#define MEDIANOCHANGE	0L		/*	media def has not changed	*/
#define MEDIAMAYCHANGE	1L		/*	media may have changed	*/
#define MEDIACHANGE 2L		/*	media def has changed	*/

/*
 *	code macros
 */

#define ADDRESS_OF(x)	x


/*
 *	bios data types
 */


/*
 *	ISR - Interrupt Service Routines.
 *	These routines currently do not return anything important.	In
 *	future versions, they will return boolean values that indicate
 *	whether a displatch should occurr (TRUE) or not.
 */

typedef BOOLEAN ISR;		/*	interrupt service routine	*/
typedef ISR (*PISR) PROTO((NOTHING)); /*	pointer to isr routines */

/*
 *	SSN - Sequential Sector Numbers
 *	At the outermost level of support, the disks look like an
 *	array of sequential logical sectors.  The range of SSNs are
 *	from 0 to n-1, where n is the number of logical sectors on
 *	the disk.  (logical sectors do not necessarilay have to be
 *	the same size as a physical sector.
 */

typedef int32_t SSN;


/*
 *	Data Structures
 */

/*
 *	BPB - Bios Parameter Block
 */

#define BPB struct _bpb_t
BPB /* bios parameter block */
{
	int16_t recsiz; 	/* sector size in bytes */
	int16_t clsiz;		/* cluster size in sectors */
	int16_t clsizb; 	/* cluster size in bytes */
	int16_t rdlen;		/* root directory length in records */
	int16_t fsiz;		/* fat size in records */
	int16_t fatrec; 	/* first fat record (of last fat) */
	int16_t datrec; 	/* first data record */
	int16_t numcl;		/* number of data clusters available */
	int16_t b_flags;
};

ERROR login PROTO((BPB *b, int drv));


/*
 *	flags for BPB
 */

#define B_16	1			/* device has 16-bit FATs	*/
#define B_FIX	2			/* device has fixed media	*/


/*
 *  DMD - Drive Media Block
 */

/*  records == logical sectors  */

DMD /* drive media block */
{
    RECNO   m_recoff[3]; /*  record offsets for fat,dir,data     */
    int16_t m_drvnum;    /*  drive number for this media         */
    RECNO m_fsiz;        /*  fat size in records                 */
    RECNO m_clsiz;       /*  cluster size in records             */
    uint16_t m_clsizb;   /*  cluster size in bytes               */
    int16_t m_recsiz;    /*  record size in bytes                */

    CLNO    m_numcl;     /*  total number of clusters in data    */
    int16_t m_clrlog;    /* log (base 2) of clsiz in records     */
    int16_t m_clrm;      /* clsiz in rec, mask                   */
    int16_t m_rblog;     /* log (base 2) of recsiz in bytes      */
    int16_t m_rbm;       /* recsiz in bytes, mask                */
    int16_t m_clblog;    /* log (base 2) of clsiz in bytes       */
    int16_t m_clbm;      /* clsiz in bytes, mask                 */
    OFD    *m_fatofd;    /* OFD for 'fat file'                   */

    OFD     *m_ofl;      /*  list of open files                  */
    DND     *m_dtl;      /* root of directory tree list          */
    uint16_t m_16;       /* 16 bit fat ?                         */
};



/*
 *	BCB - Buffer Control Block
 */

/*
 *  BCB - Buffer Control Block
 *
 *  note that this is part of the TOS API, via the pointers to BCB
 *  chains at bufl[2].  for compatibility with existing programs
 *  (such as CACHEnnn.PRG and HDDRIVER), the length must be 20 bytes,
 *  and the fields marked (API) below must remain the same (contents,
 *  size, and offset within structure).
 */

BCB
{
    BCB     *b_link;    /*  next bcb (API)              */
    int16_t b_bufdrv;   /*  unit for buffer (API)       */
    int16_t b_buftyp;   /*  buffer type                 */
    RECNO   b_bufrec;   /*  record number               */
    int16_t b_dirty;    /*  true if buffer dirty        */
    DMD     *b_dm;      /*  ptr to drive media block    */
    char    *b_bufr;    /*  pointer to buffer (API)     */
};


/*
 *	buffer type values
 */

#define BT_FAT		0		/*	fat buffer			*/
#define BT_ROOT 	1		/*	root dir buffer 	*/
#define BT_DIR		2		/*	other dir buffer		*/
#define BT_DATA 	3		/*	data buffer 		*/

/*
 *	buffer list indexes
 */

#define BI_FAT		0		/*	fat buffer list 	*/
#define BI_ROOT 	1		/*	root dir buffer list	*/
#define BI_DIR		1		/*	other dir buffer list	*/
#define BI_DATA 	1		/*	data buffer list		*/


/*
 *	MD - Memory Descriptor
 */

#define MD struct _md
MD
{
	MD	*m_link;
	long	m_start;
	long	m_length;
	PD	*m_own;
};

/*
 *	fields in Memory Descriptor
 */

#define MF_FREE 1


/*
 *	MPB - Memory Partition Block
 */

#define MPB struct _mpb
MPB
{
	MD	*mp_mfl;
	MD	*mp_mal;
	MD	*mp_rover;
};




/* For BIOS calls... */

#define date_time(op,var)	trap13(0x11,(op),(var))
#define GET_TIME	0
#define SET_TIME	1
#define GET_DATE	2
#define SET_DATE	3



/*
 *	VME/10 machine dependent stuff.
 */

#define IVNKBD	0x42		/*	keyboard interrupt vector number	*/
#define IVNDSK	0x44		/*	disk controller interrupt vec no	*/
#define IVNSER	0x45		/*	serial port interrupt vector number */

#define IVNABT	0x4e		/*	abort button interrupt vector no	*/
