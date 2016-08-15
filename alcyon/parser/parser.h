/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
 */

/**
 *  C68 Parser - include file
**/

#ifdef DRI
#	include <stdio.h>
#	include <klib.h>
#	undef putchar
#	define putchar xputchar
#	undef ferror
#	define ferror xferror
#	define printf xprintf
#endif

#include "../icode.h"

	/*symbol attribute fields*/
#define SRESWORD    001         /*is symbol a reserved word?*/
#define SGLOBAL     002         /*is symbol global?*/
#define STYPEDEF    004         /*typedef declaration?*/
#define SDEFINED    010         /*symbol defined?*/

    /*reserved words*/
#define R_AUTO      1
#define R_BREAK     2
#define R_CASE      3
#define R_CHAR      4
#define R_CONTINUE  5
#define R_DO        6
#define R_DEFAULT   7
#define R_DOUBLE    8
#define R_GOTO      9
#define R_ELSE      10
#define R_EXTERNAL  11
#define R_FLOAT     12
#define R_FOR       13
#define R_IF        14
#define R_INT       15
#define R_LONG      16
#define R_REGISTER  17
#define R_RETURN    18
#define R_SHORT     19
#define R_SIZEOF    20
#define R_STATIC    21
#define R_STRUCT    22
#define R_SWITCH    23
#define R_TYPEDEF   24
#define R_UNION     25
#define R_UNSIGNED  26
#define R_WHILE     27
#define R_ASM       28

    /*
     * mixed-mode conversions, entries in 2-d array indexed by:
     * (int,unsn,long,doub,ptr)
     */
#define INT_CHAR        1
#define UNSN_CHAR       1
#define LONG_CHAR       1
#define DOUB_CHAR       1
#define PTR_CHAR        1
#define INT_UNSN        0       /*no conversion is generated*/
#define INT_LONG        2
#define INT_DOUB        3
#define INT_PTR         4
#define UNSN_INT        0       /*no conversion is generated*/
#define UNSN_LONG       6
#define UNSN_DOUB       7
#define UNSN_PTR        8
#define LONG_INT        9
#define LONG_UNSN       10
#define LONG_DOUB       11
#define LONG_PTR        12
#define DOUB_INT        13
#define DOUB_UNSN       14
#define DOUB_LONG       15
#define PTR_INT         16
#define PTR_UNSN        17
#define PTR_LONG        18
#define PTR_PTR         19
#define BADCONV         20

    /* miscellaneous constants */
#define OPSSIZE     40          /*operator stack size*/
#define OPDSIZE     80          /*operand stack size*/
#define HSIZE       512         /*hash table size, 3.4 made prime */
#define BSIZE       512         /*io buffer size */
#define SYMSIZE     1024        /*size to alloc for symbol structures*/
#define SWSIZE      256         /*max no. of cases in a switch*/
#define DSIZE       1000        /*dimension table size*/
#define BITSPWORD   16          /*bits per word*/
#define AREGLO      010         /*A reg flag*/
#define DREG        0100        /*data loadable into D-register?*/
#define HICREG      2           /*highest reg # used for code gen*/
#define BITSPCHAR   8           /*bits per char*/
#define CHRSPWORD   2           /*chars per word*/
#define STRSIZE     300         /*max string length*/
#define NFARGS      40          /*max no. of args to function*/
#define NFRSTR      128         /*max no. of forward ref struct proto*/
#define PATHSIZE    128
#define SCOPE_LEVLS 50          /* [vlh] 4.2, # of scope levels */
#define GLOB_SCOPE  0           /* [vlh] 4.2, global level is 0 */
#define FUNC_SCOPE  1           /* [vlh] 4.2, function level is 1 */
#define STDERR		2			/* [vlh] 4.2, error ==> STDERR... */

    /*symbol table node*/
struct symbol {
    char s_attrib;          /* defined, resword, global, typedef */
    char s_sc;              /* auto, static, external, register */
    short s_type;           /* 4bits specified, 2 bit fields for ptr */
    short s_dp;             /* index into dimension table */
    short s_ssp;            /* dimension table/function arg table */
    short s_offset;         /* offset inside of structure */
    short s_scope;          /* [vlh] 4.2 scope of symbol */
    char s_symbol[SSIZE];   /* symbol identifier, to SSIZE chars */
    struct symbol *s_par;   /* if struct, ptr to parent (sys III) */
    struct symbol *s_child; /* if struct, ptr to 1st child (sys III) */
    struct symbol *s_sib;   /* if struct, ptr to sibling (sys III) */
    struct symbol *s_next;  /* next symbol table entry */
};

    /*expression tree operator node*/
struct tnode {
    short t_op;
    short t_type;
    short t_dp;
    short t_ssp;
    struct tnode *t_left;
    struct tnode *t_right;
};

    /*expression tree node for symbol - only keeps location*/
struct symnode {
    short t_op;
    short t_type;                 /*data type of symbol*/
    short t_dp;                   /*dimension pointer of symbol*/
    short t_ssp;                  /*structure size index to dtab*/
    short t_sc;                   /*storage class of symbol*/
    short t_offset;               /*offset of symbol*/
    short t_label;
};

    /*expressioon tree node for external symbol - need to keep name*/
struct extnode {
    short t_op;
    short t_type;
    short t_dp;
    short t_ssp;
    short t_sc;
    short t_offset;
    short t_reg;
    short t_symbol[SSIZE];        /*symbol name*/
};

    /*expression tree node for integer constant*/
struct conode {
    short t_op;
    short t_type;
    short t_dp;
    short t_ssp;
    short t_value;                /*constant value*/
};

    /* long constant node structure */
struct lconode {
    short t_op;
    short t_type;
    short t_dp;
    short t_ssp;
    long t_lvalue;              /*constant value*/
};

    /* key word table */
struct resword {
    char *r_name;
    int r_value;
} reswords[];

    /* switch table structure */
struct swtch {
#ifndef VAX11
    short sw_label;
    short sw_value;
#else
    short sw_label;
    short sw_value;
#endif
} swtab[SWSIZE];

    /*operator and operand stack used by expr*/
struct ops {                    /*operator stack*/
    short o_op;                 /*operator*/
    short o_pri;                /*priority*/
} opstack[OPSSIZE], *opp;

    /* Miscellaneous Variables for declarations */
short scope_decls[SCOPE_LEVLS]; /*[vlh] 4.2 decls at this scope ?? */
short scope_level;              /*[vlh] 4.2, global=0, func=1 */
short indecl;                   /*[vlh] 4.2, are we in a decl ?? */
short predecl;					/*[vlh] 4.2, value previous to sizeof */	
short tdflag;                   /*declaration is a typedef proto*/
struct symbol *tdp;             /*points to typedef prototype*/
short localsize;                /*length of local variables*/
short naregs;                   /*keeps track of ptr registers alloc'd*/
short ndregs;                   /*keep track of data registers alloc'd*/
short boffset;                  /*current bit offset in structure*/
short in_struct;                /*set when in structure declaration*/

    /* Miscellaneous Variables for expression handling */
short opdotsave;                /* vars used by the expression evaluator */
short opdontop;                 /*op on top of expr stack ?? */
short strassign;
char *opdsave, *oprsave;
char *opdstack[OPDSIZE];        /*operand stack*/
char **opdp;                    /*operand stack pointer*/
char *opap;                     /*ptr to next avail loc in exprarea*/
short opinfo[];                 /*operator info table*/
short commastop;                /*stop parse at comma*/
short colonstop;                /*stop parse at colon*/

    /* Miscellaneous Variables for statement generation */
short cswp;                     /*current low switch table index*/
short clabel;                   /*continue label*/
short blabel;                   /*break label*/
short rlabel;                   /*return label*/
short dlabel;                   /*default label*/
    
    /* Miscellaneous Variables */
short lineno;                   /*current line number of input*/
short cr_last;                  /* determine if # is file specification */
short errcnt;                   /*count of errors*/
char source[PATHSIZE];          /*[vlh]source filename for err rpting*/
short strassign;
struct tnode *frp;              /*pointer to function return info node*/
short smember;                  /*set when seen . or ->*/
short instmt;                   /*[vlh] 4.1 in a stmt*/
short infunc;                   /*set when in function body*/
short reducep;                  /*[vlh] if(procid); reduction*/
short peektok;                  /*peeked at token*/

    /* Parser Variables which are initialized in parser.ext */
extern char *exprp;             /*place to start building expression*/
extern short swp;               /*current entry in switch table*/
extern short nextlabel;         /*generates unique label numbers*/
extern short structlabel;       /*generates unique label names*/
extern char dinfo[];
extern char aregtab[];
extern char dregtab[];

    /* Parser flags */
short eflag;                    /*[vlh] 3.4 IEEE floats */
short fflag;                    /*[vlh] 3.4 FFP floats */
short gflag;                    /*[vlh] 4.2 symbolic debugger flag */
short xflag;                    /*translate int's to long's*/
short tflag;                    /*[vlh] 4.1, put strings into text seg*/
short wflag;                    /*[vlh] don't generate warning mesgs*/
short debug;                    /*[vlh] 4.1, debug flag */
short symdebug;                 /*[vlh] 4.2, sym debug flag */
short treedebug;                /*[vlh] 4.2, expr tree debug flag */

    /*dimension table*/
long dtab[DSIZE];               /* [vlh] 3.4 short => long */
short cdp;                      /*next entry in dtab to alloc*/

    /*lexical analyzer values*/
short cvalue;                   /*current token if keyword or CINT*/
short ccbytes;                  /*number of bytes in char constant*/
short cstrsize;                 /*current string size*/
long clvalue;                   /*current token value if long constant*/
struct symbol *csp;             /*current token symbol ptr if SYMBOL*/
char cstr[STRSIZE];             /*current token value if CSTRING*/
struct symbol *dsp;             /*declarator symbol pointer*/

    /* 0 no structure */
struct symbol *struc_parent[10];/*[vlh] ptrs to struc symbols*/
struct symbol *struc_sib[10];   /*[vlh] ptrs to struc symbols*/
struct symbol *hold_sib;        /*[vlh] wrap sib past struct decl*/

    /*function argument table, used to collect function parameters*/
struct farg {
    struct symbol *f_sp;
    short f_offset;
} fargtab[NFARGS];

    /*forward referenced structure prototype names*/
struct symbol *frstab[NFRSTR];
short frstp;

    /*output buffers for intermediate code and strings*/
struct iob {
    int fd;
    int cc;
    char *cp;
    char cbuf[BSIZE];
} obuf, lbuf, sbuf, ibuf, *obp;

    /* Macro's used by the Parser */
#define ISTYPEDEF(sp)       (sp->s_attrib&STYPEDEF)
#define WALIGN(add)         ((add+1)&(~1))
#define ISARRAY(type)       ((type&SUPTYP)==ARRAY)
#define ISFUNCTION(type)    ((type&SUPTYP)==FUNCTION)
#define ISPOINTER(type)     ((type&SUPTYP)==POINTER)
#define NOTARRAY(type)      ((type&SUPTYP)!=ARRAY)
#define NOTFUNCTION(type)   ((type&SUPTYP)!=FUNCTION)
#define NOTPOINTER(type)    ((type&SUPTYP)!=POINTER)
#define BTYPE(type)         (type&TYPE)
#define SUPTYPE(type)       (type&SUPTYP)
#define ISALLTYPE(type)     (type&(SUPTYP|TYPE))
#define ASGNOP(op)          ((opinfo[op]&OPASSIGN)!=0)
#define RELOP(op)           ((opinfo[op]&OPREL)!=0)
#define COMOP(op)           ((opinfo[op]&OPCOM)!=0)
#define LINTEGRAL(op)       ((opinfo[op]&OPLWORD)!=0)
#define RINTEGRAL(op)       ((opinfo[op]&OPRWORD)!=0)
#define RASOP(op)           ((opinfo[op]&OPRAS)!=0)
#define BINOP(op)           ((opinfo[op]&OPBIN)!=0)
#define UNARYOP(op)         ((opinfo[op]&OPBIN)==0)
#define LEAF(op)            ((opinfo[op]&OPTERM)!=0)
#define LVALOP(op)          ((opinfo[op]&OPLVAL)!=0)
#define OPPRIORITY(op)      (opinfo[op]&OPPRI)
#define SIMPLE_TYP(typ)     ((typ>=CHAR) && (typ<=DOUBLE))

    /* checks for symbol with structure element storage class */
#define ISSTEL(tp)  (tp->t_op==SYMBOL && (SESC(tp)))
#define SESC(x)     (x->t_sc==STELCL||x->t_sc==UNELCL||x->t_sc==BFIELDCL)

    /* peek at next token, if not read token put back, else delete */
    /* 1 if matched, 0 otherwise */
#define PEEK(tok)   ( (peektok=gettok(0)) == tok )

    /* outbentry - output symbol '%', signifying routine entry, for link info */
#define OUTBENTRY() printf("%%\n")
#define OUTCOMMON(sym,size) printf("\t.comm _%.8s,%ld\n",sym,size)
#define OUTGOTO(lab)        if( lab > 0 ) printf("\tbra L%d\n",lab)
    /* change to text segment */
#define OUTTEXT()       printf("\t.text\n")
    /* change segment to bss */
#define OUTBSS()        printf("\t.bss\n")
    /* get on a word boundary */
#define OUTEVEN()       printf("\t.even\n")
    /* output current line number into icode */
#define OUTLINE() if(!bol) putchar('\n'); printf(".%x.%s\n",lineno,source)
    /* output global symbol references */
#define OUTEXTDEF(sym)  printf("\t.globl _%.8s\n",sym)
    /* outputs reserved memory [vlh] 3.4 %d => %ld */
#define OUTRESMEM(size) printf("\t.ds.b %ld\n",size)
    /* output padding for word alignments */
#define OUTPAD()        printf("\t.even\n")
    /* output long constant to assembler */
#define OUTLCON(val)            printf("\t.dc.l %ld\n",val)
    /* output label constant */
#define OUTCLAB(lab)            printf("\t.dc.l L%d\n",lab)
    /* output a label */
#define OUTLAB(lab)             printf("\tL%d:",lab)
    /* output function label */
#define OUTFLAB(sym)            printf("\t_%.8s:\n\t~~%.8s:\n",sym,sym)
    /* output data label */
#define OUTDLAB(sym)            printf("\t_%.8s:\n",sym)
	/* output a null tree */
#define OUTNULL()				printf("0\n")
#ifdef DEBUG
#   define PUTEXPR(dbg,id_str,node_ptr) if (dbg) putexpr(id_str,node_ptr)
#else
#   define PUTEXPR(dbg,id_str,node_ptr) 
#endif

    /*functions returning pointers*/
char *expr();
char *talloc();
char *tnalloc();
char *enalloc();
char *snalloc();
char *cnalloc();
char *lcnalloc();
char *fpcnalloc();
char *popopd();
char *cvopgen();
char *arrayref();
char *funcref();
char *install();
char *lookup();
char *balpar();
char *get_symbol();     /* [vlh] 4.2 split from expr() */
char *sbrk();
char *mktemp();

long initlist();
long dsize();
long psize();
long dodecl();
long dlist();
long getdec();
long gethex();
long getoct();
long getfp();
long toieee();
long toffp();
long cexpr();
long s_or_a();
long str_init();
long outstr();
