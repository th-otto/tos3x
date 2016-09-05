/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/


const char *const ermsg[] = {
    "label redefined",                  /*  1 */
    "invalid label",                    /*  2 */
    "invalid opcode",                   /*  3 */
    "no label for operand",             /*  4 */
    "opcode redefined",                 /*  5 */
    "illegal expr",                     /*  6 */
    "undefined symbol in equate",       /*  7 */
    "opcode for 68010 only",			/*  8 */
    "invalid first operand",            /*  9 */
    "invalid second operand",           /* 10 */
    "absolute value required",          /* 11 */
    "no code or data allowed in offset",/* 12 */
    "undefined symbol",                 /* 13 */
    "illegal index register",           /* 14 */
    "illegal constant",                 /* 15 */
    "illegal extension",                /* 16 */
    "constant required",                /* 17 */
    "illegal format",                   /* 18 */
    "illegal string",                   /* 19 */
    "illegal addressing mode",          /* 20 */
    "assembler confusion...",			/* 21 */
    "illegal relative address",         /* 22 */
    "invalid bit range",                /* 23 */
    "illegal text delimiter",           /* 24 */
    "unexpected endc",                  /* 25 */
    "endc expected",                    /* 26 */
    "relocation error",                 /* 27 */
    "symbol required",                  /* 28 */
    "bad use of symbol",                /* 29 */
    "invalid data list",                /* 30 */
    "warning: cmpm generated for 68010",/* 31 */
    "missing )",                        /* 32 */
    "register required",                /* 33 */
    "illegal size",                     /* 34 */
    "illegal 8-bit displacement",       /* 35 */
    "illegal external",                 /* 36 */
    "illegal shift count",              /* 37 */
    "invalid instruction length",       /* 38 */
    "code or data not allowed in bss",  /* 39 */
    "backward assignment to *",         /* 40 */
    "illegal 16-bit displacement",      /* 41 */
    "illegal 16-bit immediate",         /* 42 */
    "illegal 8-bit immediate",          /* 43 */
    0
};


char ldfn[40];          /* name of the relocatable object file */

int brkln1 = 077777;    /* pass 1 break line number for debugging */
int opcval;             /* opcode */
int chmvq;

adirect const p1direct[] = {
    hopd,       /*  0 */
    hend,       /*  1 */
    hdsect,     /*  2 */
    hpsect,     /*  3 */
    hequ,       /*  4 */
    hequ,       /*  5  .set same as .equ */
    0,          /*  6 */
    0,          /*  7 */
    hdc,        /*  8 */
    hent,       /*  9 */
    hext,       /* 10 */
    hbss,       /* 11 */
    hds,        /* 12 */
    heven,      /* 13 */
    horg,       /* 14 */
    hmask2,     /* 15 */
    hreg,       /* 16 */
    hdcb,       /* 17 */
    hcomline,   /* 18 */
    hidnt,      /* 19 */
    hoffset,    /* 20 */
    hsection,   /* 21 */
    hifeq,      /* 22 */
    hifne,      /* 23 */
    hiflt,      /* 24 */
    hifle,      /* 25 */
    hifgt,      /* 26 */
    hifge,      /* 27 */
    hendc,      /* 28 */
    hifc,       /* 29 */
    hifnc,      /* 30 */
    hopt,       /* 31 */
    httl,       /* 32 */
    hpage,      /* 33 */
    0
};

adirect const p2direct[] = {
    0,          /*  0 */
    send,       /*  1 */
    sdsect,     /*  2 */
    spsect,     /*  3 */
    0,          /*  4 */
    0,          /*  5 */
    0,          /*  6 */
    0,          /*  7 */
    sdc,        /*  8 */
    0,          /*  9 */
    0,          /* 10 */
    sbss,       /* 11 */
    sds,        /* 12 */
    seven,      /* 13 */
    sorg,       /* 14 */
    0,          /* 15 */
    0,          /* 16 */
    sdcb,       /* 17 */
    sds,        /* 18  comline same as .ds.b */
    0,          /* 19 */
    0,          /* 20 */
    ssection,   /* 21 */
    0,          /* 22 */
    0,          /* 23 */
    0,          /* 24 */
    0,          /* 25 */
    0,          /* 26 */
    0,          /* 27 */
    0,          /* 28 */
    0,          /* 29 */
    0,          /* 30 */
    0,          /* 31 */
    0,          /* 32 */
    spage,      /* 33 */
    0
};

int symcon;
char const endstr[] = "end";
char const equstr[] = "equ";
char const evnstr[] = "even";
char const orgstr1[] = "~.yxzorg";
char const orgstr2[] = "org";

int poslab;
char tlab1[SYNAMLEN];
int explmode;   /* explicit mode length given */

