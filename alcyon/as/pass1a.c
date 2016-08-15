/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

/* reduce long relative branches to short if possible*/

#include "as68.h"


pass1a()
{
    register long reduced;
    register short writfn, i, wsize;

    pitix = &itbuf[ITBSZ];
    reduced = itoffset = 0L; stbuf[0].itrl = 0;
    wsize = 3*STBFSIZE;     /* [vlh] don't calculate many times */
    close(itfn);
    LASTCHTFN = itfnc;
    itfn = openfi(tfilname,0);  /*open it for reading*/
    writfn = open(tfilname,1);  /*may need to rewrite some of it*/
    if(writfn<0)
        abort();
    while(1) {
        ristb();        /*read it for one statement*/
        opcpt = stbuf[2].itop.ptrw2;    /*ptr to opcode entry in main tab*/
        if(!(opcpt->flags&OPDR)) {  /*not a directive*/
            format = (opcpt->flags)&OPFF;
            p1inlen = stbuf[1].itrl;    /*pass 1 instr length guess*/
            if(((format==6 && p1inlen==4) || opcpt==jsrptr) &&
              (rlflg=stbuf[3].itrl)==TEXT) {
                nite = stbuf[0].itrl & 0377;/* # of it entries */
                pnite = &stbuf[nite].itty;  /*ptr to end of stmt*/
                modelen = stbuf[2].itrl;    /*instr mode length*/
                opdix = ITOP1;              /*first operand*/
                pitw = &stbuf[ITOP1].itty;  /*ptr to first operand*/
                loctr = stbuf[3].itop - reduced;
                expr(&p2gi);
                ival -= loctr+2L;
                if(itype==ITCN && !extflg && reloc!=ABS) {
                    if(format==9) {     /*jsr*/
                        i = (ival>= -128 && ival<=127) ? p1inlen-2 :
                            (ival>= -32768 && ival<=32767) ? p1inlen-4 : 0;
                        if (!i)
                            continue;
                        stbuf[2].itop.ptrw2 = bsrptr;   /*chng to bsr*/
                    }
                    else if(ival>= -128 && ival<=127) {
                        i = 2;      /*[vlh]was 4 for ival=2*/
                        if (!ival) stbuf[2].itop.ptrw2 = nopptr;
                    }
                    else
                        continue;
                    fixsyadr(i);
                    reduced += i;
                    stbuf[1].itrl -= i;     /*reduced somewhat*/
                    if(!stbuf[1].itrl)
                        stbuf[1].itrl = -1; /*ignore flag*/
                    if(lseek(writfn,itoffset,0) == -1L) {
                        rpterr("seek error on intermediate file\n");
                        abort();
                    }
                    if(write(writfn,&stbuf[0],wsize) != wsize) {
                        rpterr("write error on it file\n");
                        abort();
                    }
                }
            }
        }
        else if(opcpt == endptr) {
            savelc[TEXT] -= reduced;
            close(writfn);
            return;
        }
    }
}

/* fix all symbol addresses that are text based and greater than loctr*/
/*   fix means subtract 2 from them*/
fixsyadr(al)
{
    register char **sx1, **sx2;
    register short l;

    l = al;
/* loop thru symbol initial reference table*/
    for(sx1 = sirt; sx1 < &sirt[SZIRT-1]; sx1 += 2) {
        if(*(sx2 = sx1+1)==0)       /* this chain is empty*/
            continue;

/* symbols on one chain*/
        sx2 = *sx2; /*first entry on this chain*/
        while(1) {
            if((sx2->flags&SYDF || sx2->flags&SYPC) && sx2->flags&SYRO &&
                sx2->vl1 > loctr)       /* [vlh] */
                sx2->vl1 -= l;
            if(sx2 == *sx1) /*end of chain*/
                break;
            sx2 = sx2->tlnk;    /*next entry in chain*/
        }
    }
}
