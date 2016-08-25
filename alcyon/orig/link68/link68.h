#include <stdio.h>

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
#define EOS	'\0'	/* end of string */
#define JSRL	0x4EB9	/* jsr to long address */
#define	JMPL	0x4EF9	/* jmp to long address */
#define BDOSFNAM 12	/* length of a filename for a BDOS call */
#define PREORDER 1	/* preorder search of command tree */
#define POSTORDER 2	/* postorder search of command tree */
#define DEFTYPE	".O"	/* default file type */
#define DEFLEN	2	/* length of deftype */

/*
 * lnkmess.c
 */
VOID banner PROTO((NOTHING));
VOID errorx PROTO((int errnum, const char *st));
VOID usage PROTO((NOTHING));

/*
 * preproc.c
 */
VOID preproc PROTO((NOTHING));
VOID parsecmd PROTO((NOTHING));
int scan PROTO((NOTHING));
int lookahd PROTO((NOTHING));
VOID globops PROTO((NOTHING));
VOID inparse PROTO((int ovnum, int parent));
struct ovtrnode *newovnod PROTO((NOTHING));
long scannum PROTO((NOTHING));
VOID cmdfile PROTO((NOTHING));
VOID addfltyp PROTO((char *flname, char *fltype));
struct filenode *newflnod PROTO((NOTHING));
VOID locops PROTO((struct filenode *fnpt));
int readop PROTO((char *oname));
BOOLEAN match PROTO((const char *s1, const char *s2));
VOID tdrvscan PROTO((NOTHING));
VOID inclname PROTO((struct filenode *fnode));
VOID symscan PROTO((NOTHING));
VOID putarrow PROTO((NOTHING));
VOID println PROTO((const char *st));

/*
 * link68.c
 */

VOID endit PROTO((int stat));




extern	const char	*errst1;		/* used for passing values to */
extern	const char	*errst2;		/* error processor */
extern	long	errnum1;
extern	long	errnum2;

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

/* error numbers -- see errorx() in main module for exact messages */

#define BADCHAR 	1		/* illegal character */
#define BADSYNT 	2		/* catchall syntax error */
#define CMDTRUNC	3		/* unexpected end of command */
#define BADOPT  	4		/* junk or out-of-place option */ 
#define NOROOM  	5		/* no more heap space */
#define BADNUM  	6		/* ill-formed hex number */
#define MORECMD 	7		/* parse done - stuff left over */
#define BADINFIL	8		/* can't open file for reading */
#define XTRACFIL	9		/* nested command file */
#define XESSOVLS	10		/* too many overlays */
#define CMDLONG 	11		/* command line too long */
#define OVTODEEP	12		/* overlays nested too deep */
#define DISCONTIG	13		/* discontig. load with overlays */
#define	BADOVREF 	14		/* illegal reference to overlay */
#define READERR  	15		/* file read error */
#define FORMATERR	16		/* file format error */
#define NORELOC  	17		/* no relocation bits in file */
#define BADSYMFLG	18		/* invalid symbol flags */
#define DUPDEF		19		/* duplicate definition */
#define SYMOFL		20		/* symbol table overflow */
#define BADTEMP		21		/* cannot open temp file */
#define INTERR		22		/* internal error */
#define UNDEF		23		/* undefined symbol */
#define BADOUT		24		/* cannot open output file */
#define SEEKERR  	25		/* seek read error */
#define SHRTOFL  	26		/* short address overflow */
#define TSZERR		27		/* text size error */
#define OWRTERR  	28		/* output file write error */
#define REOPNERR 	29		/* unable to reopen temp file */
#define	RELADROFL	30		/* relative address overflow */
#define	BADRELOC	31		/* bad relocation flag */


/* command tree structures */

#define NOPARENT -1			/* normal NULL == 0 */


struct filenode {			/* info about input file */
	char	fnfname[FNAMELEN];	/* input file name */
	short	fnflags;		/* option flags for file */
	struct filenode *fnnext;	/* next file in list */
};


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

extern union mlong textstart;
extern union mlong datastart;
extern union mlong bssstart;
extern int Bflag;
extern int Dflag;
extern int Zflag;
