/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

#include "mach.h"
#ifdef VAX11
#   include <c68/sys/cout.h>
#endif
#ifdef PDP11
#   include <c68/sys/cout.h>
#endif
#ifdef MC68000
#   include <sys/cout.h>
#endif
#ifdef DRI
#	include <stdio.h>
#	include <klib.h>
#	include <ctype.h>
#	include <cout.h>
#endif

    /* flags for symbols*/
#define SYDF    0100000     /*defined*/
#define SYEQ    0040000     /*equated*/
#define SYGL    0020000     /*global - entry or external*/
#define SYER    0010000     /*equated register*/
#define SYXR    0004000     /*external reference*/
#define SYRA    0002000     /*DATA based relocatable*/
#define SYRO    0001000     /*TEXT based relocatable*/
#define SYBS    0000400     /*BSS based relocatable*/
#define SYIN    0000200     /*internal symbol -- opcode, dir or equ*/
#define SYPC    0000100     /*[vlh]equated using star '*' expression*/
#define SYRM    0000040     /*[vlh]register mask equate*/

    /*flags for opcodes and directives*/
#define OPDR   0100000     /*0=>opcode, 1=>directive*/
#define OPFF   037         /*type of instruction (used as mask)*/

    /* intermediate text types*/
#define ITBS    0       /*beginning of statement*/
#define ITSY    1       /*pointer to symbol table*/
#define ITCN    2       /*constant*/
#define ITSP    3       /*special*/
#define ITRM    4       /*[vlh]register mask!*/
#define ITPC    5       /*[vlh]pc relative argument*/

    /* Effective address mode bits*/
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

    /* Relocation bit definitions:*/
#define RBMASK      07      /*tells type of relocation*/
#define INSABS      7       /*first word of instr -- absolute*/
#define DABS        0       /*data word absolute*/
#define TRELOC      2       /* TEXT relocatable*/
#define DRELOC      1       /* DATA relocatable*/
#define BRELOC      3       /* BSS relocatable*/
#define EXTVAR      4       /* ref to external variable*/
#define LUPPER      5       /* upper word of long*/
#define EXTREL      6       /* external relative mode*/

    /* Register values, as reflected in as68init */
#define CCR     16
#define SR      17
#define USP     18
#define WORD_ID 20      /* [vlh] 4.2 */
#define PC      22
#define SFC     23      /* [vlh] 4.2, control register for 68010 */
#define DFC     24      /* [vlh] 4.2, control register for 68010 */
#define VSR     25      /* [vlh] 4.2, control register for 68010 */

    /* Control Register Numeric Values */
#define SFC_CR  0
#define DFC_CR  1
#define USP_CR  0x800
#define VSR_CR  0x801

    /* Instruction Formats */
#define ANDI    01000
#define AND     0140000
#define ORI     0
#define OR      0100000
#define EORI    05000
#define EOR     0130000
#define MOVE    0
#define MOVEC   047172  /* [vlh] 4.2, 68010 */
#define MOVES   07000   /* [vlh] 4.2, 68010 */
#define RTD     047164  /* [vlh] 4.2, 68010 */
#define MOVETCC 042300
#define MOVEFCC 041300  /* [vlh] 4.2, 68010 */
#define MOVESR  043300
#define SRMOVE  040300
#define MOVEUSP 047140
#define CLRVAL  041000

#define CLRFOR  24

    /*relocation values*/
#define ABS    0   /*absolute*/
#define DATA   1
#define TEXT   2
#define BSS    3
#define EXTRN  4   /*externally defined*/

    /* Conditional Assembly variables and constants [vlh] */
#define LOW_CA  21      /* [vlh] */
#define HI_CA   30      /* [vlh] */

    /* Size attribute */
#define BYTE    'b'
#define WORD    'w'
#define LONG    'l'
#define BYTESIZ 1
#define WORDSIZ 2
#define LONGSIZ 4

    /* Ascii values */
#define EOLC    '\n'/*end of line character*/
#define EOF     0   /*end of file indicator*/
#define NULL    0   /* [vlh] character null '\0' */
#define SOH     1

    /* Miscellaneous Defines */
#define TRUE    1   /* [vlh] boolean values */
#define FALSE   0   /* [vlh] boolean values */
#define STDOUT  1   /* file descriptor for standard output */
#define STDERR  2   /* file descriptor for standard error */
#define NAMELEN 8   /* length of name in symbol table */
#define BSIZE   512
#define ITBSZ   256 /*size of the it buffer*/
#define STMAX   200 /*size of intermediate text buffer*/
#define SZIRT   128
#define EXTSZ   512
#define DIRECT  33  /* [vlh] 4.2, number of entries in p2direct */
#define ORGDIR  14  /* [vlh] 4.2, org entry in p2direct */

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

#define ITOP1   4   /*first it entry for operands*/

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

struct it {
    char itty;          /*it type*/
    char itrl;          /*relocation flag or # it entries*/
    long  itop;
};

short mode;             /*operand mode (byte, word, long)*/
short modelen;          /*operand length per mode*/

    /* parameters that define the main table*/
#define SZMT 300        /*initial size of the main table */
                        /*must be large enough to initialize*/
#define ICRSZMT 10      /*add to main table when run out*/
short cszmt;            /*current size of main table*/
char *bmte;             /*beginning of main table*/
char *emte;             /*end of main table*/

short itbuf[ITBSZ];     /*it buffer*/

struct it stbuf[STMAX]; /*holds it for one statement*/
#define STBFSIZE (sizeof stbuf[0])

char sbuf[BSIZE];       /*holds one block of source*/

    /* format of a symbol entry in the main table*/
struct symtab {
    char name[NAMELEN]; /*symbol name*/
    short flags;
    long  vl1;          /*symbol value*/
    char *tlnk;         /*table link*/
} *symtptr;
    /* STESIZE - byte length of symbol table entry -- should be 18 */
    /* must use a sizeof to avoid over run variables */
#define STESIZE (sizeof *symtptr)
char *lmte;             /*last entry in main table */

struct irts {
    char *irle;         /*ptr to last entry in chain*/
    char *irfe;         /*ptr to first entry in chain*/
};

long stlen;             /*length of symbol table*/

    /*initial reference table for symbols*/
char *sirt[SZIRT];
#define SIRTSIZE    (sizeof sirt[0])

    /*initial reference table to opcodes*/
char *oirt[SZIRT];
#define OIRTSIZE    (sizeof oirt[0])

    /*external symbol table*/
char *extbl[EXTSZ];
short extindx;          /*index to external symbol table*/
char **pexti;           /*ptr to external symbol table*/

short absln;            /*absolute line number*/
short p2absln;          /*pass 2 line number*/
short fcflg;            /*0=>passed an item.  1=>first char*/
short fchr;             /*first char in term*/
short ifn;              /*source file descriptor*/
short *pitix;           /*ptr to it buffer*/
short itwc;             /*number of words in it buffer*/
struct it *pitw;        /*ptr to it buffer next entry*/
short itype;            /*type of item*/
long ival;              /*value of item*/
char *lblpt;            /*label pointer*/
char lbt[NAMELEN];      /*holds label name*/
long loctr;             /*location counter*/
long savelc[4];         /*save relocation counters for 3 bases*/
short nite;             /*number of entries in stbuf*/
struct it *pnite;
short lfn;              /*loader output file descriptor*/
char *opcpt;            /*pointer to opcode entry in main table*/
short p2flg;            /*0=>pass 1  1=>pass 2*/
char **pirt;            /*entry in initial reference table*/
short reloc;            /*reloc value returned by expr evaluator (expr)*/
short rlflg;            /*relocation value of current location counter*/
struct hdr couthd;      /* cout header structure */

short format;
short sbuflen;          /*number of chars in sbuf*/
char *psbuf;            /*ptr into sbuf*/
short itfn;             /*it file number*/
char itfnc;             /*last char of it file name*/
short trbfn;            /*temp for text relocation bits*/
char trbfnc;            /*last char of text rb file*/
short dafn;             /*file for data stuff*/
char dafnc;             /*last char of data file*/
short drbfn;            /*file for data relocation bits*/
char drbfnc;            /*last char*/
short prtflg;           /*print output flag*/
short undflg;           /*make undefined symbols external flag*/

short starmul;          /* * is multiply operator*/

    /* Symbol Table Pointers for Subset of Opcodes */
char *endptr, *addptr;
char *orgptr;
char *subptr, *addiptr, *addqptr, *subiptr, *subqptr;
char *cmpptr, *addaptr, *cmpaptr, *subaptr, *cmpmptr;
char *equptr;
char *andptr, *andiptr, *eorptr, *eoriptr, *orptr, *oriptr;
char *cmpiptr;
char *moveptr, *moveqptr;
char *exgptr;
char *evenptr;
char *jsrptr, *bsrptr, *nopptr;

short numcon[2], numsym[2], indir[2], immed[2], numreg[2];
short plevel;           /*parenthesis level counter*/
short opdix;            /*operand index counter*/

    /* ptrs to ins[] and rlbits[]*/
short *pins;
short *prlb;
short ins[5];           /*holds instruction words*/

#define PRTCHLEN 128
char prtchars[PRTCHLEN];/*line buffer for putchar*/
char *prtchidx;         /*index for putchar*/

short extflg, extref;   /*external in expr*/

struct op {
    short ea;           /*effective address bits*/
    short len;          /*effective address length in bytes*/
    long con;           /*constant or reloc part of operand*/
    short drlc;         /*reloc of con*/
    short ext;          /*external variable #*/
    short idx;          /*index register if any*/
    short xmod;         /*mode of index reg*/
} opnd[2];

struct iob {
    int fd;             /* file descriptor */
    int cc;             /* char count */
    char *cp;           /* next char pointer */
    char cbuf[BSIZE];   /* character buffer */
} lbuf, tbuf, dabuf, drbuf;

char tfilname[];
#define LASTCHTFN   tfilname[11]

    /* assembler flag variables */
short didorg;
short shortadr;         /*short addresses if set*/
short initflg;          /*initialize flag*/
short m68010;           /*[vlh] 4.2, 68010 code*/

    /* pass 1 global variables */
short numops;           /*number of operands*/
short inoffset;         /*[vlh]offset directive*/
short p1inlen;          /*pass 1 instr length*/

    /* pass 2 global variables */
short instrlen;         /*pass 2 bytes in current instruction*/
  
    /* General Assembler Variables */
short stdofd;
extern int errno;
char peekc;
short ca_true;          /* true unless in a false CA*/
short ca;               /* depth of conditional assembly, none = 0*/
short ca_level;         /* at what CA depth did CA go false?*/
short nerror;           /*# of assembler errors*/
short in_err;           /*[vlh] don't generate instrlen err if in err state*/
long itoffset;
short equflg;           /*doing an equate stmt*/
short refpc;            /* * referenced in expr*/

    /* defines */
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

#define DBGSTRT()	putchar(0); stdofd = 2
#define DBGEND()	putchar(0); stdofd = 0

    /* Predeclared Functions which return values */
long lseek();
char *sbrk();
char *lemt();

int endit();
int rubout();

int p2gi();
int (*p2direct[])();

    /* Second Pass Subroutines */
int opf1(), opf2(), opf3(), opf4(), opf5(), relbr(), opf7(), opf8();
int opf9(), opf11(), opf12(), opf13(), opf15(), opf17(), opf20();
int opf21(), opf22(), opf23(), opf31();

    /* Directive Handling Subroutines */
int hopd(), hend(), send(), horg(), sorg(), hequ(), hreg();
int hds(), sds(), sdcb();
int hdsect(), hpsect(), sdsect(), spsect();
int hsection(), ssection(), hoffset();
int hent(), hext();
int igrst();
int hbss(), sbss();
int heven(), seven();
int hdc(), sdc(), hdcb();
int hmask2(), hcomline(), hidnt(), httl(), hpage();
int hifeq(), hifne(), hiflt(), hifle(), hifgt(), hifge(), hendc();
int hifnc(), hifc(), hopt();

