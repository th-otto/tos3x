
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* pass 2 miscellaneous routines */

#include "as68.h"

long stlen;
short stdofd = STDOUT;
short debug = 0;
short rlbits[], f2mode[];
short udfct, ftudp, pline, prsp;

clrea(ap)
struct op *ap;
{
	register struct op *p;

	p = ap;
	p->ea = p->len = p->xmod = p->drlc = 0; p->con = 0;
	p->ext = p->idx = -1;
}

#define US	(unsigned short)
/*
 * get one operand effective adddress (operand until , or EOS)
 * returns:
 *	opnd[opn].ea set to effective address mode bits
 *	opnd[opn].len set to # bytes for operand
 *	opnd[opn].con set to constant part of ea
 *	opnd[opn].ext set to external symbol # if any
 *	opnd[opn].idx set to index register if any
 *	opnd[opn].drlc set to effective address relocation mode
 *	opnd[opn].xmod set to index register addressing mode (word or long)
 */
getea(opn)
int opn;
{
	register short i, disp, inst;
	register struct op *p;

	p = &opnd[opn];
	disp = 0;
	clrea(p);
	if(ckitc(pitw,'#')) {
		p->len = (modelen == BYTESIZ) ? WORDSIZ : modelen;
		p->ea = IMM;
		pitw++;
		goto dosimp;
	}
	if(ckitc(pitw,'(')) {
geteal1:
		pitw++;
		if((i=getrgs()) == PC) {	/*pc relative*/
			p->ea = 072;		/*set mode & register bits*/
			p->len = 2;
		}
		else {
			if(i != -1)	/*last was some type of register*/
				pitw--;		/*havent used it yet*/
			if((i=getareg()) < 0) {	/*not a reg # next*/
				if(disp || getreg() != -1) {
					uerr(14);	/*illegal index reg*/
					return;
				}
				pitw--;
				goto dosimp;	/*must be expression in ()*/
			}
			p->ea = i&7;		/*put in a reg #*/
		}
		if(ckitc(pitw,',')) {		/*must be index reg #*/
			do_ireg(p,i);
			return;
		}
		ckrparen();
		if(i != PC) {
			if(!disp && ckitc(pitw,'+')) {
				pitw++;
				p->ea |= INDINC;
			}
			else if(disp) {		/*indirect with displacement*/
				p->ea |= INDDISP;
				p->len = 2;
			}
			else 
				p->ea |= INDIRECT;
		}
		ckeop(9+opn);
		return;
	}
	if(ckitc(pitw,'-')) {		/*predecrement maybe*/
		pitw++;
		if(ckitc(pitw,'(')) {	/*must be*/
			pitw++;
			if((i = getareg()) < 0) {	/*not valid a reg*/
				pitw -= 2;		/*must be negative expr*/
				goto dosimp;
			}
			p->ea = i|DECIND;
			ckrparen();
			ckeop(9+opn);
			return;
		}
		pitw--;
	}
dosimp:					/*simple addr or imm expr*/
	if(i=gspreg()) {
		inst = ins[0];
		if(i==PC || (i==USP && inst!=MOVE && inst!=MOVEC))
			uerr(20);
		if(i==SR || i==CCR)
			if(inst!=(US AND) && inst!=(US OR) && inst!=(US EOR) && inst!=ANDI 
					 && inst!=ORI && inst!=EORI && inst!=MOVE)
				uerr(20);
		if((i==SFC || i==DFC || i==VSR) && inst!=MOVEC)
			uerr(20);
		p->idx = i;
		ckeop(9+opn);
		return;
	}
	if((i=getreg()) >= 0) {	/*register direct*/
		p->ea = i;
		if(modelen == BYTESIZ && i>=AREGLO && i<=AREGHI)
			uerr(20);
		ckeop(9+opn);
		return;
	}
	expr(&p2gi);
	if(pitw < pnite)	/*expr passes one token*/
		pitw--;
	if(extflg) {
		p->ext = extref;
		extflg = 0;
	}
	p->con = ival;
	p->drlc = reloc;		/*relocation factor*/
	if(ckitc(pitw,'(')) {
		disp++;
		goto geteal1;
	}
	if(!p->ea) {	/*memory  address*/
		if(shortadr && (!ival.hiword || ival.hiword== -1)) { /*16-bit addrs*/
			p->ea = SADDR;
			p->len = 2;
		}
		else {
			p->ea = LADDR;
			p->len = 4;
		}
	}
	ckeop(9+opn);
}

do_ireg(p,i,opn)
struct op *p;
int i, opn;
{
	pitw++;
	p->idx = getreg();
	if(p->idx<0 || p->idx>AREGHI)
		uerr(14);
	p->len = 2;
	if(!ckitc(pitw,')')) {
		p->xmod = getrgs() - WORD_ID;
		if(p->xmod<0 || p->xmod>1) {
			uerr(34);
			p->xmod = 0;
		}
	}
	ckrparen();
	ckeop(9+opn);
	if(i==PC)
		p->ea += 1;
	else
		p->ea |= INDINX;
}

/*
 * get an A register specification
 *  call with:
 *		pitw pointing to reg operand
 *  returns:
 *		-1 if not vaid A reg
 *		A reg # if valid
 *		also updates pitw if valid
 */
getareg()
{
	register short i;

	i = getreg();
	if(i>=AREGLO && i<=AREGHI) {
		return(i&7);
	}
	else {
		if(i != -1)
			pitw--;
		return(-1);
	}
}

/*
 * get any register specification
 *  call with :
 *		pitw pointing at operand
 *  returns:
 *		register # with pitw updated
 *		-1 if not valid register
 */
getreg()
{
	register short i;

	i = getrgs();
	if(i>=0 && i<=AREGHI)
		return(i);
	else {
		if(i != -1)
			pitw--;
		return(-1);
	}
}

/*get any register specification*/
getrgs()
{
	register char *i;

	if(pitw->itty == ITSY) {
		i = pitw->itop.ptrw2;		/*symbol ptr*/
		if(i->flags&SYER) {
			pitw++;
			return((int)i->vl1.loword);		/*register #*/
		}
	}
	return(-1);
}

/* check for a right paren as the next char*/
/*  output error msg if not found*/
ckrparen()
{
	if(ckitc(pitw,')'))	/*found it*/
		pitw++;
	else
		uerr(32);
}

/*
 * check intermedate text item for special character
 *	call with:
 *		pointer to desired item in stbuf
 *		character to check for
 *	returns:
 *		0 => no match
 *		1 => match
 */
ckitc(ckpt,cksc)
char *ckpt;
int cksc;
{
	if(ckpt >= pnite || ckpt->itty != ITSP || ckpt->itop.loword != cksc)
		return(0);
	return(1);
}

/*
 * read intermediate text for one statement
 * returns:
 *	intermediate text in stbuf
 */
ristb()
{
	register short riix, i, *pi;

	do {
		riix = stbuf[0].itrl;
		itoffset += (riix&0377) * STBFSIZE;
		pi = &stbuf[0];
		for(i=0; i<(STBFSIZE)/(sizeof *pi); i++) {
			if(pitix > &itbuf[ITBSZ-1]) {	/*need new buffer full*/
				doitrd();		/*read it buffer*/
			}
			*pi++ = *pitix++;		/*first word of it*/
		}
		if(stbuf[0].itty != ITBS) /*best be beginning of statement */
			abort();

/* get the rest of the statement it*/
		riix = stbuf[0].itrl & 0377;	/*unsigned byte*/
		riix--;				/*already got first entry*/
		while(riix--) {
			for(i=0; i<(STBFSIZE)/(sizeof *pi); i++) {
				if(pitix > &itbuf[ITBSZ-1]) {	
					doitrd();	/*get new buffer of it*/
				}
				*pi++ = *pitix++;
			}
		}
	} while(stbuf[1].itrl == -1);	/* eliminated instr, read next one */
}

int errno;

doitrd()
{
	register short i;

	pitix = itbuf;
	if((i=read(itfn,itbuf,ITBSZ*(sizeof i)))<=0) {
		putchar(0);
		stdofd = STDERR;
		printf("it read error i=%d errno=%o\n", i,errno);
		putchar(0);
		abort();
	}
}

/*
 * check for end of operand
 * call with
 *		error number if this is not end of operand
 */
ckeop(uen)
int uen;
{
	if(pitw>=pnite)	/*end of all operands*/
		return(1);
	if(!ckitc(pitw,',')) {		/*not end of stmt must be op,op*/
		uerr(uen);
		return(0);
	}
	return(1);
}

/* output symbol table to file*/
osymt()
{
	register char **sx1;
	register char *p;
	register short i;

	stlen = 0;
	if(extindx) {			/*output external symbols first*/
		if(prtflg)
			printf("\n\n-EXTERNAL SYMBOLS-\n");
		sx1 = extbl;
		for(i=0;i<extindx;i++)	/*go through external table*/
			osyme(*sx1++);	/*output symbol*/
	}
	if(prtflg)
		printf("\n\n");

	for(p=bmte; p<lmte; p+= STESIZE) {	/*want them in order defined*/
		if(p->flags&(SYXR|SYIN))
			continue;
		osyme(p);
	}
}

/* make all undefined symbols external*/
fixunds()
{
	register char **sx1, **sx2;

/* loop thru symbol initial reference table*/
	for(sx1= sirt; sx1<&sirt[SZIRT-1]; sx1 += 2) {
		if(*(sx2 = sx1+1)==0)		/* this chain is empty*/
			continue;

/* symbols on one chain*/
		sx2 = *sx2;	/*first entry on this chain*/
		while(1) {
			if(!(sx2->flags&SYDF)) {	/*not defined*/
				if(undflg || sx2->flags&SYGL) { /*all or globals*/
					sx2->flags = sx2->flags|SYDF|SYXR;
					mkextidx(sx2);
				}
			}
			if(sx2 == *sx1)	/*end of chain*/
				break;
			sx2 = sx2->tlnk;	/*next entry in chain*/
		}
	}
}

/*
 * output symbols in a form to be read by a debugger
 * call with pointer to symbol table entry
 * prints all undefined symbols
 */
osyme(aosypt)
struct symtab *aosypt;
{
	register struct symtab *osypt;
	register char *p1;
	register short i;
	long lsyval;

	osypt = aosypt;		/*pointer to symbol table entry*/
	if(!prtflg && !(osypt->flags&SYDF)) {	/*undefined symbol*/
		pudfs(osypt);				/*print undefined*/
		return;
	}

	p1 = &(osypt->name[0]);
	if(prtflg && !((osypt->flags)&SYER)) {	/*put symbol on listing*/
		if(osypt->flags&SYXR)	/*external symbol*/
			printf("%-8.8s\n",p1);		/*just print name*/
		else if (osypt->flags&SYDF) {
			printf("%-8.8s\t",p1);
			puthex((int)osypt->vl1.hiword,4);
			puthex((int)osypt->vl1.loword,4);
			if(osypt->flags&SYRA)	/*print relocation factor*/
				printf("\tDATA\n");
			else if(osypt->flags&SYRO)
				printf("\tTEXT\n");
			else if(osypt->flags&SYBS)
				printf("\tBSS\n");
			else 
				printf("\tABS\n");
		}
		else {	/*undefined*/
			nerror++;
			printf("%-8.8s\t*UNDEFINED*\n",p1);
		}
	}

	stlen += 14;	/*one more symbol out*/

/*output symbol to loader file*/
	p1 = &(osypt->name[0]);
	for(i=0; i<NAMELEN; i++)	/*output symbol name*/
		putc(*p1++,&lbuf);

	lputw(&osypt->flags,&lbuf);	/* output symbol flags */
#ifdef DEBUG
	if (debug)	/* prints symbol table entries */
		printf("> %-8.8s* %o\n",osypt->name,(int)osypt->flags);
#endif
	if(osypt->flags&SYXR) {		/*external symbol*/
		lsyval.hiword = 0;
		lsyval.loword = osypt->vl1.hiword;
		lputl(&lsyval,&lbuf);
	}
	else
		lputl(&osypt->vl1,&lbuf);	/* symbol value */
}

/*
 * print undefined symbols
 * call with
 *	pointer to undefined symbol
 */
pudfs(udspt)
struct symtab *udspt;
{
	nerror++;
	if(!ftudp) {		/*first time thru*/
		printf("\n&& UNDEFINED SYMBOLS &&\n");
		ftudp++;
		udfct=0;		/*no symbols on this line*/
	}

	printf("%-8.8s  ",&(udspt->name[0]));
	if(udfct++ > 6) {
		printf("\n");
		udfct=0;
	}
}

/*
 * output source and object listing
 *	call with
 *		2 => print address and binary code only
 *		1 => object in ins[] and instr type in format
 *		0 => print address only
 */
print(pflag)
int pflag;
{
	register short i, *pi;

	if( !prtflg || fchr == EOF) /*no printing desired, or end of source file*/
		return;

	i = instrlen; instrlen = 1;	/*to print preceeding lines*/
	while(pline<p2absln) {		/*need to print some lines*/
		printf("%4d ",pline);	/*put source line num on listing*/
		printf("                        ");	/*align the source*/
		prtline(1);
		putchar('\n');
		if ((fchr=gchr()) == EOF)
			return;
		pline++;
	}
	instrlen = i;

/* output current address, binary, and source*/
	printf("%4d ",p2absln);		/*put source line num on listing*/
	puthex((int)loctr.hiword,4);
	puthex((int)loctr.loword,4);
	putchar(' ');
	if(!pflag)			/*no binary*/
		printf("                   ");	/*blanks instead*/
	else {
		pi = ins;
		for(i=0; i< (instrlen/2); i++)	/* binary*/
			puthex(*pi++,4);
		if(instrlen&1)
			puthex(*pi,2);
		putchar(' ');
		for(;i<4;i++) 			/*four bytes max per line*/
			printf("    ");		/*align the source*/
	}
	if(pline>p2absln || pflag==2)
		putchar('\n');			/*end of line*/
	else {
		prtline(0);
		if(fchr==EOF) return;
		putchar('\n');
		fchr=gchr();
		pline++;
	}
}

/*print one line aligning source output*/
prtline(flg)
int flg;
{
	register short blcnt;
	short i, col;

	if(fchr=='*' || flg) {	/*comment*/
		while(fchr!=EOLC && fchr!=EOF) {
			putchar(fchr);
			fchr = gchr();
		}
		return;
	}
	col = 1;
	blcnt = 0;
	while(1) {
		if(fchr==EOLC || fchr==EOF)
			return;
		if(fchr==' '&& blcnt<3) {
			++blcnt;
			i= (blcnt == 3) ? 017 : 7;
			while(col&i) {
				putchar(' ');
				col++;
			}
			while(fchr==' ')
				fchr=gchr();
			if(fchr==EOLC || fchr==EOF)
				return;
		}
		putchar(fchr);
		fchr=gchr();
		col++;
	}
}

/* buffered putchar routine*/
putchar(c)
char c;
{
	register short i;

	*prtchidx++ = c;
	if(!c || prtchidx >= &prtchars[PRTCHLEN]) {
		i = (short)(prtchidx - prtchars);
		write(stdofd,prtchars,i);
		prtchidx = prtchars;
	}
}

short hibytflg[4], hibytw[4];

outbyte(bv,br)
int bv, br;
{
	if (rlflg == BSS) {		/* [vlh] */
		uerr(39);
		return;
	}
	if(hibytflg[rlflg]) {
		outword(hibytw[rlflg]|(bv&0xff),br);
		hibytflg[rlflg] = 0;
	}
	else {
		hibytw[rlflg] = bv<<8;
		hibytflg[rlflg]++;
	}
}

outword(val,rb)
int val, rb;
{
	switch(rlflg) {

		case TEXT:
			lputw(&val,&lbuf);
			lputw(&rb,&tbuf);
			break;

		case DATA:
			lputw(&val,&dabuf);
			lputw(&rb,&drbuf);
			break;

		case BSS:
			uerr(39);
			break;

		default:
			rpterr("& outword: bad rlflg\n");
			abort();
	}
}

outinstr()
{
	register short i, *p1, *p2;

	i = instrlen>>1;
	p1 = ins;
	p2 = rlbits;
	while(i--) 
		outword(*p1++, *p2++);
	
}

/* copy data bits from temporary file to loader file*/
cpdata()
{
	myfflush(&lbuf);
	myfflush(&dabuf);
	docp(dafn,dafnc);
}

/* copy text then data relocation bits from temporary file to loader file*/
cprlbits()
{
	myfflush(&lbuf);
	myfflush(&drbuf);
	docp(trbfn, trbfnc);
	docp(drbfn, drbfnc);
}

/*
 * copy one of the temporary files to the loader file
 * call with:
 *	file descriptor of the temporary file
 *	last char of the temporary file name
 */
docp(cfn,cfnc)
int cfn, cfnc;
{
	register short i;

	close(cfn);
	LASTCHTFN = cfnc;
	cfn = openfi(tfilname,0);
	while((i=read(cfn,itbuf,BSIZE)) > 0) {
		if(write(lfn,itbuf,i) != i) {
			rpterr("& file write error\n");
			abort();
		}
	}
}

/* print one word in hex*/
puthex(v,l)
int v, l;
{
	register short i,j,k;

	j = 12;
	for(i=0; i<l; i++) {
		k = (v>>j)&017;
		k += (k >= 10) ? ('A'-10) : '0';
		putchar(k);
		j -= 4;
	}
}

/* check for a control operand*/
controlea(ap)
struct op *ap;
{
	register short i;

	i = ap->ea&070;
	if(i==INDIRECT || i==INDDISP || i==INDINX)
		return(1);
	if(i==070) {
		if((ap->ea&7) <= 3)
			return(1);
	}
	return(0);
}

ckcomma()
{
	if(ckitc(pitw,',')) {		/*next token a comma*/
		pitw++;
		return(1);
	}
	return(0);
}

/*
 * generate any necessary additional words for the effective address
 *  call with:
 *		pins pointing to next available word in ins[]
 *		prlb pointing to next available word in rlbits[]
 *		argument is ptr to op structure
 *
 * returns:
 *		appropriate words in ins[] and rlbits[] for operand
 *		pins and prlb updated.
 */
doea(apea)
struct op *apea;
{
	register short i;
	register struct op *p;

	p = apea;
	switch((p->ea>>3)&7) {	/* ea mode bits*/

		default:		/*no more words*/
			return;

		case 5:			/* d(An)*/
			dodisp(p);
			return;

		case 6:			/* d(An,Ri)*/
dindx:
			if (p->con > 127L || p->con < -128L)
				uerr(35);
			i = (p->con.loword&0377) | (p->idx<<12) | (p->xmod<<11);
			if(p->drlc != ABS)
				uerr(27);
			*pins++ = i;
			*prlb++ = DABS;
			instrlen += 2;
			return;

		case 7:		/*xxx.W, xxx.L, or #xxx*/
			switch(p->ea&7) {

				case 1:		/* xxx.L*/
					doupper(p);
					p->con.hiword = 0;		/*clear for dodisp check*/

				case 0:		/* xxx.W*/
					dodisp(p);
					return;

				case 2:		/*d(PC)*/
				case 3:		/*d(PC,Ri.X)*/
					if(p->drlc != ABS) {
						if(p->drlc != rlflg)	/*not same reloc base*/
							uerr(27);
						p->con -= (loctr+instrlen);
						p->drlc = ABS;
					}
					if((p->ea&7) == 3)	/*d(PC,Ri.X)*/
						goto dindx;
					dodisp(p);
					return;

				case 4:		/* #xxx*/
					chkimm(p);		/*check for valid length*/
					if(modelen == LONGSIZ) {	/*instr mode is long*/
						doupper(p);
						p->con.hiword = 0;	/*clear for dodisp check*/
					}
					dodisp(p);
					return;
			}	/* switch in case 7 */
	}
}

dodisp(ap)
struct op *ap;
{
	register struct op *p;

	p = ap;
	*pins++ = p->con.loword;		/*displacement*/
	if(p->con.hiword && p->con.hiword != -1)
		uerr(41);		/*invalid 16-bit disp*/
	*prlb++ = (p->ext != -1) ? (p->ext<<3)|EXTVAR : p->drlc;
	instrlen += 2;
}

doupper(p)
struct op *p;
{
	*pins++ = p->con.hiword;	/*upper half of long addr or constant*/
	*prlb++ = LUPPER;
	instrlen += 2;
}

/*
 * build a format 1 (add, sub, and, etc) instr
 * call with:
 *	register #
 *	mode bits
 *	ptr to operand structure for effective address
 */
makef1(arreg, armode, apea)
struct op *apea;
int arreg, armode;
{
	register struct op *p;

	p = apea;
	ins[0] |= (arreg<<9);	/*put in reg #*/
	ins[0] |= armode;		/*instr mode bits*/
	ins[0] |= p->ea;		/*put in effective addr bits*/
	doea(p);				/*may be more words in ea*/
}

/* generate an immediate instr*/
genimm()
{
	ins[0] |= (f2mode[modelen] | opnd[1].ea);
	if(modelen == LONGSIZ) {
		doupper(&opnd[0]);
		opnd[0].con.hiword = 0;	/*clear for dodisp check*/
	}
	chkimm(&opnd[0]);	/*check for valid immed length*/
	dodisp(&opnd[0]);
	doea(&opnd[1]);
}

chkimm(ap)
struct op *ap;
{
	register struct op *p;

	p=ap;
	if(modelen == WORDSIZ) {	/*word*/
		if(p->con.hiword && p->con.hiword != -1)
			uerr(42);
	}
	else if(modelen == BYTESIZ) {	/*byte*/
		if(p->con.hiword && p->con.hiword != -1)
			uerr(43);
		if(p->con.loword>255 || p->con.loword <= -256)
			uerr(43);
	}
}

/* try to make a normal instr into an immediate instr*/
makeimm()
{
	if(opnd[0].ea != IMM)
		return(0);
	if(!dataalt(&opnd[1]))
		return(0);
	if(opcpt == addptr)
		opcpt = addiptr;
	else if(opcpt == andptr)
		opcpt = andiptr;
	else if(opcpt == orptr)
		opcpt = oriptr;
	else if(opcpt == subptr)
		opcpt = subiptr;
	else if(opcpt == cmpptr)
		opcpt = cmpiptr;
	else if(opcpt == eorptr)
		opcpt = eoriptr;
	else
		return(0);
	ins[0] = opcpt->vl1.loword;
	format = (opcpt->flags)&OPFF;
	genimm();
	return(1);
}

ckbytea()
{
	if(modelen == BYTESIZ && !dataea(&opnd[0]))
		uerr(20);	/*byte mod not allowed*/
}

/* get a special register token (CCR, SR, USP, SFC, DFC or VSR)*/
gspreg()
{
	register short i;

	i = getrgs();
	if(i>AREGHI)
		return(i);
	if(i != -1)
		pitw--;
	return(0);
}

/**
 * check an operand for a special register
 * call with:
 *  ptr to operand struct
 *  special register value
**/
cksprg(ap,v1)
struct op *ap;
int v1;
{
	if(ap->ea)
		return(0);
	return((ap->idx == v1));
}

/* check for operand as any special register*/
anysprg(ap)
struct op *ap;
{
	if(ap->ea)
		return(0);
	if(ap->idx>=CCR && ap->idx<=USP)
		return(1);
	return(0);
}

/* copy opnd 0 to opnd 1*/
cpop01()
{
	opnd[1].ea = opnd[0].ea;
	opnd[1].len = opnd[0].len;
	opnd[1].con = opnd[0].con;
	opnd[1].drlc = opnd[0].drlc;
	opnd[1].ext = opnd[0].ext;
	opnd[1].idx = opnd[0].idx;
	opnd[1].xmod = opnd[0].xmod;
}

cksize(ap)		/* [vlh] try to check displacement range */
struct op *ap;
{
	long value;

	if ((ap->ea&070) != 070) return;
	value = (ap->con>0 && ap->con&0100000) ? -(ap->con&~0100000) : ap->con;
	if (modelen == BYTESIZ) {
		if (value < -128L || value > 127L)	/* 8 bits */
			uerr(35);
	}
	else if (modelen == WORDSIZ)
		if (value > 32767L || value < -32768L)	/* 16 bits */
			uerr(41);
}

ccr_or_sr()		/* [vlh] */
{
	if(opnd[1].idx==CCR)
		modelen = BYTESIZ;		/*byte mode only*/
	else	/* [vlh] SR */
		if (modelen != WORDSIZ) {
			modelen = WORDSIZ;
			uerr(34);
		}
	cksize(&opnd[0]);
	ins[0] |= IMM | f2mode[modelen];
	dodisp(&opnd[0]);
}

get2ops()
{
	getea(0);		/*get first effective address*/
	if(!ckcomma()) {
		uerr(10);
		return(1);	/*no second op*/
	}
	getea(1);		/*get second effective address*/
	return(0);
}

myfflush(ibuf)
register struct iob *ibuf;
{
	register short i;

	i = BSIZE - ibuf->cc;
	ibuf->cc = BSIZE;
	ibuf->cp = &(ibuf->cbuf[0]);
	if (write(ibuf->fd,ibuf->cp,i) != i)
		return(-1);
	return(0);
}
