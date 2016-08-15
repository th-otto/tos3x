/*
    Copyright 1981
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca.  92121
*/

/* symbol table and miscellaneous pass 1 routines */

#include "as68.h"

char *ermsg[];
char initfnam[];
char ldfn[];
char tlab1[];
short ftudp;
short poslab;

/*output it for beginning of statement*/
opitb()
{
    stbuf[0].itty = ITBS;   /*beginning of statement*/
    stbuf[0].itop = (fchr!=EOLC) ? absln : absln-1;
    stbuf[1].itty = ITSY;   /*label entry*/
    stbuf[1].itop.ptrw2 = lblpt;    /*pointer to symbol or 0*/

/*put opcode in it buffer*/
    stbuf[2].itty = ITSY;
    stbuf[2].itrl = modelen;    /*mode of instr(byte, word, long)*/
    stbuf[2].itop.ptrw2 = opcpt;    /*pointer to opcode in main table*/
    stbuf[3].itty = ITCN;
    stbuf[3].itrl = rlflg;      /*relocation base*/
    stbuf[3].itop = loctr;      /*pass1 location counter*/
    itwc = ITOP1;               /*next available slot-currently 4*/
    pitw = &stbuf[ITOP1].itty;  /*init the pointer*/
}

/*
 *  get an input term  (symbol, constant, or special character)
 *  call with:
 *      the first character in fchr
 *  returns:
 *      item type in itype
 *      item value in ival if item is a constant or special character
 *      if it is a symbol it is placed at the end of the main table
 *
 * meaning of state table:
 * 
 * currently getting:       symbol(0)   constant(1)     beginning(2)
 *
 * next char:
 * digit(0)                 0           1               1
 *
 * letter(3)                0           3               0
 * 
 * special char(6)          3           3               3
 *
 * contents of the state table is the next state.  processing stops when
 * state 3 is encountered.  state 2 is the beginning state.
 */
int sttbl[] = {0,1,1,0,3,0,3,3,3};    /*state table for parser*/

gterm(constpc)
int constpc;
{
    register short smode, i;
    register char *p;
    register short tmode;
    register char *j;
    long num;
    char istr[80];

    if(fchr == '\'' || fchr == '"')
        if(astring())       /*maybe ascii string*/
            return;
    smode = 2;  /*beginning state*/
    i = 0;
    p = istr;

/*loop to put item on istr*/
    while(fchr>=' ') {      /*until a control char*/
        if(smode==2 && fchr=='.')
            tmode = 3;
        else if(isalpha(fchr) || fchr=='~' || fchr=='_' || (fchr=='$'&&i))
            tmode=3;
        else if(isdigit(fchr))
            tmode=0;
        else
            tmode = 6;
        tmode = sttbl[tmode+smode]; /*new state*/
        if(tmode==3) break;         /*end of item*/
        smode = tmode;
        *p++ = fchr;            /*save character*/
        i++;
        fchr=gchr();
    }

/* end of item*/
    switch(smode) {

    case 0:         		/*symbol*/
        *p = '\0';      	/*end of symbol*/
        itype = ITSY;       /*symbol*/
        pack(istr,lmte);    /*put symbol at end of main table*/
        j = lemt(FALSE,sirt);
        if(istr[0]!='~' && !poslab && (j->flags&(SYEQ|SYER))==SYEQ) {
            itype = (j->flags&SYRM) ? ITRM : ITCN;  /* [vlh] */
            ival = j->vl1;
            reloc = ((j->flags)&SYRO) ? TEXT : ((j->flags)&SYRA) ? DATA :
                ((j->flags)&SYBS) ? BSS : ABS;
        }
        return;

    case 1:         	/*constant*/
        if(!constant(&num,istr,i)) {
            uerr(17);   /*illegal constant*/
            num = 0;
        }
        ival = num;
        itype = ITCN;
        reloc = ABS;
        return;

    case 2:         /*just a special char*/
        switch(fchr) {

        case '*':           /*location counter*/
            if(starmul) {   /*multiply*/
                starmul = 0;
                goto specsy;
            }
            refpc++;    /*referenced pgm ctr*/
            reloc = rlflg;  /*relocation of location counter*/
            ival = loctr;
            itype = (constpc) ? ITCN : ITPC;
            break;


        case '$':           /*hex constant*/
            oconst(16);
            return;

        case '@':           /*octal const*/
            oconst(8);
            return;

        case '%':           /*binary const*/
            oconst(2);
            return;

        case '#':
            immed[opdix]++;
            goto specsy;

        case '(':
            indir[opdix]++;
            plevel++;
            goto specsy;

        case ')':
            plevel--;
            goto specsy;

        default:
specsy:
            itype = ITSP;       /*return special char*/
            ival = fchr;
        }
        if(fchr != EOLC)
            fchr=gchr();        /*get next char*/
        if((ival=='>' && fchr=='>') || (ival=='<' && fchr=='<'))
            fchr=gchr();            /*shift op, ignore second char*/
        return;

    default:
        abort();        /*not possible*/
    }
}

/*astring - check for an ascii string enclosed in single quotes*/
astring()
{
    register char delim;

    if(fchr != '\'' && fchr != '"')     /*valid delimiter*/
        return(0);
    delim = fchr;
    if(equflg || (itype==ITSP && ival.loword=='#')) {  /*immediate operand*/
        if(astr1(delim)) {
            fchr = gchr();
            if(fchr!=delim)
                xerr(19);
            fchr=gchr();
        }
        return((equflg) ? 1 : 0);
    }
    while(astr1(delim)) {
        itype = ITSP;
        ival = ',';         /*separate by commas*/
        reloc = ABS;
        opitoo();
    }
    return(0);
}

astr1(adelim)
int adelim;
{
    register short delim,i,retv;
    register long l;

    delim = adelim;
    i = 0; l = 0;
    retv = 1;
    while((fchr=gchr()) != EOF) {
        if(fchr==delim) {
            fchr = gchr();
            if(fchr != delim) {
                retv = 0;       /*end of string*/
                break;
            }
        }
        if(fchr == EOLC) {
            xerr(19);
            retv = 0;   /*end of string*/
            break;
        }
        l = (l<<8) | fchr;
        if(++i >= modelen) {
            if((fchr=gchr()) == delim) {
                fchr = gchr();
                retv = 0;       /*end of string*/
            }
            else
                peekc = fchr;   /*next char in string*/
            break;          /*filled one bucket*/
        }
    }
    while(i < modelen) {
        l <<= 8;
        i++;
    }
    itype = ITCN;
    ival = l;
    reloc = ABS;
    if(!equflg)
        opitoo();           /*output one operand*/
    return(retv);
}

/*get constant given radix*/
oconst(ardx)
int ardx;
{
    register short trdx,j;
    register long i;

    switch (ardx) {     /* radix as power of 2 */
        case 16 : trdx = 4; break;
        case  8 : trdx = 3; break;
        case  2 : trdx = 1; break;
        default :
            rpterr("invalid radix in oconst");
            abort();
    }
    i=0;
    while(1) {
        fchr=gchr();
        j=fchr;
        if(isdigit(j))
            j -= '0';
        else if((j=tolower(j))>='a' && j<='f')
            j = j-'a'+10;
        else
            break;          /*not valid numeric char*/
        if(j>=0 && j<ardx)
            i = (i<<trdx)+j;
        else
            break;
    }
    ival = i;
    itype = ITCN;
    reloc = ABS;
}


/*convert ascii constant to binary*/
constant(pnum,pstr,idx)
long *pnum;
register char *pstr;
int idx;
{
    register short i,j;
    register long l;

    l = 0;
    for(i=0; i<idx; i++) {
        j = *pstr++;
        if(isdigit(j))
            j -= '0';
        if(j<0 || j>=10)
            return(0);
        l = (l<<3) + (l<<1) + j;    /* l = l*10 + j*/
    }
    *pnum = l;
    return(1);
}

/**
 * method for looking up entries in the main table
 *
 * Note:    The entry to be looked up must be placed at the end
 *          of the main table.  The global cell 'lmte'(last main
 *          entry) points to the next available entry in the main
 *          table.  The address of an initial reference table must
 *          also be provided.
 *
 *  1)  Compute the hash code for the symbol and add it to the base address
 *      of the initial reference table given as input.  Thus, two words are
 *      accessed which define the chain on which the symbol must be if it
 *      is in the table at all.
 *
 *  2)  Alter the table link of the last symbol in the chain so that it
 *      points to the symbol being looked up.  Note that the symbol to be 
 *      looked up is always placed at the end of the main table before
 *      calling the lookup routine.  This essentially adds one more element
 *      to the end of the chain, namely the symbol to be looked up.
 *
 *  3)  Now start at the first symbol in the chain and follow the chain
 *      looking for a symbol equal to the symbol being looked up.  It is
 *      guaranteed that such a symbol will be found because it is always
 *      the last symbol on the chain.
 *
 *  4)  When the symbol is found, check to see if it is the last symbol
 *      on the chain.  If not, the symbol being looked for is in the table
 *      and has been found.  If it is the last symbol, the symbol being
 *      looked up is not in the table.
 *
 *  5)  In the case the looked up symbol is not found, it is usually added
 *      to the end of the table.  This is done simply b changing the
 *      initial reference table entry which points to the previous
 *      last symbol on the chain so that is now points to the symbol at the
 *      end of the main table.  In case the symbol just looked up is not to
 *      be added to the main table then no action is needed .  This means
 *      that the table link of the last symbol on a chain may point any-
 *      where.
 *
 * look up entry in the main table
 *      call with:
 *          address of initial reference table
 *          entry to be looked up at the end of the main table
 *      returns:
 *          a pointer to the entry.  if this pointer is equal to
 *          lmte then the symbol was not previously in the table.
**/
char *
lemt(oplook,airt)
char **airt;
int oplook;     /* if true then looking in opcode table */
{
    register char *mtpt;
    register short *p1, *p2, i, j;

    if (oplook) {   /* [vlh] get rid of preceding '.', to lowercase */
        if (lmte->name[0]=='.') {
            lmte->name[NAMELEN-1] = NULL;   /* in case of '.' */
            j = 1;
        }
        else 
            j = 0;
        for (i=0; j<NAMELEN; i++, j++)
            lmte->name[i] = tolower(lmte->name[j]);
    }
    pirt = airt + hash();   /*hashed ptr to irt*/
    mtpt = pirt->irfe;      /*pointer to first entry in chain*/
    if(!mtpt)             	/*empty chain*/
        mtpt = lmte;        /*start at end of main table*/
    else
        (pirt->irle)->tlnk = lmte;  /*last entry in chain is new symbol*/
	if((lmte->name[0]=='~') && (lmte->name[1]!='~') && (lmte->name[1]!='.'))
		return(lmte);		/*[vlh] 4.2, force local symbols */

/*loop to locate entry in main table*/
lemtl:
    p1 = &mtpt->name[0];
    p2 = &lmte->name[0];
    i = NAMELEN/(sizeof i);
    while(i) {
        if(*p1++ != *p2++) {
            mtpt = mtpt->tlnk;  /*go to next entry in chain*/
            goto lemtl;
        }
        i--;
    }
    return(mtpt);
}

/* compute a hash code for the last entry in the main table*/
/*  returns the hash code*/
hash()
{
    register short i, ht1;
    register char *p;

    ht1 = 0;
    p = &lmte->name[0];
    for(i=0; i<NAMELEN; i++) 
        ht1 += *p++;
    return(ht1&(SZIRT-2));  /*make hash code even and between 0 & SZIRT-2*/
}

/*
 * Make an entry in the main table
 * assumes :
 *   entry to be made is pointed at by lmte
 *   pirt points to the correct initial reference table entry.
 */
mmte()
{
    pirt->irle = lmte;      /*pointer to last entry in chain*/
    if(pirt->irfe == 0)     /*first entry in chain*/
        pirt->irfe = lmte;
    lmte += STESIZE;            /*bump last main table entry pointer*/
    if(lmte>=emte) {        /*main table overflow*/
        if(sbrk(STESIZE*ICRSZMT) == -1){    /*get more memory*/
            rpterr("symbol table overflow\n");
            endit();
        }
        else {
            emte += STESIZE*ICRSZMT;    /*move end of main table*/
            cszmt += ICRSZMT;
        }
    }
}

/*
 * make an entry in the main table for a directive
 *  call with:
 *      pointer to string containing directive name
 *      address of routine to handle directive in pass one
 *      address of routine to handle directive in pass two
 */
mdemt(mdstr,dirnum)
char *mdstr;
int dirnum;
{
    register char *mdept;

    pack(mdstr,lmte);       /*pack name at end of main table*/
    mdept=lemt(TRUE,oirt);  /*look up in opcode table*/
    if(mdept != lmte) {     /*best not be there already*/
        uerr(5);
        abort();
        return;
    }
    mmte();                 	/*make main table entry*/
    mdept->flags |= OPDR|SYIN;  /*directive*/
    mdept->vl1 = dirnum;        /*directive #*/
}

/*
 * pack a string into an entry in the main table
 *  call with:
 *      pointer to the string
 *      pointer to desired entry in the main table
 */
pack(apkstr,apkptr)
char *apkstr, *apkptr;
{
    register short i;
    register char *pkstr, *pkptr;

    pkstr = apkstr;
    pkptr = apkptr;
    i = NAMELEN;
    while(*pkstr && i) {
        *pkptr++ = *pkstr++;
        i--;
    }
    while(i--)
        *pkptr++ = '\0';    /*pad with nulls*/
}

/* function to get characters from source file*/
gchr()
{
    register short chr1;

    if(peekc) {
        chr1 = peekc;
        peekc = 0;
    }
    else {
gchr1:
        if(sbuflen<=0){     /*nothing on input buffer*/
            sbuflen=read(ifn,sbuf,BSIZE); /*read in source*/
            if(sbuflen<=0)
                return(EOF);            /*end of file*/
            psbuf = sbuf;
        }
        chr1 = *psbuf++;
        sbuflen--;
    }
    if (chr1 == SOH)    /*preprocessor flag*/
        goto gchr1;     /*ignore it*/
    if(chr1 == EOLC) {      /*end of line*/
        if(!p2flg)      /*pass 1 only*/
            absln++;
    }
    else if(chr1=='\t') {   /*convert tabs to spaces*/
        chr1 = ' ';
    }
    return(chr1);
}

/*
 * write out intermediate text for one statement
 *  call with
 *      the it for the statement in stbuf
 */
wostb()
{
    register short woix, *itwo, i;

    if(stbuf[0].itty != ITBS) 
		abort();  /*not beginning of stmt*/
    itwo = &stbuf;
    woix = stbuf[0].itrl & 0377;    /*unsigned byte*/
    while(woix--) {
        for(i=0; i<STBFSIZE/(sizeof *itwo); i++) {
            if(pitix > &itbuf[ITBSZ-1])       /*no room in buffer*/
                doitwr();
            *pitix++ = *itwo++; /*first word*/
        }
    }
/*  debug();        //call debug package*/
}

doitwr()
{
    register short i;

    if(write(itfn,itbuf,ITBSZ*(sizeof i)) != ITBSZ*(sizeof i)) {
        rpterr("it write error errno=%o\n",errno);
        endit();
    }
    pitix = itbuf;
}

/*
 * user source error
 *  call with:
 *      number to indicate reason for error
 *  types the error number and the line number on which
 *  the error occured.
 */
uerr(errn)
int errn;
{
	putchar(0);	/* clear the buffer */
	stdofd = STDERR;	/* output file descriptor <== STDERR */
	if(p2flg) { /*pass 2 gets two ampersands*/
        in_err++;   /* [vlh] instrlen <- pass1 estimation */
        printf("&& %d: %s\n",p2absln,ermsg[errn-1]);
    }
    else
        printf("& %d: %s\n",(fchr==EOLC)?absln-1:absln,
			   ermsg[errn-1]);
	putchar(0);
	stdofd = STDOUT;
    nerror++;
}
/*
 * user error that causes the statement to be abandoned
 *  call with:
 *      error number
 */
xerr(xern)
int xern;
{
    uerr(xern);     /*type error message*/
    if(!p2flg)      /*pass one*/
        igrst();    /*pass rest of source*/
}

/* abort the assembly*/
abort()
{
    rpterr("as68 abort\n");
    endit();
}

/*ignore rest of statement*/
igrst()
{
    while(fchr!=EOLC && fchr!=EOF)  /*until end of line*/
        fchr=gchr();
    while((fchr=gchr())==EOLC) ;    /*ignore null lines*/
}

/*ignore blanks after a label*/
ligblk()
{
    if(fchr == EOF) return;
    igblk();
    if(fchr==EOLC) {
        fchr=gchr();
        ligblk();
    }
}

rubout()
{
    nerror = -1;
    endit();
}

/* exit from the assembler*/
endit()
{
    LASTCHTFN = itfnc;
    unlink(tfilname);       /*delete temporary files*/
    LASTCHTFN = trbfnc;
    unlink(tfilname);
    LASTCHTFN = dafnc;
    unlink(tfilname);
    LASTCHTFN = drbfnc;
    unlink(tfilname);
    if(nerror != -1) {      /*not rubout*/
        if(ftudp)
            putchar('\n');
        putchar(0); /* flush the printing*/
    }
    if(nerror > 0) {
        putchar(0);
        stdofd = STDERR;
        printf("& %d errors\n",nerror);
        putchar(0);
    }
    if (initflg)
        unlink(ldfn);   /* [vlh] get rid of empty .o file */
    exit(nerror!=0);
}

/*
 * open files
 *  call with:
 *      pointer to name of file to open
 *      flag for how to open
 *          0 => read
 *          1 => write
 */
openfi(pname,hflag)
char *pname;
int hflag;
{
    register short fd;

    fd = (hflag) ? creat(pname,0666) : open(pname,hflag);
    if(fd < 0) {    /*open failed*/
        rpterr("can't open %s errno=%o\n",pname,errno);
        endit();
    }
    return(fd);
}

/* get a temp file for the intermediate text*/
gettempf()
{
    register short j;
    register char *p;

    if(LASTCHTFN == 'A') {
        j = getpid();
        p = &LASTCHTFN-4;
        while(p < &LASTCHTFN) {
            *p++ = (j&017) + 'a';
            j >>= 4;
        }
    }
    while(LASTCHTFN < 'z') {
        LASTCHTFN++;
        if((j=creat(tfilname,0600))>=0)
            return(j);
    }
    rpterr("temp file create error: %s errno=%o\n",tfilname,errno);
    endit();
}

/* move label name from lbt to main table entry pointed to by lmte*/
setname()
{
    register short *p1, *p2;

    p1 = &lmte->name[0];
    for(p2 = &lbt[0]; p2 < &lbt[NAMELEN]; ) {
        *p1++ = *p2;
        *p2++ = 0;
    }
}

/* get the initialized main table and initial reference tables from*/
/*  the initialize file*/
getsymtab()
{
    register char **p;
    register struct symtab *p1;
    register char *p2;
    register short fd,i;

    if((fd=open(initfnam,0)) < 0) {
rerr:
        printf("& Unable to read init file: %s\n", initfnam);
        endit();
    }
    if(read(fd,sirt,SZIRT*SIRTSIZE) != SZIRT*SIRTSIZE) {
        goto rerr;
    }

    if(read(fd,oirt,SZIRT*SIRTSIZE) != SZIRT*SIRTSIZE)
        goto rerr;

    if((i=read(fd,bmte,SZMT*STESIZE)) <= 0)
        goto rerr;

    if((i%STESIZE) != 0)
        goto rerr;

    lmte = bmte + i;
    p2 = bmte-1;
    for(p=sirt; p<&sirt[SZIRT]; p++) {
        if(*p)
            *p += (long)p2; /* 11 apr 83, for vax */
    }
    for(p=oirt; p<&oirt[SZIRT]; p++) {
        if(*p)
            *p += (long)p2; /* 11 apr 83, for vax */
    }
    for(p1=bmte; p1<lmte; p1++) {
        if(p1->tlnk)
            p1->tlnk += (long)p2; /* 11 apr 83, for vax */
    }
    close(fd);
}

/* write the initialization file*/
putsymtab()
{
    register char **p;
    register struct symtab *p1;
    register char *p2;
    register short fd,i;

    if((fd=creat(initfnam,0644))<0) {
werr:
        printf("& Write error on init file: %s\n", initfnam);
        return;
    }
/*
 * change all pointers so that they are relative to the beginning
 * of the symbol table
 */
    p2 = bmte-1;
    for(p=sirt; p<&sirt[SZIRT]; p++) {
        if(*p)
            *p = *p - p2;   /* 11 apr 83, for vax */
    }
    for(p=oirt; p<&oirt[SZIRT]; p++) {
        if(*p)
            *p = *p - p2;   /* 11 apr 83, for vax */
    }
    for(p1=bmte; p1<lmte; p1++) {
        if(p1->tlnk)
            p1->tlnk = p1->tlnk - p2;   /* 11 apr 83, for vax */
    }

    if(write(fd,sirt,SZIRT*SIRTSIZE) != SZIRT*SIRTSIZE) {
        goto werr;
    }

    if(write(fd,oirt,SZIRT*OIRTSIZE) != SZIRT*OIRTSIZE)
        goto werr;

    i = (char *)lmte - bmte;        /*length of current main table*/
    if((i % STESIZE) != 0) {
        goto werr;
    }
    if(write(fd,bmte,i) != i)
        goto werr;
    close(fd);
}

/* print an error on file descriptor 2*/
/*  used for errors with disasterous consequences*/
rpterr(ptch,x1,x2,x3,x4,x5,x6)
char *ptch;
{
    putchar(0);     /*flush buffer*/
    stdofd = STDERR;     /*error file*/
    printf("& %d: ",absln);
    printf(ptch,x1,x2,x3,x4,x5,x6);
	nerror++;	/* [vlh] 4.2.... */
}

/* set the file name for the relocatable object file (sourcefile.o)*/
setldfn(ap)
char *ap;
{
    register char *p1,*p2;

    p1 = ap;
    p2 = ldfn;
    while(*p1) {
        *p2++ = *p1++;
    }
    if(*(p2-2) != '.') {    /*not name.?*/
        *p2++ = '.';
        *p2++ = 'o';
    }
    else {          /* is name.? */
        *(p2-1) = 'o';
    }
    *p2 = '\0';
}
