/*
    Copyright 1982
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

/*
 * intermediate code operators
 * 0=>EOF, special operator
 */
#define CEOF     0

/* 1-59=>operators that generate code (entries in code gen optab) */
#define ADD     1
#define SUB     2
#define MULT    3
#define DIV     4
#define MOD     5
#define RSH     6
#define LSH     7
#define AND     8
#define OR      9
#define XOR     10
#define NOT     11
#define UMINUS  12
#define COMPL   13
#define PREDEC  14
#define PREINC  15
#define POSTDEC 16
#define POSTINC 17
#define ASSIGN  18
#define EQADD   19
#define EQSUB   20
#define EQMULT  21
#define EQDIV   22
#define EQMOD   23
#define EQRSH   24
#define EQLSH   25
#define EQAND   26
#define EQOR    27
#define EQXOR   28
#define FJSR    29
#define EQUALS  30
#define NEQUALS 31
#define GREAT   32
#define GREATEQ 33
#define LESS    34
#define LESSEQ  35
#define INT2L   36
#define LONG2I  37

/* machine dependent operators that generate code */
#define BTST    38
#define LOAD    39
#define LMULT   40
#define LDIV    41
#define LMOD    42
#define LEQMULT 43
#define LEQDIV  44
#define LEQMOD  45
#define EQADDR  46
#define EQNOT   47
#define EQNEG   48
#define DOCAST  49

#define STASSIGN    50
#define LONG2F  51
#define FLOAT2L 52
#define INT2F   53
#define FLOAT2I 54
#define TOCHAR  55
#define LCGENOP 56      /* change if adding more operators... */

/* intermediate code operators that do not generate code */
#define ADDR    60
#define INDR    61
#define LAND    62
#define LOR     63
#define QMARK   64
#define COLON   65
#define COMMA   66
#define CINT    67
#define CLONG   68
#define SYMBOL  69
#define AUTOINC 70
#define AUTODEC 71
#define CALL    72
#define NACALL  73
#define BFIELD  74
#define IFGOTO  75
#define INIT    76
#define CFORREG 77
#define DCLONG  78
#define CFLOAT  79

/* operators local to parser */
#define CAST    80
#define SEMI    81
#define LCURBR  82
#define RCURBR  83
#define LBRACK  84
#define RBRACK  85
#define LPAREN  86
#define RPAREN  87
#define STRING  88
#define RESWORD 89
#define APTR    90
#define PERIOD  91
#define SIZEOF  92
#define MPARENS 93
#define FRETURN 94
#define ASM     95
#define STACKEND    100

/* data types */
#define TYPELESS     0
#define CHAR         1
#define SHORT        2
#define INT          3
#define LONG         4
#define UCHAR        5
#define USHORT       6
#define UNSIGNED     7
#define ULONG        8
#define FLOAT        9
#define DOUBLE      10
#define SIZE_T UNSIGNED

/* data types local to parser */
#define STRUCT      11
#define FRSTRUCT    12
#define LLABEL      13

/* type flags and definitions */
#define TYPE        0x0f
#define SUPTYP      0x30
#define ALLTYPE     0x3f
#define POINTER     0x10
#define FUNCTION    0x20
#define ARRAY       0x30
#define SUTYPLEN    2

/* data registers */
#define DREG0   0
#define DREG2   2
#define DREG3   3
#define DREG4   4
#define DREG5   5
#define DREG6   6
#define DREG7   7
#define AREG3   11
#define AREG4   12
#define AREG5   13

/* storage classes */
#define AUTO        1
#define REGISTER    2
#define EXTERNAL    3
#define STATIC      4
#define REGOFF      5
#define EXTOFF      6
#define STATOFF     7
#define INDEXED     8

/* exclusively code generator storage classes */
#define CINDR       9
#define CLINDR      10
#define CFINDR      11

/* exclusively parser storage classes */
#define STRPROTO    9
#define PDECLIST    10
#define PARMLIST    11
#define BFIELDCL    12
#define UNELCL      13
#define STELCL      14
#define PDECREG		15	/* PDECLIST register variable */
#define DEXTERN     16  /* explicitly defined extern */

/* opinfo table bits */
#define OPPRI       077
#define OPBIN       0100
#define OPLVAL      0200
#define OPREL       0400
#define OPASSIGN    01000
#define OPLWORD     02000
#define OPRWORD     04000
#define OPCOM       010000
#define OPRAS       020000
#define OPTERM      040000
#define OPCONVS     0100000

/* 68000 definitions */
#define PTRSIZE     4
#define LONGSIZE    4
#define INTSIZE     2
#define CHARSIZE    1
#define SSIZE       8               /* chars per symbol */
#define TRUE        1
#define FALSE       0
#define TABC        '\t'            /* tab character */
#define EOLC        '\n'            /* end of line character */
#define BITSPBYTE   8

/* operator class priorities */
#define TRMPRI      0               /* terminal nodes */
#define RPNPRI      1               /* ) and ] */
#define CALPRI      2               /* in-stack call, ( or [ */
#define COLPRI      3               /* init or case priority for : or , */
#define STKPRI      4               /* priority of end of stack */
#define COMPRI      5               /* normal priority for , */
#define ASGPRI      6               /* =, +=, -=, *=, /=, %=, ... */
#define QMKPRI      7               /* ?: */
#define LORPRI      8               /* || */
#define LNDPRI      9               /* && */
#define ORPRI       10              /* |, ^ */
#define ANDPRI      11              /* & */
#define EQLPRI      12              /* ==, != */
#define RELPRI      13              /* >, <, >=, <= */
#define SHFPRI      14              /* <<, >> */
#define ADDPRI      15              /* +, - */
#define MULPRI      16              /* *, /, % */
#define UNOPRI      17              /* !, ++, --, &, *, -, ~, sizeof */
#define LPNPRI      18              /* ., ->, [, (, function call */
#define PSTPRI      19              /* in-stack post--, post++ */

extern short inittype;
