/*
    Copyright 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

char *version = "@(#)as68 assembler 4.2 - Sep 6, 1983";

/*
 * a two pass relocatable assembler for the Motorola 68000 microprocessor
 *
 *  Bill Allen
 *  Modified by Vicki Hutchison
 *
 *  after any of this assembler is recompiled, it must be initialized
 *  before it will execute properly.  To initialize, become super user
 *  and execute the command:
 *
 *      as68 -i as68init
 *
 *  where as68 is the newly compiled version of the assembler.  With-
 *  out this initialization, the assembler will not run (probably bus
 *  error).
 */

#include "as68.h"
#include "def.h"
#include <signal.h>

#define INIT(op,ptr) pack(op,lmte); ptr=lemt(TRUE,oirt)

main(argc,argv)
int argc;
char **argv;
{
    register short i, ttmp;
    register long longtmp;

    nerror = initflg = 0;
    prtchidx = prtchars;
    signal(SIGINT,rubout);
    pitix = itbuf;
    pexti = extbl;
    ttmp = (STESIZE*SZMT) + 2;
    bmte = sbrk(ttmp);
    longtmp = bmte; /* 11 apr 83, for vax */
    if(longtmp&1L)  /* 11 apr 83, for vax */
        bmte++;     /*make it even*/
    emte = bmte + ttmp - 2;     /*end of main table*/
    if(argc<=1) 
		usage();
    i = 1;
    shortadr = 0;  /*long addresses...*/
    while(argv[i][0] == '-') {      /*may be print or initialize*/
        switch(argv[i++][1]) {

        case 'a':       /*[vlh] 4.2, short addresses only*/
            shortadr = -1;
            break;

        case 'i':       /*initialize the assembler*/
            initflg++;
            break;

        case 'p':       /*produce a listing*/
            prtflg++;
            break;

        case 'u':       /*make undefined symbols external*/
            undflg++;
            break;

        case 'N':       /*no branch optimization*/
            didorg++;
            break;

        case 'L':       /*4.2 OBSOLETE, long addresses only*/
            shortadr = 0;
            break;

        case 'T':       /*generating code suitable for the 68010*/
            m68010++;
            break;

        default:
            usage();
        }
    }
    if(i>=argc) 
		usage();
    ifn=openfi(argv[i],0);  /*open source file*/
    setldfn(argv[i]);   /*create relocatable object file name*/
    lfn=openfi(ldfn,1); /*open loader file*/
    itfn = gettempf();  /*get a temp file for it*/
    itfnc = LASTCHTFN;  /*remember last char for unlink*/
    trbfn = gettempf(); /*temp for text relocation bits*/
    trbfnc = LASTCHTFN;
    dafn = gettempf();  /*temp for data binary*/
    dafnc = LASTCHTFN;
    drbfn = gettempf(); /*temp for data relocation bits*/
    drbfnc = LASTCHTFN;
    if(initflg) {       /*initializing te main table*/
        lmte=bmte;      /*beginning main table*/
        cszmt = SZMT;   /*current size of main table*/
        for(i = 0; i <= SZIRT-2; i += 2) {
            sirt[i] = &sirt[i];     /*initialize the initial ref tables*/
            sirt[i+1] = 0;
            oirt[i] = &oirt[i];
            oirt[i+1] = 0;
        }

/*make entries in main table for directives*/
        mdemt("opd",0);         /*opcode definition*/
        mdemt(endstr,1);        /*end statement*/
        mdemt("data",2);        /*dsect directive(code DATA based)*/
        mdemt("text",3);        /*psect directive(code TEXT based)*/
        mdemt(equstr,4);        /*equate*/
        mdemt("set",5);         /*.set - same as .equ*/
        mdemt("dc",8);          /*define byte*/
        mdemt("globl",9);       /*define global (public) symbols*/
        mdemt("xdef",9);        /*[vlh]define global (public) symbols*/
        mdemt("xref",9);        /*[vlh]define global (public) symbols*/
        mdemt("comm",10);       /*define external symbols*/
        mdemt("bss",11);        /*block storage based*/
        mdemt("ds",12);         /*block storage based*/
        mdemt(evnstr,13);       /*round pc*/
        mdemt(orgstr1,14);      /*[vlh] internal, *= */
        mdemt(orgstr2,14);      /*[vlh] org location, also *= */
        mdemt("mask2",15);      /*[vlh] assemble for mask2, ignore*/
        mdemt("reg",16);        /*[vlh] register equate*/
        mdemt("dcb",17);        /*[vlh] define block*/
        mdemt("comline",18);    /*[vlh] command line*/
        mdemt("idnt",19);       /*[vlh] relocateable id record, ignore*/
        mdemt("offset",20);     /*[vlh] define offsets*/
        mdemt("section",21);    /*[vlh] define sections*/
        mdemt("ifeq",22);       /*[vlh] ca if expr = 0*/
        mdemt("ifne",23);       /*[vlh] ca if expr != 0*/
        mdemt("iflt",24);       /*[vlh] ca if expr < 0*/
        mdemt("ifle",25);       /*[vlh] ca if expr <= 0*/
        mdemt("ifgt",26);       /*[vlh] ca if expr > 0*/
        mdemt("ifge",27);       /*[vlh] ca if expr >= 0*/
        mdemt("endc",28);       /*[vlh] end ca*/
        mdemt("ifc",29);        /*[vlh] ca if string compare*/
        mdemt("ifnc",30);       /*[vlh] ca if not string compare*/
        mdemt("opt",31);        /*[vlh] ignored, assemb options*/
        mdemt("ttl",32);        /*[vlh] ttl define, ignore*/
        mdemt("page",33);       /*[vlh] page define, ignore*/

    }
    else  					/*not initializing*/
        getsymtab();        /*read initialized main table*/

    rlflg = TEXT;           /*code initially TEXT based*/
    inoffset = 0;           /*[vlh]not in offset mode*/
    loctr = 0;              /*no generated code*/
    ca = 0;                 /*[vlh]depth of conditional assembly*/
    extindx = 0;            /*no external symbols yet*/
    p2flg = 0;              /*pass 1*/
    ca_true = 1;            /*[vlh]true unless in side false case*/
    absln = 1;
    sbuflen = -1;           /*no source yet*/
    fchr = gchr();          /*get first char*/
    if(!initflg) {      	/*not initializing*/
        INIT(orgstr2,orgptr);
        INIT(endstr,endptr);
        INIT(equstr,equptr);
        INIT("add",addptr);
        INIT("addi",addiptr);
        INIT("addq",addqptr);
        INIT("sub",subptr);
        INIT("subi",subiptr);
        INIT("subq",subqptr);
        INIT("cmp",cmpptr);
        INIT("adda",addaptr);
        INIT("cmpa",cmpaptr);
        INIT("suba",subaptr);
        INIT("cmpm",cmpmptr);
        INIT("and",andptr);
        INIT("andi",andiptr);
        INIT("or",orptr);
        INIT("ori",oriptr);
        INIT("cmpi",cmpiptr);
        INIT("eor",eorptr);
        INIT("eori",eoriptr);
        INIT("move",moveptr);
        INIT("moveq",moveqptr);
        INIT("exg",exgptr);
        INIT("jsr",jsrptr);
        INIT("bsr",bsrptr);
        INIT("nop",nopptr);
        INIT(evnstr,evenptr);
    }
    mloop();
}

usage()
{
    rpterr("Usage: as68 [-p] [-u] [-L] [-N] sourcefile\n");
    endit();
}


/*main loop*/
mloop()
{
    register short i;

    while(fchr!=EOF) {
        if(absln>=brkln1)   /*break for debugging the assembler*/
            i=0;
        fcflg = 0;          /*first time thru expr pass one*/
        cisit();            /*create it for one statement*/
    }
    opcpt = endptr;
    hend();
}

#define NOCODE ((i>=0&&i<6)||i==9||i==11||i==12||i==16||(i>=20&&i<=30))
/* cond-directives, section, ds, set, equ, reg, globl, end, offset */

/*create intermediate text (it) for one statement*/
/*  call with first character of statement in fchr*/
cisit()
{
    register short *p1,*p2;
    register short (*dirop)();
    register short i, col1;   /*[vlh] col1 labels in col 1...*/
    char str[NAMELEN], *l;

ciss1:
    immed[0] = immed[1] = indir[0] = indir[1] = numcon[0] = 0;
    numcon[1] = numsym[0] = numsym[1] = numreg[0] = numreg[1]=0;
    plevel = numops = opdix = explmode = 0;
cistop:
    col1 = 1;
    if(fchr==EOLC) {
        fchr = gchr();
        goto cistop;
    }
    if(fchr==' ') {
        col1 = 0;
        igblk();
        if(fchr==EOLC)      /*blank line*/
            goto cistop;
        peekc = fchr;
        if (fchr != EOF) 
			fchr = ' ';    /* [vlh] catch eof... */
    }
    if(fchr==EOF) 
		return;
    if(fchr=='*') {         /*ignore comments*/
        fchr = gchr();
        if(fchr=='=') {     /*relocation counter assignment*/
            fchr = gchr();  /*pass the =*/
            horg();         /*output constants if not bss*/
        }
        igrst();
        fcflg = 0;      /*clear expr first time flag for next stmt*/
        goto ciss1;
    }

/* get the opcode and label*/

    mode = 'w';         /*word mode*/
    igblk();            /*ignore blanks*/
    poslab = 1;
    gterm(TRUE);
    poslab = 0;
    if(fchr==':' || fchr=='=') {            /*there is a label*/
label:
        col1 = 0;
        if(itype!=ITSY) {         /*not a symbol*/
            uerr(2);
            lbt[0] = (char)0;     /*no label*/
        }
        else {
            p2 = &lmte->name[0];
            for(p1= &lbt[0]; p1 < &lbt[NAMELEN]; ) {
                *p1++ = *p2++;
            }
            if(fchr==':') 
				fchr=gchr();      /*ignore the colons*/
        }
labl1:
        ligblk();
        if(fchr == EOF) 
			return;
        if(fchr == '*') {
            igrst();        /*comment*/
            goto labl1;
        }
        gterm(TRUE);
        if(fchr==':' || fchr=='=') {    	/*another label*/
            if(lbt[0]) {
                strcpy(tlab1,lmte,NAMELEN); /*save current label */
                dlabl();    				/*define the last one*/
                pack(tlab1,lmte);   		/*restor the old lable*/
            }
            goto label;
        }
    }
    else
        lbt[0] = 0;         /*no label*/

    igblk();
    if(fchr == '=')
        goto label;
    if(itype==ITSP) {
        if(ival.loword == '=') {
            hequ();
            return;
        }
    }
    if(itype!=ITSY)   		/*not valid opcode*/
        goto cisi3;
    if (col1) { /* [vlh] could be a label save as is... */
        l = &str;
        strcpy(l,lmte->name,NAMELEN);
    }
    if((opcpt=lemt(TRUE,oirt))==lmte) { /*not in opcode table*/
        if (col1) { /* [vlh] it's a label... */
            strcpy(lmte->name,l,NAMELEN);
            goto label;
        }
cisi3:
        if (ca_true)    /* [vlh] report error if not in CA false */
            xerr(3);
        igrst();
        return;
    }
    getmode();      /*look for .b .w or .l mode flag*/
    if(opcpt->flags&OPDR) { /* its a directive*/
        i = opcpt->vl1;
        if (!ca_true && (i < LOW_CA || i > HI_CA)) { igrst(); return; }
        if (inoffset)   /* [vlh] */
            if (!(NOCODE)) {    /* can't generate code in offset */
                xerr(12);
                return;
            }
        dirop = p1direct[i];    /*call routine to handle directive*/
        (*dirop)();
        return;
    }
    else if (!ca_true) {        /* [vlh] */
        igrst();
        return;
    }
    else if (inoffset) {        /* [vlh] */
        xerr(12);
        return;
    }

    opcval = (opcpt->vl1);          /*opcode*/
    format = (opcpt->flags&OPFF);   /*format of this instr*/
    if (explmode)
        if (!modeok()) { xerr(16);  return; }
    dlabl();            /*define label*/
    opitb();            /*beginning of statement*/
    if(format)
        opito();        /*may have operands*/
    else
        igrst();        /*only comments*/
    format = (opcpt->flags&OPFF);   /* may have changed*/


/*end of statement*/

    i = calcilen();
    stbuf[1].itrl = i;      /*assumed instruction length*/
    stbuf[0].itrl = itwc;   /*number of it entries*/
    wostb();            	/*write out statement buffer*/
    loctr += i;
}

getmode()
{
    if (fchr=='.') {
        fchr = gchr();
        switch (fchr) {
            case 'b':
            case 'B':
            case 's':
            case 'S':
                modelen = BYTESIZ;
                mode = BYTE;
                break;
            case 'w':
            case 'W':
                modelen = WORDSIZ;
                mode = WORD;
                break;
            case 'l':
            case 'L':
                modelen = LONGSIZ;
                mode = LONG;
                break;
            default:
                peekc = fchr;
                fchr = '.';
                goto getm1;
        }
        explmode++;
        fchr = gchr();
        igblk();
        return;
    }
getm1:
    if(opcpt == exgptr) {   /*length is long*/
        modelen = LONGSIZ;
        mode = LONG;
    }
    else {
        mode = WORD;        /*default is word*/
        modelen = WORDSIZ;
    }
}

/* check to be sure specified mode is legal */
modeok()    /* [vlh] */
{
    switch(format) {
        case 0  :
        case 14 :
        case 18 :
            return(FALSE);
        case 13 :
        case 15 :
        case 20 :
        case 21 :
            return(modelen==BYTESIZ?FALSE:TRUE);
        case 4  :
        case 25 :
            return(modelen==BYTESIZ?TRUE:FALSE);
        case 7  :
        case 9  :
            return(modelen==WORDSIZ?FALSE:TRUE);
        case 5  :
        case 11 :
        case 28 :
            return(modelen==WORDSIZ?TRUE:FALSE);
        case 6  :
            return(modelen==LONGSIZ?FALSE:TRUE);
        case 12 :
        case 30 :
        case 22 :
        case 29 :
            return(modelen==LONGSIZ?TRUE:FALSE);
        default :
            return(TRUE);
    }
}

/* calculate the instruction length in bytes*/
calcilen()
{
    register short i;
    register long l;
    register char *p;

    i = 2;      /*all instrs at least 2 bytes*/

    switch(format) {

    case 20:
        i += 2; /*for reg mask*/
    case 1:     /*two ea's -- one of which may be a reg*/
    case 15:
    case 30:
    case 26:
    case 5:
    case 3:
    case 21:
        i += lenea(1);
    case 16:
    case 24:
    case 25:
    case 29:
        i += lenea(0);
        break;

    case 9:     /* [vlh] explicit jmp length... */
        if (!explmode)
            i += lenea(0);
        else
            return(mode==LONG?6:4); /*[vlh] explicit jmp.? */
        break;

    case 7:
        i += (immed[0]) ? 2+lenea(1) : lenea(1);
        break;

    case 14:
    case 11:
    case 19:
	case 31:
        i += 2;     /*always 4 bytes*/
        break;

    case 6:     /*relative branches*/
        if(itwc == ITOP1+1) {
            if(stbuf[ITOP1].itty == ITCN)
                l = stbuf[ITOP1].itop;
            else if(stbuf[ITOP1].itty == ITSY) {
                p = stbuf[ITOP1].itop.ptrw2;
                if(p->flags&SYDF)
                    l = p->vl1;         /*symbol value*/
                else
                    goto loffst;
            }
            else
                goto loffst;
            l -= (loctr+2);
            if(l<=127 && l>=-128)   /*8 bit offset*/
                break;
        }
loffst:
        if (!explmode || modelen > BYTESIZ) /*[vlh] recognize br extensions*/
            i += 2;     /*long offset for branches*/
        break;

    case 2:
        i += (mode==LONG?4:2) + lenea(1);
        break;

    case 23:
        if(immed[0])
            i += (mode==LONG?4:2);
    case 17:
    case 22:
        i += lenea(1);
        break;

    case 8:
        if(numops==1)       /*memory shift instruction*/
            i += shiftea(0);
        break;
    }

    return(i);
}

/* calc the length of an effective address*/
lenea(lidx)
int lidx;
{
    if(immed[lidx])
        return(mode==LONG?LONGSIZ:WORDSIZ);
    return(shiftea(lidx));
}

shiftea(lidx)
int lidx;
{
    if(indir[lidx])
        return((numcon[lidx] || numsym[lidx]) ? 2 : 0);
    if(numsym[lidx] || numcon[lidx])
        return((!shortadr || numcon[lidx]==2) ? 4 : 2);
    return(0);
}

/*
 *define a label if there is one to define
 *  call with:
 *      label name in lbt if it exists
 *      else lbt[0] == 0
 */
dlabl()
{
    if(lbt[0]) {    /*got a label*/
        pack(lbt,lmte);     /*put label in main table*/
        lblpt=lemt(FALSE,sirt); /*look up label*/
        if(lblpt != lmte) {     /*symbol entered previously*/
            if(lbt[0] == '~') {     /*local symbol -- may be duplicate*/
                lblpt = lmte;
                mmte();
            }
            else {
                if(lblpt->flags&SYXR) {
                    uerr(29);
                    lblpt = 0;
                    return;
                }
                if((lblpt->flags)&SYDF) {
                    rpterr("label redefined %s\n",lbt); /* non fatal err */
                    lblpt = 0;
                    return;
                }
            }
        }
        else {
            mmte();     /*make label entry in main table*/
        }
        lblpt->flags |= SYDF;   /*label is now defined*/
        lblpt->flags |= (rlflg==DATA)?SYRA:(rlflg==BSS)?SYBS:SYRO;
        lblpt->vl1 = loctr;     /*label value*/
    }
    else
        lblpt = 0;
}

/*
 * output it for operands
 *      gets intput from gterm
 *      puts output in stbuf using itwc as an index
 *      itwc should point at the next entry to be made in stbuf
 */
opito()
{
    register short lopcomma;

    lopcomma = symcon = chmvq = 0;
    numops++;               /*count first operand*/
    while(1) {
        starmul = symcon;   /*star is multiply op if flag is set*/
        if(fchr=='\'' || fchr=='"')
            lopcomma = 0;
        gterm(FALSE);   /*get a term*/
        if(itwc==ITOP1 && format==CLRFOR && opcval==CLRVAL)
            chgclr();
        opitoo();   /*output it for one operand*/
        if(itype==ITSP && ival.loword==',') {
            if (plevel==1 && !numcon[opdix])    /* [vlh] */
                numcon[opdix] = 1;
            if(lopcomma)
                uerr(30);
            lopcomma++;
            igblk();    /*ignore blanks for 68000 C compiler*/
        }
        else
            lopcomma=0;
        if(ival==EOLC && itype==ITSP)   /*end of operands*/
            break;
        if(fchr==EOLC) {
            fchr=gchr();
            break;
        }
    }
    if(chmvq)       /*changed move to moveq*/
        if(numops!=2 || immed[1] || indir[1] || numcon[1] || numsym[1] ||
          numreg[1]>=AREGLO) {
            stbuf[2].itop.ptrw2 = moveptr;  /*change it back*/
            opcpt = moveptr;
        }

	if (stbuf[2].itop.ptrw2==cmpptr)    /* [vlh] cmp -> cmpm ?? */
		if (numreg[0] && numreg[1] && indir[0] && indir[1]) {
			stbuf[2].itop.ptrw2 = cmpmptr;
			opcpt = cmpmptr;
		}

    if(lopcomma)
        uerr(30);
}

/* change clr.l An to suba.l An,An*/
chgclr()
{
    register char *p;

    if(itype==ITSY) {   /*first op is symbol*/
        p = lemt(FALSE,sirt);
        if(p==lmte)
            return;
        if(!(p->flags&SYER) || p->vl1<AREGLO)       /*not A reg*/
            return;
        opcpt = subaptr;    /*make it a suba instr*/
        opitb();
        opitoo();       /*output first operand -- An*/
        itype = ITSP;
        ival = ',';
        opitoo();       /*output a comma*/
        itype = ITSY;   /*now the A reg again*/
    }
}

/*output it for one operand*/
opitoo()
{
    register char *sp;

    symcon = 0;
    if(itype==ITSP) {   /*special symbol*/
        if(ival.loword==',' && !plevel) {      /* another operand */
            numops++;
            if(!opdix)
                opdix++;
        }
        if(ival.loword==')')
            symcon = 1;         /* star is multiply */
        if(ival.loword==' ') {     /*end of operands*/
            while(fchr!=EOLC)       /*ignore rest of statement*/
                fchr=gchr();
            return;
        }
        if(ival.loword==EOLC)
            return;
    }
    else        /*symbol or constant*/
        symcon = 1;

    if(itwc >= STMAX) {         /*it overflow*/
        rpterr("i.t. overflow\n");
        abort();
    }
    pitw->itty = itype;     /*type of it entry*/

/*put symbol in it buffer*/
    if(itype==ITSY) {
        sp=lemt(FALSE,sirt);        /*look up it main table*/
        pitw->itop.ptrw2 = sp;  /*ptr to symbol entry*/
        if(sp==lmte)            /*first occurrance*/
            mmte();
        itwc++;             /*count entries in it buffer*/
        pitw++;
        if(!(sp->flags&SYER))   /*is it a register?*/
            numsym[opdix]++;
        else        /*yes, a register*/
            numreg[opdix] = sp->vl1;
        return;
    }
    else if(itype == ITCN ) {
        if(ival.hiword && ival.hiword != -1)
            numcon[opdix] = 2;
        else if(!numcon[opdix])
            numcon[opdix] = 1;
        if(numops == 1)
            tryquick();
    }

/* special characters and constants*/
    pitw->itop = ival;
    pitw->itrl = reloc;
    itwc++;
    pitw++;
}

/* change add into addq and sub into subq if possible*/
tryquick()
{
    register char *p;
    register long l;

    if(fchr!=',' || !immed[0])
        return;
    l = ival;
    if(itwc != ITOP1+1) {
        if(itwc!=ITOP1+2 || stbuf[ITOP1+1].itty!=ITSP ||
          stbuf[ITOP1+1].itop.loword != '-')
            return;
        l = -l;
    }
    p = stbuf[2].itop.ptrw2;
    if(p==moveptr) {
        if(explmode && modelen != LONGSIZ)    /*dont change .w or .b*/
            return;
        if(l>=-128 && l<=127) {
            stbuf[2].itop.ptrw2 = moveqptr;
            opcpt = moveqptr;
            chmvq++;
        }
        return;
    }
    if(l<=0 || l>8) {
        return;
    }
    if(p==addptr || p==addiptr) {
        stbuf[2].itop.ptrw2 = opcpt = addqptr;
    }
    else if(p==subptr || p==subiptr) {
        stbuf[2].itop.ptrw2 = opcpt = subqptr;
    }
}

strcpy(astr1, astr2, alen)
char *astr1, *astr2;
register int alen;
{
    register char *str1, *str2;

    str1 = astr1;
    str2 = astr2;
    while (--alen != -1)
        *str1++ = *str2++;
}

/* index - find the index of a character in a string*/
/*      This is identical to Software Tools index.*/
index(str,chr)                      /* returns index of c in str or -1*/
char *str;                          /* pointer to string to search*/
char chr;                           /* character to search for*/
{
    register char *s;
    register short i;

    for( s = str, i = 0; *s != '\0'; i++ )
        if( *s++ == chr )
            return(i);
    return(-1);
}

