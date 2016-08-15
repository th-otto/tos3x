/* Compiler Test Program Part I */
#include <stdio.h>
#define TVAL	5
#define FVAL	0
union un1{
	long ul1;
	int ui1;
} uu;
struct ss {
	char m1;
	short int  m2;
	long m3;
} s1;
char *swt1();
char *swt2();
char *swt3();
char *swt4();

struct { char *ptrwd; };

struct {
	unsigned f1:1;
	unsigned f2:2;
	unsigned f3:3;
} flags;

typedef char CHAR;
typedef int  INT;

typedef struct {
	CHAR tsc;
	INT  tsi;
	CHAR *tsp;
} TYPSTR;
TYPSTR zz[3];
typedef int typeary[4];

char c;
static char str[] ="string 1";
static long l;
long l1,l2,l3;

long addlargs();
long addpargs();
long ldiv();
long ldivr;
int initi = 1;
int initk[3] = { 1, 2, 3};
int initl[3][4] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
char *initx[] = {
	"that's all, folks...",
	0,
};
struct {
	int s_x;
	long s_l;
} inits[] = {
	0,	0L,
	1,	1L,
	2,	2L,
};
char initc[20] = "this is it";
char initd[] = "this might be it";
char inite[][3] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
char *initcc = {"this is also it"};
int ccinit = '\1\2';

char ac[4];
int ai[4];
int *pai[] = {&ai[1],&ai[3]};
long al[4];
typedef char *CHARP;
CHARP p0;
/* beginning new declarations as of V3.3 */
struct butblk {
		struct butblk *nextbutblk;
		int butno;
		short args;
		int actno;
		struct node *ptr;
};

struct node {
		struct node *nfptr;
		struct butblk *bptr;
		long actno;		/* non-unique structure names */
};

struct other {
	int a;
	int *b;
};
int x[5];
struct other sound[] = {
	{ 1, x+1},
};
/* end new declarations as of v3.3 */
/* begin new declarations for 3.4 */
typedef struct xxx34 STRASSIGN;

struct xxx34 {
	long l;
	char ch;
	STRASSIGN *pxxx34;
};

struct ss *f();
char largear[20000];
typedef struct atype *STRUCTYPE;

struct atype {
	char ch;
	STRUCTYPE ptr;
};
int err;
/* end new declarations for 3.4 */

main()
{

	register i;
	int j;
	register char *p;
	int ai2[2][3];
	char n;
	register unsigned u1,u2;
	unsigned int u3;
	register long lr1;
	int *pi;
	char **a;

	putstring("Beginning 68000 C Compiler Test");
	for(j=0; j<5; j++) {
		c = 'A';
		for(i=0; i<62; i++) {
			putchar(c);
			c++;
		}
		putchar('\n');
	}
	if(12345 != 12345) error(0x0);
	j = i;
	if(i != j) error(0x0a);
	if(sizeof flags != 2) error(0x0b);
	if(sizeof(TYPSTR) != 8) error(0x0c);
	if(sizeof zz[2] != sizeof(TYPSTR)) error(0x0d);
	if(sizeof(typeary[4])!=32)error(0x0e);
	if(sizeof(int[3])!=6)error(0x0f);
	if(sizeof(typeary*)!=4)error(0x10);
	i = 0; if(i) error(0x1);
	if(i!=0) error(0x2);
	if(++i != 1) error(0x3);
	i<<= 2;
	if(i != 4) error(0x4);
	i += i+2;
	if(i != 10) error(0x5);
	i *= 3*2;
	if(i != 60) error(0x6);
	i /= 3;
	if(i != 20) error(0x7);
	i = (i+12)|7;
	if(i != 39) error(0x8);
	i = i - i/2 - 2;
	if(i != 18) error(0x9);
	i = i % 6;
	if( i != 0 ) error(0x10);
	j = 0;
	if(j) error(0x11);
	if(j!=0) error(0x12);
	if(++j != 1) error(0x13);
	j<<= 2;
	if(j != 4) error(0x14);
	j += j+2;
	if(j != 10) error(0x15);
	j *= 3*2;
	if(j != 60) error(0x16);
	j /= 3;
	if(j != 20) error(0x17);
	j = (j+12)|7;
	if(j != 39) error(0x18);
	j = j - j/2 - 2;
	if(j != 18) error(0x19);
	j = 1229;
	j <<= 1;
	if(j != 2458) error(0x20f);
	l = 0;
	if(l) error(0x21);
	if(l!=0) error(0x22);
	if(++l != 1) error(0x23);
	l<<= 2;
	if(l != 4) error(0x24);
	l += l+2;
	if(l != 10) error(0x25);
	l = l*6;
	if(l != 60) error(0x26);
	l /= 3;
	if(l != 20) error(0x27);
	l = (l+12)|7;
	if(l != 39) error(0x28);
	l = l - l/2 - 2;
	if(l != 18) error(0x29);
	l = l % 6;
	if( l != 0 ) error(0x290);
	l = 12345L;
	if(l != 12345) error(0x291);
	l = 0xfffffffel;
	if(l != -2) error(0x292);
	l = 40000; if( (l/= 10) != 4000 || l != 4000 ) error(0x293);
	l = 40000; l /= 10; if( l != 4000 ) error(0x294);
	l = 40000; l /= 10L; if( l != 4000 ) error(0x295);
	l = 40000; if( (l % 400) !=  0) error(0x296);
	l = 40000; if( (l % 400L) != 0 ) error(0x297);
	targs(1,1000,-18363);
	i = addargs(2,7,65);
	if(i != 74) error(0x30);
	l1 = 10000L;
	l2 = 4273;
	lr1 = 0xabcd5678;
	if((l=addlargs(l1,l2)) != l1+l2) error(0x310);
	if( addlargs(l1+l2,l1) != addlargs(l1,l1+l2) ) error(0x311);
	p = "x.c";
	p[strlen(p)-1] = 's';
	if( eqstr(p,"x.s") == 0 ) error(0x312);
	if(lr1 != 0xabcd5678) error(0x313);
	if(eqstr("azx953#;k9","azx953#;k9") == 0) error(0x314);
	if(eqstr("try me","try m") != 0) error(0x315);
	if( strlen("x.s") == strlen("x.ss") ) error(0x316);
	if( strlen("x.s") + 1 != strlen("x.ss") ) error(0x317);
	if( j = 1, i = 0 ) error(0x318);
	if( (j=0,i=1) != 1 ) error(0x319);
	l1 = -3347;
	l2 = 7913;
	if((l=addpargs((char *)&l1,&l2)) != l1+l2) error(0x34);
	l = 60;
	if((l *= 9) != 540) error(0x36);
	c = 0377;
	if(c != 0377) error(0x36);
	i = c;
	if(i != -1) error(0x37);
	j = c;
	if(j != -1) error(0x38);
	l = c;
	if(l != -1) error(0x39);
	c = 0x1f;
	if((c&0x10) == 0) error(0x39a);
	if((c&0x40)) error(0x39b);
	ai[0] = 0;
	ai[1] = 1;
	ai[2] = 2;
	ai[3] = 3;
	if(ai[0] != 0) error(0x40);
	if(ai[1] != 1) error(0x41);
	if(ai[2] != 2) error(0x42);
	if(ai[3] != 3) error(0x43);
	for(i=0; i<=3; i++) {
		if(ai[i] != i) error(0x44);
		ac[i] = ai[i];
	}
lab45:
	for(i=0; i<4; i++) {
		if(ac[i] != i) error(0x45);
		al[i] = i*30000;
	}
	for(i=0; i<4; i++) {
		if(al[i] != i*30000) error(0x46);
		if(al[i]/30000 != i) error(0x47);
		if(al[i]/100 != i*300) error(0x48);
	}
	n = 3;
	if(str[n] != 'i') error(0x48a);
	if(ai[n] != n) error(0x48b);
	if(ac[n] != 3 || al[n] != n*30000) error(0x48c);
	i = 0;
	for(j=0; j<100; j++)
		i += 5;
	if(j != 100) error(0x49);
	if(i != 500) error(0x50);
	i = 0155555;
	i ^= i;
	if(i) error(0x51);
	s1.m1 = 64;
	s1.m2 = 1250;
	s1.m3 = 9264;
	p = &s1;
	if(s1.m1!=64 || p->m1!=64) error(0x52);
	if(s1.m2!=1250 || p->m2!=1250) error(0x53);
	if(s1.m3==9264 && p->m3==9264) ; else error(0x54);
	j = -50;
	if(j >= 0) error(0x55);
	if(j < 0) ; else error(0x56);
	l = 7777;
	if(l < 7776) error(0x57);
	if(l > 7776) ; else error(0x58);
	p = &ac[0];
	p0 = &ac[3];
	if(p >= p0) error(0x59);
	if(p <= p0) ; else error(0x60);
	if(p == 0) error(0x61);
	if(p0 != 0) ; else error(0x62);
	for(i=0; i<2; i++) {
		for(j=0; j<3; j++) {
			ai2[i][j] = (i+2)*(j+3);
		}
	}
	for(i=0; i<2; i++) {
		for(j=0; j<3; j++) {
			if(ai2[i][j] != (i+2)*(j+3)) error(0x63);
		}
	}
	if(swt1()!=1) error(0x64);
	if(swt2()) error(0x65);
	if(swt3()!= -1) error(0x65);
	if(swt4()!=4) error(0x66);

	l1 = 20000;
	l2 = 10;
	l3 = ldiv(l1,l2);
	if(l3 != 2000) error(0x201);
	if(ldivr) error(0x202);
	l2 = 9999;
	if(ldiv(l1,l2) != 2) error(0x203);
	if(ldivr != 2) error(0x204);
	l2 = -123;
	if(ldiv(l1,l2) != -162) error(0x207);
	if(ldivr != -74) error(0x208);
lab209:
	u1 = 0100000;
	u2 = 0010000;
	if(u1 < u2) error(0x209);
	uu.ui1 = 1;
	if(uu.ul1 != 0x10000) error(0x210);
#if TVAL
	i = 10;
#else
	i = 5;
#endif
	if(i != 10) error(0x211);
	i = 0;
#ifndef FVAL
	i++;
#endif
#ifdef XXYYZZ
	i++;
#endif
#if FVAL|TVAL-5
	i++;
#endif
	if(i) error(0x212);

	if(sizeof(uu) != sizeof(uu.ul1)) error(0x213);
	zz[0].tsi = 14;
	zz[1].tsc = 0177;
	zz[2].tsp = 199999;
	if(zz[0].tsi != 14) error(0x214);
	if(zz[1].tsc != 0177) error(0x215);
	if(zz[2].tsp != 199999) error(0x216);

	i = 1;
#if TVAL
	i++;
#	if FVAL
	error(0x217);
#	else
	i++;
#endif
#else
	i++;
#endif;
	if(i != 3) error(0x218);
#undef FVAL
#ifdef FVAL
	error(0x219);
#endif
#if ((TVAL*6)/3) != 3*2+8/2
	error(0x220);
#endif
	flags.f1=1;
	flags.f2=0;
	flags.f3=5;
	if(flags.f1 != 1) error(0x221);
	if(flags.f2) error(0x222);
	if(flags.f3 != 5) error(0x223);
	flags.f3 += 2;
	if(flags.f3 != 7) error(0x224);
	p = &flags;
	p->f1 = 0;
	p->f2 = 1;
	p->f3 = 3;
	if(p->f1) error(0x224a);
	if(p->f2 != 1) error(0x224b);
	if(p->f3 != 3) error(0x224c);
	p0 = &flags;
	p0->f1 = 1;
	p0->f2 = 2;
	p0->f3 = 7;
	if(p0->f1 != 1) error(0x224d);
	if(p0->f2 != 2) error(0x224e);
	if(p0->f3 != 7) error(0x224f);
	i = 13;
	checklv((long)(i+5));
	checkregs();
	chkinits();
	if((sizeof(char *)+3) != 7) error(0x225);
	j = -12;
	chklext((long)j);
	u1 = 60000;
	u2 = u1/20;
	if(u2 != 3000) error(0x226);
	l1 = 55123L;
	l1 = u1 + l1;
	if(l1 != 115123L) error(0x227);
	l1 -= u1;
	if(l1 != 55123L) error(0x228);
	al[3] = 99999L;
	u1 = 3;
	if(al[u1] != 99999L) error(0x229);
	l1 = 180000L;
	u1 = 60000;
	u2 = l1/u1;
	if(u2 != 3) error(0x230);
	u2 = 12000;
	u3 = 5;
	u1 = u2*u3;
	if(u1 != 60000) error(0x231);
	l1 = 5;
	u1 = 60000;
	l1 = u1*l1;
	if(l1 != 300000L) error(0x232);

//int<->char
	n = 100;
	i = (n<<3) + 2;
	j = (n<<4) - 2;
	if(i != 802) error(0x240);
	if(j != 1598) error(0x241);
	i = 50;
	if(n <= i) error(0x242);
	j = 100;
	if(n != j) error(0x243);
	pi = &j;
	if(*pi != n) error(0x243a);
	if(*pi != j) error(0x243b);
	l = 100;
	if(n != l) error(0x244);
	n = 255;
	if(n>l || n>=j || n>i) error(0x255);
	i = n + 500;
	if(i != 499) error(0x256);
	if((n-4) != -5) error(0x257);
	i = 1, i = 2, i = 3; if( i != 3 ) error(0x258);
	if( (i = 3, j = 2, i = 1) != 1 || i != 1 || j != 2 ) error(0x259);
	if( i++ != 1 || i-- != 2 || i != 1 ) error(0x260);
	if( (i++ + j++) != 3 || i != 2 || j != 3 )error(0x261);

	ckcast();

	if(p=5);else error(0x301);
	if(p -= 5) error(0x302);
	if(!(++p&1)) error(0x303);
	p0 = 0;
	if(p = p0) error(0x304);
	p0 = &p0;
	if(p = p0--);else error(0x305);
	p = &ai[2]->ptrwd + zz[3].tsp;	//this used to give error msg on conversion
	p = &zz[2];
	zz[2].tsi = 2;
	if(p->tsi&1) error(0x306);
	if(!p->tsi&2) error(0x307);
	p = 070;
	if(p&1) error(0x308);
	if((p&040)==0) error(0x309);
	j = 105;
	p = 105;
	if(p != j) error(0x320);
	ai[3] = 0x8765;
	j = 1;
	i = *pai[j];
	if(i != 0x8765) error(0x321);
	p = 1000;
	a = &p0;
	*a=pi=p+1;
	if(p0 != 1001) error(0x322);
	chkptrs();		//check pointer code
	tembedasg();	//complex ptr assignment
	macrotest();
	lextest();
	cmplxptrpp();

	n = 100;
	i = 1024;
	l = 31234;
	c = 'Z';
	printf("100=%d $64=%x 1024=%d 02000=%o 31234l=%ld string 1=%s Z=%c\n",
		n,n,i,i,l,str,c);
	l = 0xa9b7c3;
	i = 0xf1e4;
	printf("0xa9b7c3=%lx 0XF1E4=%x\n",l,i);
	v33();
	v34();

	if (!err)
		putstring("Far Freaking Out!!!!");
	else
		putstring("Tough Luck!!!");
}

v33()	/* new for version 3.3 */
{
	long v33l;
	struct butblk bb1;
	struct node bb2;

	bb2.bptr = &bb1;
	bb1.butno = 23;
	printf("%d = %d ",bb1.butno,bb2.bptr->butno);
	x[1] = 99;
	printf("%d = %d ",x[1],*sound[0].b);
	v33l = 0x1L + 0x10L + 0x100L + 0x1000L + 0x10000L + 0x100000L;
	printf("0x111111 = %lx\n",v33l);
}

v34()		/* new for version 3.4 */
{
	char *q;
	register struct ss *p3;
	struct ss p4, p5;
	char ar34[sizeof (long)];
	float f1, f2;
	int i;
	double d1, d2;
	STRUCTYPE sop1;
	STRASSIGN *ptr, s;

	if (q&((sizeof(struct ss))-1));

	goto a;
	error(0xe00);
a:
	if (sizeof(largear) < 0) error(0xe01);

	l = 0x0100000;
	i = 1;
	l <<= i;
	if (l != 0x0200000) error(0xe02);
	l = 0x010;
	l <<= i;
	if (l != 0x020) error(0xe03);
	l1 = 1L;
	l2 = 1L;
	i = 5;
	l1 <<= i;
	l2 = l2 << i;
	if (l1 != l2) error(0xe04);

	if ((p3 = f()) != -1) error(0xe05);

	p4.m1 = 'p';
	p3 = &p5;
	*p3 = p4;
	if (p3->m1 != p4.m1) error(0xe10);
	p4.m1 = 'E';
	p5 = p4;
	if (p5.m1 != p4.m1) error(0xe11);
	p3->m1 = '7';
	p4 = *p3;
	if (p4.m1 != p3->m1) error(0xe12);
}

struct ss *
f()
{
	return(-1);
}

chklext(al9)
long al9;
{

	if(al9 != -12L) error(0x375);
}

chkinits()
{

	static long liv1 =999999;
	static struct xx {
		char *csiv1;
		int isiv1;
		long lsiv1;
	} ss[] ={"abc",1,0x3ff41,"def",2,143L,"ghi",3};
	register int i;

	if(liv1 != 999999L) error(0xff1);
	if(ss[1].isiv1 != 2) error(0xff2);
	if(eqstr(ss[2].csiv1,"ghi")==0) error(0xff3);
	if(ss[0].lsiv1 != 0x3ff41) error(0xff4);
	if(ss[1].lsiv1 != 143L) error(0xff5);
	if(ss[2].lsiv1 != 0) error(0xff6);
	if(ss[1].lsiv1&0200) ; else error(0xff7);
	if( initi != 1 ) error(0xff8);
	for( i = 0; i < 3; i++ ) {
		if( initk[i] != i+1 ) error(0xff9);
		if( initl[i][0] != (i*4)+1 ) error(0xff10);
		if( inits[i].s_x != i ) error(0xff11);
		if( inits[i].s_l != i ) error(0xff12);
		if( inite[i][0] != (i*3)+1 ) error(0xff13);
	}
	if( eqstr(initc,"this is it") == 0 ) error(0xff14);
	if( eqstr(initd,"this might be it") == 0 ) error(0xff15);
	if( eqstr(initcc,"this is also it") == 0 ) error(0xff16);
	if( sizeof inite != 9 ) error(0xff17);
	if( sizeof initc != 20 ) error(0xff18);
	if( sizeof initd != 17 ) error(0xff19);
	if( ccinit != 258 ) error(0xff20);
}

checkregs()
{

	register rx1,rx2,rx3,rx4,rx5,rx6,rx7,rx8,rx9;

	rx1 = 1;
	rx2 = 2;
	rx5 = 5;
	rx8 = 8;
	rx9 = 9;
	if(rx1+rx2 != 3) error(0x360);
	if(rx2+rx5 != 7) error(0x361);
	if(rx9-rx5 != 4) error(0x362);
	if(rx8+rx9 != 17) error(0x363);
}

checklv(alv)
long alv;
{
	if(alv != 18L) error(0x364e);
}

error(pc)
{
	printf("error %x\n",pc);
	err++;
}

putstring(pc)
char *pc;
{
	register char *p;

	p = pc;
	while(*p)
		putchar(*p++);
	putchar('\n');
}

targs(a1,a2,a3)
char a1;
{
	if(a1!=1 || a2!=1000 || a3!= -18363) error(0xfe00);
	if(40000 != 40000L) error(0xfe01);
}

addargs(a1,a2,a3)
{

	return(a1+a2+a3);
}

eqstr(ap1,ap2)
char *ap1, *ap2;
{
	register char *p1, *p2;

	p1 = ap1;
	p2 = ap2;
	while(*p1) {
		if(*p1++ != *p2++)
			return(0);
	}
	if(*p2)
		return(0);
	return(1);
}

long addlargs(a1,a2)
long a1,a2;
{
	register i,j,k;
	register long lr2;

	i = 1000;
	lr2 = 0xbbbbbbbb;
	targs(1,i,-18363);
	if(lr2 != 0xbbbbbbbb) error(0xfe02);

	return(a1+a2);
}

long addpargs(a1,a2)
register long *a1,*a2;
{

	return(*a1+*a2);
}

// simple switch stmt test
char *swt1()
{
	register int i;

	for( i = -1; i < 10; i++ ) {
		switch(i) {
	
			error(0x140);

			case -1:
				if(i!= -1)error(0x141);
				continue;
			case 0:
				if(i!=0)error(0x142);
				continue;
			case 1:
				if(i!=1)error(0x143);
				continue;
			case 4:
				if(i!=4)error(0x144);
				continue;
			case 2:
				if(i!=2)error(0x145);
				continue;
			case 3:
				if(i!=3)error(0x146);
				continue;
			default:
				if(i!=5)error(0x147);
				continue;
	
			case 6:
				break;
		}
		break;
	}
	if(i!=6)error(0x148);
	return(1);
}

char *swt2()
{
	register int i;

	for( i = 0; i < 100; i += 10 ) {
		switch(i) {

		error(0x150);

		case 60:
			break;
		case 10:
			if(i!=10)error(0x151);
			continue;
		case 0:
			if(i)error(0x157);
			continue;
		case 20:
			if(i!=20)error(0x152);
			continue;
		case 40:
			if(i!=40)error(0x153);
			continue;
		case 50:
			if(i!=50)error(0x154);
			continue;
		default:
			if(i!=30)error(0x155);
			continue;

		}
		break;
	}
	if(i!=60)error(0x156);
	return(0);
}

char *swt3()
{
	register int i, j;

	for( i = 0; i < 10; i++ ) {
		switch(j = 1<<i) {

		error(0x160);

		case 8:
			if(j!=8)error(0x161);
			continue;
		case 32:
			break;
		case 4:
			if(j!=4)error(0x162);
			continue;
		case 2:
			if(j!=2)error(0x163);
			continue;
		case 1:
			if(j!=1)error(0x164);
			continue;
		default:
			if(j!=16)error(0x165);
			continue;

		}
		break;
	}
	if(j!=32)error(0x166);
	return(-1);
}

char *swt4()
{
	register int i, j;

	for( i = 0; i < 10; i++ ) {
		switch(j = 1<<i) {

		error(0x170);

		case 4:
			break;
		case 2:
			if(j!=2)error(0x173);
			continue;
		case 1:
			if(j!=1)error(0x174);
			continue;
		default:
			break;

		}
		break;
	}
	if(j!=4)error(0x176);
	return((long) j);
}

ckcast()
{

	struct results {
		struct results *length;
		int cutoff;
	} cst19[3], *arypt;

	arypt = cst19;
	arypt->length = 6;
	(char *)arypt += (long)arypt->length;
	if(arypt != &cst19[0]+1) error(0xfe03);
}

chkptrs()
{

	register char *pc;
	register int *pi;
	register char **pl;
	char *apc;
	int *api;
	long *apl;

	pc = &str[4];
	if(*pc++ != 'n') error(0x400);
	if(*pc-- != 'g') error(0x401);
	if(*--pc != 'i') error(0x402);
	if(*(pc+4) != '1') error(0x403);

	pc = &str[4];
	if( pc > &str[4] ) error(0x410);
	if( pc >= &str[5] ) error(0x441);
	if( pc < &str[4] ) error(0x442);
	if( pc <= &str[3] ) error(0x443);
	if( pc == &str[3] ) error(0x444);
	if( pc != &str[4] ) error(0x445);

	apc = &str[4];
	if(*apc++ != 'n') error(0x420);
	if(*apc-- != 'g') error(0x421);
	if(*--apc != 'i') error(0x422);
	if(*(apc+4) != '1') error(0x423);

	apc = &str[4];
	if( apc > &str[4] ) error(0x430);
	if( apc >= &str[5] ) error(0x431);
	if( apc < &str[4] ) error(0x432);
	if( apc <= &str[3] ) error(0x433);
	if( apc == &str[3] ) error(0x434);
	if( apc != &str[4] ) error(0x435);

	ai[0] = 14;
	ai[1] = 33;
	ai[2] = 194;
	ai[3] = 555;
	if(**pai != 33) error(0x404);
	pi = &ai[1];
	if(*++pi != 194) error(0x405);
	pi--;
	if(*pi+5 != 38) error(0x406);
	ai[0] = 0;
	if(*--pi) error(0x407);
	api = pi + 3;
	if(*api++ != 555) error(0x408);

	pl = pai;
	if(*++pl != &ai[3]) error(0x409);
	ckstrpt();
}

ckstrpt()
{

	register TYPSTR *p;
	register int nn;
	register long lll;

	zz[1].tsi = 1987;
	zz[1].tsc = 255;
	zz[2].tsc = 13;
	zz[2].tsi = -5;
	p = zz;
	if((++p)->tsi != 1987) error(0x450);
	if(p->tsc != -1) error(0x451);
	if((p+1)->tsi != -5) error(0x452);
	p++;
	if(p->tsc != 13) error(0x453);
	if(--p->tsi != -6) error(0x454);
	if((--p)->tsi != 1987) error(0x454a);
	p0 = &zz[1];
	if(*p0 != -1) error(0x455);
	if(p0->tsi != 1987) error(0x456);
	if((p0->tsi&1)==0 || (p0->tsi&2)==0) error(0x457);
	if((long)p&1) error(0x458);
	p = zz;
	nn = 2;
	p += nn;
	if(p->tsc != 13) error(0x459);
	if(nn&2) ; else error(0x460);
	nn = 0100000;
	if(nn&0x8000) ; else error(0x461);
	lll = 0x3f001;
	if(lll&0x40) error(0x462);
	if(lll&0xf00) error(0x463);
	if(lll&0x8000) ; else error(0x464);
	if(lll&0x10000) ; else error(0x465);
	if((lll&0xf0000) != 0x30000) error(0x466);
}

//embeded ptr assignment test
tembedasg()
{

	register char *p,mcc;
	register int t;
	char ctype[4];
	char c9;

	mcc = 2;
	ctype[2] = 'Z';
	p = ctype;
	t = ctype[*p++=mcc];
	if(t != 'Z') error(0xfe04);
	if(ctype[0] != 2) error(0xfe05);
	c9 = 2;
	c9 |= 1;
	if(c9 != 3) error(0xfe08);
	c9 &= 1;
	if(c9 != 1) error(0xfe09);
}

macrotest()
{
#define	x	1
#if x!=1
	error(0xfd1);
#endif
#ifndef x
	error(0xfd2);
#endif
#ifdef x
#else
	error(0xfd3);
#endif
#undef x
#ifdef x
	error(0xfd4);
#endif
#ifndef x
#else
	error(0xfd5);
#endif
#define	y(a,b,c)	c+a+b
#if y(1,2,3)!=6
	error(0xfd6);
#endif
#include "test.h"
#ifndef test2x
	error(0xfd7);
#endif
#define z(a,b,c)	\
#define	a	b+c
z(zxx,1,2)
#if zxx!=3
	error(0xfd8);
#endif
#if zxx==3
#else
	error(0xfd9);
#endif
/*
#define	zzz
*/
#ifdef zzz
	error(0xfd9);
#endif
#if ((5*6)/3) != 3*2+8/2
	error(0xfd10);
#endif
#if ((5*6)/3) == 3*2+8/2
#else
	error(0xfd11);
#endif
#define	z1(a,b)	(a+b)
#define	z2(a,b)	(a*b)
#if	z1(z2(2,3),z2(z1(1,1),3)) != 12
	error(0xfd12);
#endif
#if 1>2
	error(0xfd13);
#endif
#if 2<1
	error(0xfd14);
#endif
#if 1>=2
	error(0xfd15);
#endif
#if 2-1!=1
	error(0xfd16);
#endif
#if 1-2!= -1
	error(0xfd17);
#endif
#if -1 >= 0
	error(0xfd18);
#endif
#if 1 ? 0 : 1
	error(0xfd19);
#endif
#if 0 ? 1 : 0
	error(0xfd20);
#endif
#if (010<<1) != 020
	error(0xfd21);
#endif
#if (020>>1) != 010
	error(0xfd22);
#endif
#if (0xc^0xa) != 0x6
	error(0xfd23);
#endif
#if (0xc&0xa) != 0x8
	error(0xfd24);
#endif
#if (0xc|0xa) != 0xe
	error(0xfd25);
#endif
#if (~2) != -3
	error(0xfd26);
#endif
#if (!1) != 0
	error(0xfd26);
#endif
}

// check int extension to long
lextest()
{

	int iii;
	long lll;

	iii = -10;
	lll = iii*2;
	if(lll != -20L) error(0xfe07);
}

cmplxptrpp()
{
	int v;
	int dd[4];
	int *pd[4];
	register int i;

	for(v=0; v<4; v++) {
		pd[v] = &dd[v];
		dd[v] = v+100;
	}
	v = 1;
	i = *pd[v]++;
	if(i != 101) error(0xfe18);
	if(*pd[v] != 102) error(0xfe19);
}

strlen(p)
char *p;
{
	register int i;

	i = 0;
	while( *p++ )
		i++;
	return(i);
}

