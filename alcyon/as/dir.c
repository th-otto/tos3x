/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*	Pass 1 and pass 2 directive handling routines */
/*	code to handle conditional assembly directives */

#include "as68.h"

int p1gi();
int p2gi();
int igrst();

/*directive to define an opcode*/
hopd()
{
	if(!lbt[0]) {
		xerr(4);		/*no label*/
		return;
	}
	setname();					/*move label into main table*/
	if((lblpt=lemt(TRUE,oirt))!=lmte) {
		xerr(5);				/*opcode redefined*/
		return;
	}
	mmte();						/*make main table entry*/
	expr(&p1gi);				/*get instruction format*/
	if(itype!=ITCN || ival<0 || ival>OPFF) {
		xerr(18);			/*illegal format specifier*/
		return;
	}
	lblpt->flags |= ival|SYIN;		/*remember format*/
	if(fchr != ',') {			/*field separator*/
		xerr(10);
		return;
	}
	expr(&p1gi);				/*get opcode value*/
	if(itype != ITCN) {
		xerr(17);		/*not a constant*/
		return;
	}
	lblpt->vl1 = ival;			/*put value in main table*/
	igrst();					/*ignore rest of statement-comment*/
}

/* equate directive*/
hequ()
{
	if(lbt[0] == 0) {
		xerr(4);		/*no label*/
		return;
	}
	setname();
	if((lblpt=lemt(FALSE,sirt))!=lmte) {	/*aready there*/
		if(lbt[0] == '~') {	/*local symbol*/
			lblpt = lmte;
			mmte();
		}
	}
	else
		mmte();
	if(lblpt->flags&SYXR) {
		xerr(29);
		return;
	}
	lblpt->flags |= SYDF|SYEQ;	/*defined & equate*/
	equflg = 1;
	modelen = LONGSIZ;
	expr(&p1gi);
	equflg = 0;
	if(itype == ITSY && ival.ptrw2->flags&SYER) {
		lblpt->flags |= SYER;	/*equated register*/
		ival = ival.ptrw2->vl1;
	}
	else if(itype != ITCN) {
		xerr(7);				/*not a constant*/
		return;
	}
	if (inoffset && reloc != ABS) {	/* [vlh] */
		xerr(11);
		return;
	}
	if(initflg)					/*doing symbol table initialization*/
		lblpt->flags |= SYIN;	/*internal symbol*/
	lblpt->vl1 = ival;
	if(reloc == DATA)			/*check relocation*/
	{
		lblpt->flags |= SYRA;	/*DATA relocatable*/
	}
	else if(reloc == TEXT)
		lblpt->flags |= SYRO;	/*TEXT relocatable*/
	else if(reloc == BSS)
		lblpt->flags |= SYBS;	/*BSS relocatable*/
	else if(fchr==',' && (fchr=gchr())=='r')
		lblpt->flags |= SYER;	/*equated register*/
	if (refpc)		/*[vlh] flag directive is pc relative */
		lblpt->flags |= SYPC;
	igrst();
}

/* process dsect directive*/
hdsect()
{
	dorlst(DATA);
}

dorlst(xrtyp)
int xrtyp;
{
	inoffset = 0;	/* [vlh] offset mode terminated my sect directive */
	dlabl();		/*define label on old base if there is one*/
	savelc[rlflg] = loctr;	/*save old base relocation*/
	rlflg = xrtyp;
	loctr = savelc[xrtyp];	/*set new base relocation ctr*/
	opitb();
	stbuf[0].itrl = itwc;
	wostb();
	igrst();
}

/*process psect directive*/
hpsect()
{
	dorlst(TEXT);
}

hbss()
{
	dorlst(BSS);
}

/*make pc even*/
heven()
{
	if(loctr&1) {		/*have to make it even*/
		dorlst(rlflg);
		loctr++;
	}
	else {
		igrst();
	}
}

/*process globl directive*/
hent()
{
	while(1) {
		gterm(TRUE);					/*get entry symbol*/
		if(itype!=ITSY) {				/*error if not symbol*/
			xerr(28);
			return;
		}
		if((lblpt=lemt(FALSE,sirt)) == lmte)	/*look up in main table*/
			mmte();						/*not there, make new entry*/
		else
			if(lblpt->flags&SYER)		/*already there*/
				uerr(29);
		lblpt->flags |= SYGL;			/*symbol is an entry*/
		if(lblpt->flags&SYXR)			/*been thru hext code*/
			lblpt->flags &= ~(SYXR|SYDF);	/*reset for init of .comm*/
		if (inoffset && reloc != ABS) {	/* [vlh] */
			xerr(11);
			return;
		}
		if(fchr == ',')					/*skip ',' between entries*/
			fchr = gchr();
		else {
			igrst();					/*statement finished*/
			return;
		}
	}
}

/*process comm directive*/
hext()
{
	gterm(TRUE);					/*get external symbol*/
	if(itype!=ITSY) {				/*error if not symbol*/
		xerr(28);
		return;
	}
	if((lblpt=lemt(FALSE,sirt)) == lmte)	/*look up in main table*/
		mmte();						/*not there, make new entry*/
	else
		if(lblpt->flags&SYDF && (lblpt->flags&SYXR)==0)	/*already there*/
			uerr(29);
	lblpt->flags |= SYXR | SYDF;	/*symbol is an external*/
	mkextidx(lblpt);	/*put into external table*/
	if(fchr == ',') {				/*skip ',' between operands*/
		fchr = gchr();
		gterm(TRUE);
		if(itype != ITCN) {
			xerr(17);
			return;
		}
		lblpt->vl1.hiword = ival;		/* # bytes of storage required*/
	}
	else
		lblpt->vl1.hiword = 1;			/* default # bytes*/
	igrst();
}

mkextidx(p)
struct symtab *p;
{
	if(extindx >= EXTSZ) {	/*check for overflow of external symbol tbl*/
		rpterr("overflow of external table\n");
		endit();
	}
	p->vl1.loword = (int)(pexti - extbl);	/* external symbol index #*/
	*pexti++ = p;		/*save external in external table*/
	extindx++;
}

/* end statement*/
hend()
{
	register short i;

	inoffset = 0;	/*[vlh] turn off inoffset mode*/
	lblpt = 0;	/*no label*/
	opitb();	/*output beginning of statement*/
	igrst();	/* [vlh] ignore operands */
	stbuf[0].itrl = itwc;	/*number of it entries*/
	wostb();	/*write out statement buffer*/
	if(pitix > itbuf)	/*some it in buffer*/
		if(write(itfn,itbuf,ITBSZ*(sizeof i)) != ITBSZ*(sizeof i)) {
			rpterr("I/O write error on it file\n");
			endit();
		}
	if(initflg) {
		putsymtab();
		printf("68000 assembler initialized\n");
		endit();
	}
	if((fchr=gchr())!=EOF)
		rpterr("end statement not at end of source\n");
	savelc[rlflg] = loctr;	/*last location on current reloc base*/
	fixunds();				/*make golbals and maybe undefineds external*/
	if(!didorg)				/*did not assign to location counter*/
		pass1a();			/*resolve short branches*/
	pass2();				/*assembler pass 2*/
}

/* define storage given number of bytes*/
hds()
{
	chkeven();			/*may need to make pc even*/
	dlabl();			/*define label maybe*/
	if (!inoffset)	/* [vlh] */
		opitb();		/*output it for beginning of statement*/
	refpc = 0;
	expr(&p1gi);
	if(itype!=ITCN) {
		xerr(17);		/*must be constant*/
		return;
	}
	if(reloc != ABS)  {
		xerr(9);		/*must be absolute*/
		return;
	}
	if (!inoffset) {	/* [vlh] don't generate it if in offset */
		opitoo();			/*output one operand*/
		stbuf[0].itrl = itwc;
		wostb();			/*write out statement buffer*/
		loctr += (ival*modelen);
	}
	igrst();
}

/* make pc even if necessary for .dc and .ds */
chkeven()
{
	register char *pi;

	if (modelen>BYTESIZ && (loctr&1)) {
		pi = opcpt;
		opcpt = evenptr;
		opitb();
		stbuf[0].itrl = itwc;
		wostb();
		opcpt = pi;
		loctr++;
	}
}

/* define byte directive*/
hdc()
{
	chkeven();
	hdata(modelen);
}

/*
 * define bytes or words of data
 *	call with:
 *		1 => defining bytes
 *		2 => defining words
 *		4 => defining long words
 */
hdata(mul)
int mul;
{
	dlabl();		/*define label*/
	opitb();		/*beginning of statement*/
	numops = 0;		/*initialize count for number of operands*/
	opito();		/*output it for operands*/
	stbuf[0].itrl = itwc;	/* # of it entries*/
	wostb();		/*write out statement buffer*/
	loctr += numops*mul;	/* count by bytes or words*/
}

/* handle org statement*/
horg()
{
	if(rlflg==TEXT && loctr!=0)
		didorg++;	/*can't do branch optimization as separate pass now*/
	expr(&p1gi);	/*value of new relocation counter*/
	if(reloc != rlflg && reloc != ABS) {
		xerr(27);
		return;
	}
	if(ival < loctr) {
		xerr(40);		/*trying to go backwards*/
		return;
	}
	opcpt = orgptr;		/*org directive for pass 2*/
	opitb();
	opitoo();
	stbuf[0].itrl = itwc;
	wostb();
	loctr = ival;
	dlabl();		/*define label*/
}

/* Assemble for mask2 (R9M), ignore... */
hmask2()	/* [vlh] */
{
	igrst();
}

/* Define register list */
hreg()		/* [vlh] */
{
	short mask;

	if(lbt[0]==0) {
		xerr(4);		/*no label*/
		return;
	}
	setname();					/*move label into main table*/
	if((lblpt=lemt(FALSE,sirt))!=lmte) {
		xerr(5);				/*opcode redefined*/
		return;
	}
	if (inoffset)
		if (reloc != ABS) {
			xerr(11);
			return;
		}
	mmte();						/*make main table entry*/
	if ((mask = mkmask()) == -1) {
		xerr(6);
		return;
	}
	lblpt->flags |= SYDF|SYEQ|SYRM;	/* register mask, defined & equated */
	lblpt->vl1 = mask;
	igrst();
}

short regmsk[] = {0100000,040000,020000,010000,04000,02000,01000,0400,0200,
				0100,040,020,010,4,2,1};
/* make a register mask for the reg routine */
mkmask()	/* [vlh] */
{
	register short *p, i, j, mask;

	p = &regmsk;	mask = 0;
	while ((i = chkreg()) != -1) {
		if (fchr == '-') {
			fchr = gchr();
			if ((j = chkreg()) == -1) {
				xerr(40);
				return(-1);
			}
			while (i <= j)
				mask |= p[i++];
		}
		else mask |= p[i];
		if (fchr != '/' && fchr != ',') break; /*[vlh] Signetics fix*/
		fchr = gchr();
	}
	return(mask);
}

/* get a register # from file, return -1 if none or illegal */
chkreg()	/* [vlh] */
{
	register short i, j;

	i = j = 0;
	if (fchr == 'a' || fchr == 'A')
		i = 8;
	else if (fchr != 'd' && fchr != 'r' && fchr != 'D' && fchr != 'R')
		return(-1);
	fchr = gchr();
	do {
		j = (j*10) + (fchr - '0');
		fchr = gchr();
	} while (isdigit(fchr));
	if (j < 0 || j > AREGHI) return(-1);
	i += j;
	if (i >= 0 && i <= AREGHI) return(i);
	else return(-1);
}

/* Define constant block */
hdcb()		/* [vlh] */
{
	chkeven();	/* on even boundary if not byte block. */
	dlabl();		/* define label... */
	opitb();
	opito();
	stbuf[0].itrl = itwc;
	numops = stbuf[ITOP1].itop;
	loctr += numops * modelen;
	wostb();		/* write out statement buffer */
}

/* Command line, similar to ds.b */
hcomline()	/* [vlh] */
{
	dlabl();			/*define label maybe*/
	modelen = BYTESIZ;		/* byte store... */
	opitb();			/*output it for beginning of statement*/
	refpc = 0;
	expr(&p1gi);
	if(itype!=ITCN) {
		xerr(17);		/*must be constant*/
		return;
	}
	if(reloc != ABS)  {
		xerr(9);		/*must be absolute*/
		return;
	}
	opitoo();			/*output one operand*/
	stbuf[0].itrl = itwc;
	wostb();			/*write out statement buffer*/
	loctr += ival;
	igrst();
}

/* Relocateable id record, ignore */
hidnt()	/* [vlh] */
{
	igrst();
}

/* Define offsets */
hoffset()	/* [vlh] */
{
	inoffset = 1;
	expr(&p1gi);	/* get new location counter */
	if (itype != ITCN) {
		xerr(17);	/* constant required */
		return;
	}
	if (reloc != ABS) {
		xerr(9);	/* must be absolute */
		return;
	}
	loctr = ival;
	igrst();
}

/* define sections: map to bss, text and data */
hsection()	/* [vlh] */
{
	inoffset = 0;	/* reseting section turns off offset mode */
	dlabl();		/* define label on old base if there is one */
	savelc[rlflg] = loctr;	/* save old base relocation */
	opitb();		/* intermediate table... */
	expr(&p1gi);	/* get section # */
	if (itype != ITCN) {
		xerr(17);	/* must be a constant */
		return;
	}
	if (ival > 15 || ival < 0) {
		xerr(9);	/* proper range 0..15 */
		return;
	}
	rlflg = (ival==14) ? DATA : (ival==15) ? BSS : TEXT;
	loctr = savelc[rlflg];
	stbuf[3].itop = loctr;		/* pass 1 location counter */
	stbuf[3].itrl = rlflg;		/* relocation base */
	stbuf[0].itrl = itwc;
	wostb();
	igrst();
}

/* hopt -- ignore, set up assembler options */
hopt()	/* vlh */
{
	igrst();
}

/* hpage - page directive, ignore */
hpage()		/* vlh */
{
	igrst();
}

/* httl - title directive, ignore */
httl()	/* vlh */
{
	igrst();
}

/****	Second pass directive handling routines ****/

/* second pass end statement*/
send()
{
	register short i;

	savelc[rlflg] = loctr;
	if(savelc[TEXT]&1) {
		rlflg = TEXT;
		outbyte(0,DABS);
	}
	if(savelc[DATA]&1) {
		rlflg = DATA;
		outbyte(0,DABS);
	}
	ival = 0;
	reloc = ABS;
	ckeop(9);
	print(0);
	cpdata();			/*copy data to loader file*/
	osymt();			/*output symbol table*/
	myfflush(&tbuf);	/*flush text relocation bits*/
	cprlbits();			/*copy relocation bits*/
	myfflush(&lbuf);
	i = (sizeof couthd.ch_magic) + 3*(sizeof couthd.ch_tsize);
	if((lseek(lfn,(long)i,0) == -1L) || lwritel(lfn,&stlen) == -1)
		rpterr("I/O error on loader output file\n");
	endit();			/*end*/
}

/*second pass define storage - ds*/
sds()
{
	print(0);
	if(rlflg == TEXT || rlflg==DATA) {
		expr(&p2gi);
		if(itype != ITCN) {
			uerr(13);
			return;
		}
		ival *= modelen;
		while(ival) {
			outbyte(0,DABS);
			loctr++;
			ival--;
		}
	}
	else
		loctr += stbuf[ITOP1].itop*modelen;		/*reserve storage*/
}

/* second pass - define block storage, initialized */
sdcb()	/* [vlh] */
{
	register short pfg, i, hflg, len;

	pfg = hflg = 0;
	expr(&p2gi);
	if (itype != ITCN || reloc != ABS) {
		uerr(13);	/* must be absolute constant */
		return;
	}
	len = ival;
	expr(&p2gi);
	if (modelen == BYTESIZ && (ival<-128 || ival>=256 || reloc != ABS)) {
		uerr(20);
		ival = 0;
		reloc = ABS;
	}
	while (len--) {
		if (modelen == BYTESIZ) {
			if (!hflg) {
				ins[i].hibyte = ival;
				outbyte((int)ival.loword,DABS);
				hflg++;
			}
			else {
				ins[i++].lobyte = ival;
				outbyte((int)ival.loword,DABS);
				hflg=0;
			}
			goto sdbl2;
		}
		else if (modelen == WORDSIZ) {
sdbl1:
			ins[i++] = ival.loword;
			outword((int)ival.loword, reloc);
sdbl2:
			if (i>3) {
				instrlen = i*2;
				print ((pfg++) ? 2 : 1);
				loctr += instrlen;
				i=0;
			}
		}
		else {		/* long word... */
			ins[i++] = ival.hiword;
			outword((int)ival.hiword,LUPPER);
			goto sdbl1;
		}
	}
	if (i) {	/* more printing */
		instrlen = i*2 - hflg;
		print ((pfg) ? 2 : 1);
		loctr += instrlen;
	}
}

/*second pass data*/
sdsect()
{
	savelc[rlflg] = loctr;
	rlflg = DATA;
	loctr = savelc[DATA];
	print(0);			/*print the new location counter*/
}

/*second pass text*/
spsect()
{
	savelc[rlflg] = loctr;
	rlflg = TEXT;
	loctr = savelc[TEXT];
	print(0);			/*print the new location counter*/
}

sbss()
{
	savelc[rlflg] = loctr;
	rlflg = BSS;
	loctr = savelc[BSS];
	print(0);			/*print the new location counter*/
}

/* make loctr even*/
seven()
{
	if(loctr&1) {
		if(rlflg==TEXT || rlflg==DATA)
			outbyte(0,DABS);
		loctr++;
	}
	print(0);
}

/* second pass org*/
sorg()
{
	register long l;

	if(rlflg==TEXT || rlflg==DATA) {	/*must put out zeros*/
		l = stbuf[ITOP1].itop - loctr;	/*# zeroes to output*/
		ins[0] = 0;
		print(1);
		while(l > 0) {
			outbyte(0,DABS);
			loctr++;
			l--;
		}
	}
	else {	/*BSS*/
		loctr = stbuf[ITOP1].itop;		/*new location counter*/
		print(0);
	}
}

/*
 *second pass define data (words or bytes)
 * call with
 *	2 => defining words
 *	1 => defining bytes
 *	4 => defining long words
 */
sdata(dtyp)
int dtyp;
{
	register short pfg,i, hflg;

	hflg = i = pfg = 0;
	while(1) {
		expr(&p2gi);			/*evaluate expression*/
		if(pitw < pnite)
			pitw--;		/*expr passed a token*/
		if(itype!=ITCN && reloc != EXTRN) {	/*must be constant*/
			uerr(13);
			ival=0;
			reloc = ABS;
		}
		if(reloc == EXTRN)
			reloc = (extref<<3)|EXTVAR;	/*gen extern reference*/
		if(dtyp==1) {			/*defining a byte*/
			if(ival<-128 || ival>=256 || reloc!=ABS) {		/*not a byte*/
				uerr(20);
				ival=0;
				reloc = ABS;
			}
			if(!hflg) {
				ins[i].hibyte = ival;
				outbyte((int)ival.loword,DABS);
				hflg++;
			}
			else {
				ins[i++].lobyte = ival;
				hflg = 0;
				outbyte((int)ival.loword,DABS);
			}
			goto sdal2;
		}
		else if(dtyp == 2) {	/*defining a word*/
sdal1:
			ins[i++] = ival.loword;
			outword((int)ival.loword, reloc);
sdal2:
			if(i>3) {
				instrlen = i*2;
				print ((pfg++) ? 2 : 1);
				loctr += instrlen;
				i=0;
			}
		}
		else {	/*long words*/
			ins[i++] = ival.hiword;
			outword((int)ival.hiword,LUPPER);
			goto sdal1;
		}
		if(!ckeop(15))	/*should be end of operand*/
			return;
		pitw++;
		if(ckein()) {
			if(hflg) {
				ins[i++].lobyte = 0;
			}
			if(i) {		/*more printing*/
				instrlen = i*2 - hflg;
				print ((pfg) ? 2 : 1);
				loctr += instrlen;
			}
			return;
		}
	}
}

sdc()
{
	sdata(modelen);
}

ssection()	/* [vlh] */
{
	short sect;

	sect = stbuf[3].itrl;
	if (sect==DATA) 
		sdsect();
	else if (sect==BSS) 
		sbss();
	else 
		spsect();
}

/****	Conditional assembly directives ****/

hifeq()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (ival) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifne()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (!ival) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hiflt()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (ival >= 0) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifle()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (ival > 0) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifgt()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (ival <= 0) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifge()	/* [vlh] */
{
	if (!acok()) 
		return;
	if (ival < 0) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifc()	/* [vlh] */
{
	if (!cmp_ops()) {
		if (ca_true) ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hifnc()	/* [vlh] */
{
	if (cmp_ops()) {
		if (ca_true) 
			ca_level = ca;
		ca_true = 0;
	}
	ca++;
}

hendc()	/* [vlh] */
{
	if (!ca) {
		xerr(25);	/* unexpected endc */
		return;
	}
	ca--;
	if (!ca_true)
		if (ca_level == ca) ca_true = 1;
	igrst();
}

acok()
{
	expr(&p1gi);
	if (itype != ITCN) {
		xerr(7);	/* must be a constant */
		return(0);
	}
	if (reloc != ABS) {
		xerr(11);	/* must be absolute, no forward references */
		return(0);
	}
	igrst();
	return(1);
}

cmp_ops()				/* return 1 true, 0 false */
{
	char str1[25], str2[25];
	register short len1, len2;

	if (fchr != '\'') { 
		xerr(9); 
		return(0);
	}
	len1 = len2 = 0;
	while ((fchr = gchr()) && fchr != '\'') {
		if (fchr == EOLC) 
			return(0);
		str1[len1++] = fchr;
	}
	if ((fchr=gchr()) != ',') {
		xerr(9); 
		return(0); 
	}
	if ((fchr=gchr()) != '\'') { 
		xerr(10); 
		return(0);
	}
	while ((fchr = gchr()) && fchr != '\'') {
		if (fchr == EOLC) 
			return(0);
		str2[len2++] = fchr;
	}
	igrst();
	if (len1 != len2) 
		return(0);
	str1[len1] = str2[len2] = NULL;
	return((strcmp(str1,str2) == 0));
}

strcmp(s,t)
char *s, *t;
{
	for( ; *s == *t; s++, t++ )
		if( *s == '\0' )
			return(0);
	return( *s - *t );
}
