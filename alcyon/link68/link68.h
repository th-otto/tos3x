#include <stdio.h>

#define _(x) x

#define LINK68 1

#include <sendc68.h>

#define BOOLEAN int
#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define	PROGNAME "LINK68"	/* name of this program */
#define CBMAIN	"__cbmain"	/* main entry point in CBASIC program */

/* format of a symbol entry in the main table*/

#define SEEKREL	1	/*relative seek flag*/

#define DRIVELEN 2	/* drive spec length -- FOO */

#define LINELEN  132	/* maximum input/output string length */
#define FNAMELEN 15	/* drive, name, and type, null terminated */
#define MAXOVLS  255	/* maximum number of overlays */
#define	MAXOVDEP 5	/* maximum nesting depth for overlays */
#define	ROOT	 0	/* overlay number for root */
#define JSRL	0x4EB9	/* jsr to long address */
#define	JMPL	0x4EF9	/* jmp to long address */
#define DEFTYPE	".o"	/* default file type */
#define DEFLEN	2	/* length of deftype */

/*
 * lnkmess.c
 */
extern int exstat;

VOID banner PROTO((NOTHING));
VOID errorx PROTO((const char *fmt, ...)) __attribute__((format(printf, 1, 2)));
VOID fatalx PROTO((int witharrow, const char *fmt, ...)) __attribute__((format(printf, 2, 3)));
VOID synerr PROTO((const char *str));
VOID oom PROTO((NOTHING));
VOID usage PROTO((NOTHING));

/*
 * preproc.c
 */
struct filenode {			/* info about input file */
	char	fnfname[FNAMELEN];	/* input file name */
	short	fnflags;		/* option flags for file */
	struct filenode *fnnext;	/* next file in list */
};


VOID preproc PROTO((NOTHING));
VOID putarrow PROTO((NOTHING));

/*
 * link68.c
 */

VOID endit PROTO((int stat));




/* some global file stuff */

extern	char	cmdline[];		/* command line buffer */
extern	char	tdisk[];		/* drive for temp files */


/* option flag/switches */

extern int	mapflg;			/* generate a load map */
extern int	absflg;			/* absolute load */
extern int	symflg;			/* set to output symbol table */
extern int	ovflag;			/* set if loading overlays */
extern int	ignflg;			/* ignore 16-bit address overflow */
extern int	udfflg;			/* allow undefined symbols */
extern int	chnflg;			/* chained program */
extern int	dmpflg;			/* dump symbols -- undocumented */

/* flags for input files */

#define	FNALL	0100000			/* don't search library */
#define FNINCL	0040000			/* load named symbol from file */
#define FNSYM	0020000			/* symbol name -- not a file */
#define	FNLOCS	0010000			/* put locals in symbol table */

/* command tree structures */

#define NOPARENT -1			/* normal NULL == 0 */


struct jmpblock {			/* used for building overlay calls */
	struct symtab *globref;		/* pointer to global symbol */
	struct jmpblock *nxtjbl;	/* next jump block in list */
};

struct ovtrnode {			/* info about each overlay */
	char	ovfname[FNAMELEN];	/* output filename */
	long	ovtxbase;		/* base of text segment */
	long	ovdtbase;		/* base of init. data segment */
	long	ovbsbase;		/* base of local uninit. data */
	long	ovcap;			/* first word above overlay */
	struct symtab *ovfsym;	/* 1st sym tab ent for module */
	short	ovnxtsib;		/* next sibling overlay	node */
	short	ovfstkid;		/* first descendant overlay */
	short	ovparent;		/* parent node link */
	struct filenode *ovflist;	/* list of input files */
	struct jmpblock *ovjblck;	/* list of jump blocks */
};

extern struct ovtrnode *ovtree[MAXOVLS+1]; /* leave room for root info */
extern int numovls;			/* number of actual overlays */

/* structure of entry in output overlay table */



extern long textstart;
extern long datastart;
extern long bssstart;
extern int Bflag;
extern int Dflag;
extern int Zflag;
