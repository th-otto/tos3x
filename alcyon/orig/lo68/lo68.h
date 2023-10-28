/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "cout.h"
#include "ar68.h"

/* format of a symbol entry in the main table*/

struct symtab {
	char name[SYNAMLEN];		/*symbol name*/
	short flags;			/*bit flags*/
	long vl1;			/*symbol value*/
	char *tlnk;			/*table link*/
};
struct symtab *symptr;

char *bmte;		/*beginning of main table*/
char *emte;		/*end of main table*/

/* flags for symbols*/
#define SYDF	0100000		/*defined*/
#define SYEQ	0040000		/*equated*/
#define SYGL	0020000		/*global - entry or external*/
#define SYER	0010000		/*equated register*/
#define SYXR	0004000		/*external reference*/
#define SYDA	0002000		/*DATA based relocatable*/
#define SYTX	0001000		/*TEXT based relocatable*/
#define SYBS	0000400		/*BSS based relocatable*/
#define SYFN	0000200		/* ?.o file name */


struct irts {
	char *irle;		/*ptr to last entry in chain*/
	char *irfe;		/*ptr to first entry in chain*/
};

/* parameters that define the main table*/
#define SZMT 300		/*initial size of the main table*/
						/*must be large enough to initialize*/
#define ICRSZMT 100		/*add to main table when run out*/
int cszmt;				/*current size of main table*/

/*initial reference table for externals*/
#define SZIRT	64
char *eirt[SZIRT];
char *saveirt[SZIRT];

/*initial reference table for globals*/
char *girt[SZIRT];
char *savgirt[SZIRT];

char **pirt;
char *lmte;		/*last entry in main table*/
char *savlmte;

#define AREGLO		8
#define AREGHI		15

/* relocation bit definitions:*/
#define RBMASK		07		/*tells type of relocation*/
#define INSABS		7		/*first word of instr -- absolute*/
#define DABS		0		/*data word absolute*/
#define TRELOC		2		/* TEXT relocatable*/
#define DRELOC		1		/* DATA relocatable*/
#define BRELOC		3		/* BSS relocatable*/
#define EXTVAR		4		/* ref to external variable*/
#define LUPPER		5		/* upper word of long*/
#define EXTREL		6		/* relative mode on external variable*/

#define BSIZE	512

struct buf {
	int fildes;
	int nunused;
	char *xfree;
	char buff[BSIZE];
};

struct buf ibuf;
struct buf tbuf;
struct buf obuf;
struct buf rbuf;
struct buf rtbuf;
struct buf rdbuf;

int sflag;		/*remove symbol table*/

int32_t textbase;
int32_t database;
int32_t bssbase;
int32_t textsize;
int32_t datasize;
int32_t bsssize;
int32_t stacksize;

int32_t textstart;
int32_t datastart;
int32_t bssstart;

char *ifilname;			/*points to name of current input file*/

#define NFILE	256		/*max # files we can process*/
char *fsymp[NFILE];		/*points to first symbol for each .o file*/
char **firstsym;		/*points to entry in fsymp*/

int extmatch;			/*matched an external in a library entry*/
int noload;				/*dont load this lib file flag*/

#define LIB1MAGIC	0xff6d
#define LIB2MAGIC	0xff65
#define LIB1HDSIZE	16
#define LIB2HDSIZE	28

struct lib2hdr
{
	char l2fname[14];
	int32_t l2modti;
	char l2userid;
	char l2gid;
	short l2fimode;
	int32_t l2fsize;
	short l2junk;
};

#define NLIB	16		/*max # libraries to process*/
int lbfictr[NLIB];		/*counts files loaded from one library*/
int *libfctr;			/*points to lbfictr*/
int32_t lbfioff[NFILE];	/*each file offset in library*/
int32_t *libptr;			/*points to lbfioff*/

#define	ARMAG	"!<arch>\n"
#define	SARMAG	8

#define	ARFMAG	"`\n"

struct ar_hdr
{
	char ar_name[16];
	char ar_date[12];
	char ar_uid[6];
	char ar_gid[6];
	char ar_mode[8];
	char ar_size[10];
	char ar_fmag[2];
};
struct ar_hdr lib3_hdr;

int libhdsize;
#if 0
struct libhdr libhd;	/* 15 apr 83, used to be int[13] */
#else
long  libhd[20];		/*length is max lib head size*/
#endif

int undflg;
char dafnc;
int pass2;
long stlen;
int Xflag;
int Dflag;
int Bflag;
int Zflag;

struct symtab *etextptr;
struct symtab *edataptr;
struct symtab *endptr;
char *lastdup;

char rtfnc;
char rdfnc;
int saverbits;

#define TWOKSHT		-1		/* 2k shared text */
#define FOURKSHT	1	/* 4k shared text */

#define DEFSHTEXT	FOURKSHT

