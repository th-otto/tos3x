/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "../common/linux/libcwrap.h"
#include <stdio.h>
#include <ctype.h>
#include <cout.h>
#include "../util/util.h"

#define _(x) x
#define N_(x) x


/* flags for symbols */
#define SYDF    0x8000     /* defined */
#define SYEQ    0x4000     /* equated */
#define SYGL    0x2000     /* global - entry or external */
#define SYER    0x1000     /* equated register */
#define SYXR    0x0800     /* external reference */
#define SYRA    0x0400     /* DATA based relocatable */
#define SYRO    0x0200     /* TEXT based relocatable */
#define SYBS    0x0100     /* BSS based relocatable */
#define SYIN    0x0080     /* internal symbol -- opcode, dir or equ */
#define SYPC    0x0040     /* equated using star '*' expression */
#define SYRM    0x0020     /* register mask equate */

/* flags for opcodes and directives */
#define OPDR   0x8000      /* 0=>opcode, 1=>directive */
#define OPFF   0x1f        /* type of instruction (used as mask) */

/* intermediate text types */
#define ITBS    0       /* beginning of statement */
#define ITSY    1       /* pointer to symbol table */
#define ITCN    2       /* constant */
#define ITSP    3       /* special */
#define ITRM    4       /* register mask */
#define ITPC    5       /* pc relative argument */
#define ITCW    6       /* constant.w */

/* Effective address mode bits */
#define DDIR        000
#define ADIR        010
#define INDIRECT    020
#define INDINC      030
#define DECIND      040
#define INDDISP     050
#define INDINX      060
#define SADDR       070
#define LADDR       071
#define IMM         074

/* Register Range */
#define AREGLO      8
#define AREGHI      15

/* Relocation bit definitions: */
#define RBMASK      07      /* tells type of relocation */
#define INSABS      7       /* first word of instr -- absolute */
#define DABS        0       /* data word absolute */
#define TRELOC      2       /* TEXT relocatable */
#define DRELOC      1       /* DATA relocatable */
#define BRELOC      3       /* BSS relocatable */
#define EXTVAR      4       /* ref to external variable */
#define LUPPER      5       /* upper word of long */
#define EXTREL      6       /* external relative mode */

/* Register values, as reflected in as68init */
#define CCR     16
#define SR      17
#define USP     18
#define BYTE_ID 19
#define WORD_ID 20
#define LONG_ID 21
#define PC      22
#define SFC     23      /* control register for 68010 */
#define DFC     24      /* control register for 68010 */
#define VBR     25      /* control register for 68010 */

/* Control Register Numeric Values */
#define SFC_CR  0
#define DFC_CR  1
#define USP_CR  0x800
#define VBR_CR  0x801

/* Instruction Formats */
#define ANDI    01000
#define AND     0140000
#define ORI     0
#define OR      0100000
#define EORI    05000
#define EOR     0130000
#define MOVE    0
#define MOVEC   047172  /* 68010 */
#define MOVES   07000   /* 68010 */
#define RTD     047164  /* 68010 */
#define MOVETCC 042300
#define MOVEFCC 041300  /* 68010 */
#define MOVESR  043300
#define SRMOVE  040300
#define MOVEUSP 047140
#define CLRVAL  041000

#define CLRFOR  24

/* relocation values */
#define ABS    0   /* absolute */
#define DATA   1
#define TEXT   2
#define BSS    3
#define EXTRN  4   /* externally defined */

/* Conditional Assembly variables and constants */
#define LOW_CA  21
#define HI_CA   30

/* Size attribute */
#define BYTE    'b'
#define WORD    'w'
#define LONG    'l'
#define BYTESIZ 1
#define WORDSIZ 2
#define LONGSIZ 4

/* Ascii values */
#define EOLC    '\n'/* end of line character */
#define SOH     1
#define CEOF     0

/* Miscellaneous Defines */
#ifndef FALSE
#define TRUE    1   /* boolean values */
#define FALSE   0   /* boolean values */
#endif

#define ITBSZ   256 /* size of the it buffer */
#define STMAX   200 /* size of intermediate text buffer */
#define SZIRT   128
#define EXTSZ   1024
#define DIRECT  34  /* number of entries in p2direct */
#define ORGDIR  14  /* org entry in p2direct */

/*
 * intermediate text file
 * format of the intermediate text for one statement:
 *
 *  ******************************************************
 *  *  it type = ITBS      *     # it entries            *  0
 *  ******************************************************
 *  *       absolute line number (long)                  *
 *  ******************************************************
 *  *  it type = ITSY      * instr length                *  1
 *  ******************************************************
 *  *  symbol table pointer for stmt label (long)        *
 *  ******************************************************
 *  *  it type = ITSY      *  instr mode length          *  2
 *  ******************************************************
 *  *            opcode ptr (long)                       *
 *  ******************************************************
 *  *  it type = ITCN      *  relocation base            *  3
 *  ******************************************************
 *  *            location counter (pass 1)               *
 *  ******************************************************
 *  *  it type             *  relocation flag            *  4 - oprnds
 *  ******************************************************
 *  *               value (long)                         *
 *  ******************************************************
 *                         .
 *
 *                         .
 *  ******************************************************
 *  *  it type             *  relocation flag            *  n - oprnds
 *  ******************************************************
 *  *               value (long)                         *
 *  ******************************************************
 */

#define ITOP1   4   /* first it entry for operands */

/* format of a symbol entry in the main table */
struct symtab {
    char name[SYNAMLEN]; /* symbol name */
    unsigned short flags;
	int32_t vl1;         /* symbol value */
	short vextno;		 /* external symbol reference # */
    struct symtab *tlnk; /* hash table link */
    struct symtab *next; /* global table link */
};

/*
 *  it type             meaning
 *      0           beginning of statement
 *      1           value is pointer to symbol table
 *      2           value is a constant
 *      3           value is a specal char
 *
 *  relocation flag for opcode it entry is operand length:
 *      'b' => byte
 *      'w' => word
 *      'l' => long
 */

union iival {
	int32_t l;
	int32_t oper;
	struct symtab *ptrw2;
	VOIDPTR p;
};


struct it {
	union {
	    struct {
	    	char _itty;          /* it type */
	    	char _itrl;          /* relocation flag or # it entries */
	    } b;
	    short w;
	} b;
    union iival itop;
};
#define itty b.b._itty
#define itrl b.b._itrl
#define swd1 b.w

extern short mode;             /* operand mode (byte, word, long) */
extern short modelen;          /* operand length per mode */

extern struct symtab *bmte;    /* beginning of main table */
extern struct symtab *emte;    /* end of main table */

extern short itbuf[ITBSZ];     /* it buffer */

extern struct it stbuf[STMAX]; /* holds it for one statement */

extern struct symtab *lmte;    /* last entry in main table */

struct irts {
	struct symtab *irle;		/* ptr to last entry in chain */
	struct symtab *irfe;		/* ptr to first entry in chain */
};

extern int32_t stlen;             /* length of symbol table */

/* initial reference table for symbols */
extern struct irts sirt[SZIRT];

/* initial reference table to opcodes */
extern struct irts oirt[SZIRT];

/* external symbol table */
extern struct symtab *extbl[EXTSZ];
extern int extindx;            /* index to external symbol table */

extern int absln;              /* absolute line number */
extern int p2absln;            /* pass 2 line number */
extern short fcflg;            /* 0=>passed an item.  1=>first char */
extern short fchr;             /* first char in term */
extern FILE *ifn;              /* source file descriptor */
extern int pitix;              /* ptr to it buffer */
extern short itwc;             /* number of words in it buffer */
extern struct it *pitw;        /* ptr to it buffer next entry */
extern short itype;            /* type of item */
extern union iival ival;       /* value of item */
extern struct symtab *lblpt;   /* label pointer */
extern char lbt[SYNAMLEN + 1]; /* holds label name */
extern int32_t loctr;          /* location counter */
extern int32_t savelc[4];      /* save relocation counters for 3 bases */
extern short nite;             /* number of entries in stbuf */
extern struct it *pnite;
extern struct symtab *opcpt;   /* pointer to opcode entry in main table */
extern short p2flg;            /* 0=>pass 1  1=>pass 2 */
extern struct irts *pirt;      /* entry in initial reference table */
extern short reloc;            /* reloc value returned by expr evaluator (expr) */
extern short rlflg;            /* relocation value of current location counter */
extern struct hdr2 couthd;     /* cout header structure */

extern short format;
extern FILE *itfn;             /* it file number */
extern short prtflg;           /* print output flag */
extern short undflg;           /* make undefined symbols external flag */

extern short starmul;          /* * is multiply operator */

/* Symbol Table Pointers for Subset of Opcodes */
extern struct symtab *endptr;
extern struct symtab *addptr;
extern struct symtab *orgptr;
extern struct symtab *subptr;
extern struct symtab *addiptr;
extern struct symtab *addqptr;
extern struct symtab *subiptr;
extern struct symtab *subqptr;
extern struct symtab *cmpptr;
extern struct symtab *addaptr;
extern struct symtab *cmpaptr;
extern struct symtab *subaptr;
extern struct symtab *cmpmptr;
extern struct symtab *dcptr;
extern struct symtab *andptr;
extern struct symtab *andiptr;
extern struct symtab *eorptr;
extern struct symtab *eoriptr;
extern struct symtab *orptr;
extern struct symtab *oriptr;
extern struct symtab *cmpiptr;
extern struct symtab *moveptr;
extern struct symtab *moveqptr;
extern struct symtab *exgptr;
extern struct symtab *evenptr;
extern struct symtab *jsrptr;
extern struct symtab *bsrptr;
extern struct symtab *nopptr;

extern short indir[2];
extern short immed[2];
extern short plevel;           /* parenthesis level counter */
extern short opdix;            /* operand index counter */

/* ptrs to ins[] and rlbits[] */
extern short *pins;
extern short *prlb;
extern short ins[5];           /* holds instruction words */

extern short extflg, extref;   /* external in expr */

struct op {
    short ea;           /* effective address bits */
    short len;          /* effective address length in bytes */
    int32_t con;        /* constant or reloc part of operand */
    short drlc;         /* reloc of con */
    short ext;          /* external variable # */
    short idx;          /* index register if any */
    short xmod;         /* mode of index reg */
};

extern struct op opnd[2];

extern FILE *lfil;				/* loader output file descriptor */
extern FILE *dafil;            /* temp file for data stuff */
extern FILE *trfil;			/* temp for text relocation bits */
extern FILE *drfil;            /* temp for data relocation bits */

#ifndef PATH_MAX
#ifdef _WIN32
#define PATH_MAX 260 /* avoid redefinition */
#else
#define PATH_MAX 256
#endif
#endif

extern char itfilnam[PATH_MAX];
extern char dafilnam[PATH_MAX];
extern char trfilnam[PATH_MAX];
extern char drfilnam[PATH_MAX];
extern char ldfn[PATH_MAX];        /* name of the relocatable object file */
extern char *sfname;				/* Source filename */

/* assembler flag variables */
extern short didorg;
extern short shortadr;         /* short addresses if set */
extern short m68010;           /* 68010 code */
extern short aesflag;
extern char *lineftbl;

/* pass 1 global variables */
extern short numops;           /* number of operands */
extern short inoffset;         /* offset directive */
extern short p1inlen;          /* pass 1 instr length */

/* pass 2 global variables */
extern short instrlen;         /* pass 2 bytes in current instruction */
  
/* General Assembler Variables */
extern char peekc;
extern short ca_true;          /* true unless in a false CA */
extern short ca;               /* depth of conditional assembly, none = 0 */
extern short ca_level;         /* at what CA depth did CA go false? */
extern short nerror;           /* # of assembler errors */
extern short in_err;           /* don't generate instrlen err if in err state */
extern int32_t itoffset;
extern short equflg;           /* doing an equate stmt */
extern short refpc;            /* * referenced in expr */

/* defines */
#undef tolower
#undef islower
#undef isalpha
#undef isdigit
#undef isalnum
#define tolower(c)  ((c)<='Z' && (c)>='A') ? (c)|32 : (c)
#define islower(c)  ((c) <= 'z' && (c) >= 'a')
#define isalpha(c)  (islower( (c) | 32 ))
#define isdigit(c)  ((c) >= '0' && (c) <= '9')
#define isalnum(c)  (isalpha(c) || isdigit(c))
#define igblk()     while(fchr==' ') fchr=gchr()
#define ckein()     ((pitw >= pnite))

/* is it an alterable operand */
#define memalt(ap)  (memea(ap) && altea(ap))
#define dataalt(ap) (dataea(ap) && altea(ap))
#define altea(ap)   ((((ap)->ea&070)!=SADDR || ((ap)->ea&6)==0))

/* is it the specific type of operand */
#define memea(ap)   (((ap)->ea&070) >= INDIRECT)
#define dataea(ap)  (((ap)->ea&070) != ADIR)
#define pcea(ap)    ((ap)->ea==072 || (ap)->ea==073)
#define ckdreg(ap)  ((ap)->ea>=0 && (ap)->ea<AREGLO)
#define ckareg(ap)  ((ap)->ea>=AREGLO && (ap)->ea<=AREGHI)
#define ckreg(ap)   ((ap)->ea>=0 && (ap)->ea<=AREGHI)

typedef VOID (*adirect) PROTO((NOTHING));

extern adirect const p2direct[];


/*
 * dir.c
 */
VOID dorlst PROTO((int xrtyp));
VOID mkextidx PROTO((struct symtab *p));
/* Directive Handling Subroutines */
VOID hopd PROTO((NOTHING));
VOID hequ PROTO((NOTHING));
VOID hdsect PROTO((NOTHING));
VOID hpsect PROTO((NOTHING));
VOID hbss PROTO((NOTHING));
VOID heven PROTO((NOTHING));
VOID hent PROTO((NOTHING));
VOID hext PROTO((NOTHING));
VOID hend PROTO((NOTHING));
VOID hds PROTO((NOTHING));
VOID hdc PROTO((NOTHING));
VOID hascii PROTO((NOTHING));
VOID horg PROTO((NOTHING));
VOID hmask2 PROTO((NOTHING));
VOID hreg PROTO((NOTHING));
VOID hdcb PROTO((NOTHING));
VOID hcomline PROTO((NOTHING));
VOID hidnt PROTO((NOTHING));
VOID hoffset PROTO((NOTHING));
VOID hsection PROTO((NOTHING));
VOID hopt PROTO((NOTHING));
VOID hpage PROTO((NOTHING));
VOID spage PROTO((NOTHING));
VOID httl PROTO((NOTHING));
/* Second Pass Subroutines */
VOID send PROTO((NOTHING));
VOID sds PROTO((NOTHING));
VOID sdcb PROTO((NOTHING));
VOID sdsect PROTO((NOTHING));
VOID spsect PROTO((NOTHING));
VOID sbss PROTO((NOTHING));
VOID seven PROTO((NOTHING));
VOID sorg PROTO((NOTHING));
VOID sdc PROTO((NOTHING));
VOID sascii PROTO((NOTHING));
VOID ssection PROTO((NOTHING));
/* Conditional assembly directives */
VOID hifeq PROTO((NOTHING));
VOID hifne PROTO((NOTHING));
VOID hiflt PROTO((NOTHING));
VOID hifle PROTO((NOTHING));
VOID hifgt PROTO((NOTHING));
VOID hifge PROTO((NOTHING));
VOID hifc PROTO((NOTHING));
VOID hifnc PROTO((NOTHING));
VOID hendc PROTO((NOTHING));


/*
 * expr.c
 */
typedef VOID (*aexpr) PROTO((NOTHING));

VOID expr PROTO((aexpr iploc));
VOID p1gi PROTO((NOTHING));
VOID p2gi PROTO((NOTHING));


/*
 * list.c
 */
VOID psyms PROTO((NOTHING));
VOID print PROTO((int pflag));
VOID prtline PROTO((int flg));
VOID page PROTO((NOTHING));

/*
 * main.c
 */
VOID dlabl PROTO((NOTHING));
VOID opito PROTO((NOTHING));
VOID opitoo PROTO((NOTHING));


/*
 * misc.c
 */
VOID clrea PROTO((struct op *ap));
VOID getea PROTO((int opn));
int getreg PROTO((NOTHING));
int ckitc PROTO((const struct it *ckpt, int cksc));
VOID ristb PROTO((NOTHING));
int ckeop PROTO((int uen));
VOID osymt PROTO((NOTHING));
VOID fixunds PROTO((NOTHING));
VOID outbyte PROTO((int bv, int br));
VOID outword PROTO((unsigned short val, unsigned short rb));
VOID outinstr PROTO((NOTHING));
VOID cpdata PROTO((NOTHING));
VOID cprlbits PROTO((NOTHING));
int ckcomma PROTO((NOTHING));
VOID doea PROTO((struct op *apea));
VOID dodisp PROTO((struct op *ap));
VOID cksize PROTO((struct op *ap));
VOID chkimm PROTO((struct op *ap));
VOID genimm PROTO((NOTHING));


/*
 * pass1a.c
 */
VOID pass1a PROTO((NOTHING));


/*
 * pass2.c
 */
extern short rlbits[];
extern short const f2mode[];
VOID pass2 PROTO((NOTHING));


/*
 * symt.c
 */
extern int poslab;

VOID initsy PROTO((NOTHING));
VOID opitb PROTO((NOTHING));
VOID gterm PROTO((int constpc));
struct symtab *lemt PROTO((int oplook, struct irts *airt));
VOID mmte PROTO((NOTHING));
struct symtab *mdemt PROTO((const char *mdstr, int dirnum));
VOID pack PROTO((const char *apkstr, struct symtab *apkptr));
int gchr PROTO((NOTHING));
int nameeq PROTO((const char *name1, const char *name2, int len));
VOID wostb PROTO((NOTHING));
VOID uerr PROTO((int errn, ...));
VOID xerr PROTO((int errn, ...));
VOID asabort PROTO((NOTHING)) __attribute__((noreturn));
VOID ligblk PROTO((NOTHING));
VOID igrst PROTO((NOTHING));
VOID endit PROTO((NOTHING)) __attribute__((noreturn));
VOID setname PROTO((NOTHING));
VOID rpterr PROTO((const char *ptch, ...)) __attribute__((format(__printf__, 1, 2)));
VOID setldfn PROTO((const char *ap));
FILE *openfi PROTO((const char *pname, const char *mode));
VOID prstbuf PROTO((const char *tag));
VOID prits PROTO((const struct it *its, int cnt));


unsigned short isaes PROTO((const char *name));
VOID readlineftab PROTO((NOTHING));
