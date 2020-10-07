/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/


adirect const p1direct[DIRECT] = {
    hopd,       /*  0 */
    hend,       /*  1 */
    hdsect,     /*  2 */
    hpsect,     /*  3 */
    hequ,       /*  4 */
    hequ,       /*  5  .set same as .equ */
    0,          /*  6 */
    hascii,     /*  7 */
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
    hpage       /* 33 */
};

adirect const p2direct[DIRECT] = {
    0,          /*  0 */
    send,       /*  1 */
    sdsect,     /*  2 */
    spsect,     /*  3 */
    0,          /*  4 */
    0,          /*  5 */
    0,          /*  6 */
    sascii,     /*  7 */
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
    spage       /* 33 */
};
