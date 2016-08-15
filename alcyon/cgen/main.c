/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

char *version = "@(#)c168 code generator 4.2 - Sep 6, 1983";

/**
 *	ALCYON C Compiler for the Motorola 68000 - Code Generator
 *
 *	Called from c68:
 *
 *		c168 icode link asm
 *
 *	icode:		parsed intermediate code with some assembly code
 *              preceded by left parens.
 *
 *	link:		contains the procedure link and movem instructions.
 *
 *	asm:	    output assembler code for as68.
 *
 *	The basic structure of the code generator is as follows:
 *
 *	main				- main routine
 *		readicode		- code generation driven by intermediate code
 *
**/

#include "cgen.h"
#include "cskel.h"

char *opap;
short gflag;
short nextlabel	= 10000;
char null[] = "";
short lflag = 1;
char source[PATHSIZE] = "";

char *readtree();
char *readsym();

/* main - main routine, handles arguments and files*/
main(argc,argv)						/* returns - none*/
int argc;							/* arg count*/
char **argv;						/* arg pointers*/
{
	register char *q, *calledby;

	calledby = *argv++;
	if( argc < 4 )
		usage(calledby);
	if( fopen(*argv,&ibuf,0) < 0 )	/* 3rd arg for versados */
		ferror("can't open %s",*argv);
	if( fopen(*++argv,&lbuf,0) < 0)
		ferror("can't open %s",*argv);
	if( fcreat(*++argv,&obuf,0) < 0 )
		ferror("can't create %s",*argv);

	for( argc -= 4; argc--; ) {
		q = *++argv;
		if( *q++ != '-' )
			usage(calledby);
		while( 1 ) {
			switch( *q++ ) {

			case 'a':		/* [vlh] 4.2, alter ego of the '-L' flag */
				lflag = 0;
				continue;

			case 'c':
				cflag++;
				continue;

			case 'e':
				eflag++;
				continue;

			case 'f':
				fflag++;
				continue;

			case 'g':	/* [vlh] 4.2 generate line labels for cdb */
				gflag++;
				continue;

			case 'm':
				mflag++;
				continue;

			case 'o':
				oflag++;
				continue;

			case 'D':
				dflag++;
				continue;

			case 'L':	/* [vlh] 4.2, OBSOLETE */
				lflag++;
				continue;

			case 'T':	/* [vlh] 4.2 generates code for the 68010 */
				m68010++;
				continue;

			case '\0':
				break;

			default:
				usage(calledby);
			}
			break;
		}
	}

	readicode();
	myfflush(&obuf);
	exit(errcnt!=0);
}

/* readicode - read intermediate code and dispatch output*/
/*		This copies assembler lines beginning with '(' to assembler*/
/*		output and builds trees starting with '.' line.*/
readicode()								/*returns - none*/
{
	register short c;
	register struct tnode *tp;

	while( (c=getc(&ibuf)) > 0 ) {
		switch(c) {

		case '.':
			lineno = readshort();
			readfid();
			opap = exprarea;
			if( tp = readtree() ) {
				PUTEXPR(cflag,"readicode",tp);
				switch( tp->t_op ) {

				case INIT:
					outinit(tp->t_left);
					break;

				case CFORREG:
					outcforreg(tp->t_left);
					break;

				case IFGOTO:
					outifgoto(tp->t_left,tp->t_type,tp->t_su);
					break;

				default:
					outexpr(tp);
					break;
				}
			}
			else
				outline();
			break;

		case '(':
			while( (c=getc(&ibuf)) != '\n' )
				putchar(c);
			putchar(c);
			break;

		case '%':		/* [vlh] 4.2 */
			while( (c=getc(&ibuf)) != '\n' )
				;	/* skip over carriage return */
			while( (c=getc(&lbuf)) != '%' && c != -1)
				putchar(c);
			if (c == -1)
				ferror("early termination of link file");
			break;

		default:
			error("intermediate code error %c,%d",c,c);
			break;
		}
	}
}

/* readtree - recursive intermediate code tree read*/
char *
readtree()						/* returns ptr to expression tree*/
{
	register short op, type, sc;
	register struct tnode *tp, *rtp;
	char sym[SSIZE];

	if( (op=readshort()) <= 0 )
		return(0);
	type = readshort();
	switch( op ) {

	case SYMBOL:
		if( (sc=readshort()) == EXTERNAL )
			tp = cenalloc(type,sc,readsym(sym));
		else
			tp = snalloc(type,sc,readshort(),0,0);
		break;

	case CINT:
		tp = cnalloc(type,readshort());
		break;

	case CLONG:
		tp = lcnalloc(type,readlong());	/* [vlh] 4.1 was two readshort's */
		break;

	case CFLOAT:	/* [vlh] 3.4 */
		tp = fpcnalloc(type,readlong());	/* [vlh] 4.1 was two readshort's */
		break;

	case IFGOTO:
	case BFIELD:
		sc = readshort();
		if( tp = readtree() )
			tp = tnalloc(op,type,sc,0,tp,null);
		break;

	default:
		if( BINOP(op) ) {
			if( !(tp=readtree()) )
				return(0);
			if( !(rtp=readtree()) )
				return(0);
			tp = tnalloc(op,type,0,0,tp,rtp);
		}
		else if( tp = readtree() )
			tp = tnalloc(op,type,0,0,tp,null);
		break;
	}
	return(tp);
}

/* readfid - read source filename out of intermediate file */
readfid()
{
	register char *p;

	p = &source[0];
	while( (*p = getc(&ibuf)) != '\n') 
		p++;
	*p = 0;
}

/* readshort - reads an integer value from intermediate code*/
short
readshort()
{
	register short c;
	register short i;

	i = 0;
	while(1) {
		switch( c = getc(&ibuf) ) {

		case '.':
		case '\n': 
			return(i);

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			i <<= 4;
			i += (c-'0');
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			i <<= 4;
			i += (c-('a'-10));
			break;

		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			i <<= 4;
			i += (c-('A'-10));
			break;

		default:
			error("intermediate code error - %c,%d",c,c);
		}
	}
}

/* readlong - reads a long value from intermediate code*/
long
readlong()		/* [vlh] 4.1 */
{
	long l;
	register unsigned short w1, w2;
	register short c, onedot;

	w2 = 0; onedot = 0;
	while(1) {
		switch( c = getc(&ibuf) ) {

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			w2 <<= 4;
			w2 += (c-'0');
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			w2 <<= 4;
			w2 += (c-('a'-10));
			break;

		case '.':
			if (!onedot++) {
				w1 = w2;
				w2 = 0;
				continue;
			}
		case '\n': 
			if (onedot) {
				l.hiword = w1;
				l.loword = w2;
				return(l);
			}
		default:
			error("intermediate code error - %c,%d",c,c);
		}
	}
}

/* readsym - read a symbol from intermediate code*/
char *readsym(sym)
char *sym;
{
	register short i, c;
	register char *s;

	for( i = SSIZE, s = sym; (c=getc(&ibuf)) != '\n'; )
		if( --i >= 0 )
			*s++ = c;
	if( i > 0 )
		*s = '\0';
	return(sym);
}

/* error - output an error message*/
error(s,x1,x2,x3,x4,x5,x6)
char *s;
int x1, x2, x3, x4, x5, x6;
{
	errcnt++;
	if( lineno != 0 )
		printf((char *)STDERR,"\"%s\", ** %d: ",source,lineno);
	printf((char *)STDERR,s,x1,x2,x3,x4,x5,x6);
	cputc('\n',STDERR);
}

/* warning - output a warning message*/
warning(s,x1,x2,x3,x4,x5,x6)
char *s;
int x1, x2, x3, x4, x5, x6;
{
	if( lineno != 0 )
		printf((char *)STDERR,"\"%s\", ** %d: (warning) ",source,lineno);
	else
		printf((char *)STDERR,"(warning) ");
	printf((char *)STDERR,s,x1,x2,x3,x4,x5,x6);
	cputc('\n',STDERR);
}

/* ferror - output error message and die*/
ferror(s,x1,x2,x3,x4,x5,x6)
char *s;
int x1, x2, x3, x4, x5, x6;
{
	error(s,x1,x2,x3,x4,x5,x6);
	exit(1);
}

/* tnalloc - allocate binary expression tree node*/
/*	returns ptr to node made.*/
char *tnalloc(op,type,info,dummy,left,right)
int op;						/* operator*/
int type;					/* resultant node type*/
int info;					/* info field*/
int dummy;					/* dummy field - used to match pass1 args*/
struct tnode *left;			/* left sub-tree*/
struct tnode *right;		/* righst sub-tree*/
{
	register struct tnode *tp;

	tp = talloc(sizeof(*tp));
	tp->t_op = op;
	tp->t_type = type;
	tp->t_su = info;			/* info for bit-field & condbr's*/
	tp->t_left = left;
	tp->t_right = right;
	return(tp);
}

/* cnalloc - allocate constant expression tree node*/
char *cnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
int value;						/* value of constant*/
{
	register struct conode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CINT;
	cp->t_type = type;
	cp->t_value = value;
	return(cp);
}

/* lcnalloc - allocate constant expression tree node*/
char *lcnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
long value;						/* value of constant*/
{
	register struct lconode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CLONG;
	cp->t_type = type;
	cp->t_lvalue = value;
	return(cp);
}

/* fpcnalloc - allocate constant expression tree node*/
char *fpcnalloc(type,value)	/* returns pointer to node alloced*/
int type;						/* type of constant*/
long value;						/* value of constant*/
{								/* [vlh] 3.4 */
	register struct lconode *cp;

	cp = talloc(sizeof(*cp));
	cp->t_op = CFLOAT;
	cp->t_type = type;
	cp->t_lvalue = value;
	return(cp);
}

/* talloc - allocate expression tree area*/
char *talloc(size)				/* returns pointer to area alloced*/
int size;						/* number of bytes to alloc*/
{
	register char *p;

	p = opap;
	if( p + size >= &exprarea[EXPSIZE] )
		ferror("expression too complex");
	opap = p + size;
	return(p);
}

/* symcopy - copy symbol*/
symcopy(sym1,sym2)					/* returns - none*/
char *sym1;						/* from symbol*/
char *sym2;						/* to symbol*/
{
	register char *p, *q;
	register short i;

	for( p = sym1, q = sym2, i = SSIZE; --i >= 0; )
		*q++ = (*p ? *p++ : '\0');
}

/* usage - ouput usage message*/
usage(calledby)
char *calledby;
{
	ferror("usage: %s icode link asm [-DLameco]",calledby);
}

/* cputc - put a character to a file descriptor (used by error) */
cputc(c, fn)
char c;
int fn;
{
#ifdef VERSADOS
	versaputchar(c);
#else
	if (fn == STDERR)
		write(STDERR, &c, 1);
	else
		putchar(c);
#endif
}

/**
 * putchar - special version
 *		This allows the use of printf for error messages, debugging
 *		output and normal output.
**/
putchar(c)							/* returns - none*/
char c;								/* character to output*/
{
	if( dflag > 1 )
		write(1,&c,1);			/*to standard output*/
	putc(c,&obuf);				/*put to assembler file*/
}

#ifdef VERSADOS

#define STDOUT 1

struct iob versfout { STDOUT, BSIZE, &versfout.cbuf[0]};

versaputchar(c)
char c;
{
	if (c == '\n') {	/* end of line */
		if (versaflush())	/* write one line */
			return(-1);
		return(c);
	}

	/* buffered output */
	if (versfout.cc <= 0) {
		versfout.cp = &(versfout.cbuf[0]);
		if (write(versfout.fd,versfout.cp,BSIZE) != BSIZE)
			return(-1);
		versfout.cc = BSIZE;
	}
	*(versfout.cp)++ = c;
	versfout.cc--;
	return(c);
}

versaflush()
{
	register short size, fildes;

	if ((size = (BSIZE - versfout.cc)) == 0)
		return(0);
	versfout.cc = BSIZE;
	versfout.cp = &(versfout.cbuf[0]);
	fildes = (versfout.fd <= STDERR) ? 6 : versfout.fd;
	if (write(fildes,versfout.cp,size) < 0)
		return(-1);
	return(0);
}
#else
#	ifdef VAX11
	getc(ibuf)
	struct iob *ibuf;
	{
		if (ibuf->cc <= 0) {
			ibuf->cp = &(ibuf->cbuf[0]);
			ibuf->cc = read(ibuf->fd,ibuf->cp,BSIZE);
		}
		if (ibuf->cc <= 0)
			return(-1);
		ibuf->cc--;
		return((int)(*(ibuf->cp)++)&0xff);
	}

	fopen(fname,ibuf)
	char *fname;
	register struct iob *ibuf;
	{
		ibuf->cc = 0;   /* no chars */
		ibuf->fd = open(fname,0);
		return(ibuf->fd);
	}
#	endif
#endif

myfflush(mybuf)
register struct iob *mybuf;
{
	register i;

	i = BSIZE - mybuf->cc;
	mybuf->cc = BSIZE;
	mybuf->cp = &(mybuf->cbuf[0]);
	if (write(mybuf->fd,mybuf->cp,i) != i)
		return(-1);
	return(0);
}

#ifdef DRI
printf(string,a,b,c,d,e,f,g)
char *string;
int a,b,c,d,e,f,g;
{
	char area[256];
	register char *p;

	sprintf(area,string,a,b,c,d,e,f,g);
	for(p = &area[0]; *p ; p++ )
		putchar(*p);
}
#endif

