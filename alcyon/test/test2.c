/* Compiler Test Program Part II - version 4.0 plus */

#define DOHIST '\!'
#define QUOTED 128
  
struct lbits {
	unsigned b1 : 4;
	unsigned b2 : 12;
} low;

struct hbits {
	unsigned b1 : 12;
	unsigned b2 : 4;
} ;

struct hbits high;		/* be sure global decl ok... */

int err;
int globali = -32768;
unsigned globalu = 44000;

char ca[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

long lconst1 = 5 * 9;
long lconst2 = 5L * 9;
long lconst3 = 5 * 9L;
long lconst4 = 5L * 9L;
long lconst5 = (long)(5 * 9);

char c0[] = {
	0,  1,  2,  3,  4, 
	5,  6,  7,  8,  9, 
	10, 11, 12, 13, 14, 15,
};

int i0[] = {
	0,  1,  2,  3,  4, 
	5,  6,  7,  8,  9, 
	10, 11, 12, 13, 14, 15,
};

long l0[] = {
	0,  1,  2,  3,  4, 
	5,  6,  7,  8,  9, 
	10, 11, 12, 13, 14, 15,
};

struct Proto {
	char P_id;
	int (*P_turnon)();
	int (*P_rdmsg)();
	int (*P_wrmsg)();
	int (*P_rddata)();
	int (*P_wrdata)();
	int (*P_turnoff)();
};

extern int gturnon(), gturnoff();
extern int grdmsg(), grddata();
extern int gwrmsg(), gwrdata();

struct Proto Ptbl[] = {
	'g', gturnon, grdmsg, gwrmsg, grddata, gwrdata, gturnoff,
	'\0'
};

struct nroff {
	int a;
	char *p;
	char *ctag[5];
	char *zz;
};

struct nroff star = {
	1,
	"pstring...",
	"zero",
	"one",
	"two",
	"three",
	"four",
	"zzstring"
};

struct {
	int a[2];
	char b[2];
	long c[2];
	char *d[2];
} sa = {
	1, 2,
	'a', 'b',
	3, 4,
	"abc", "def"
};

int gturnon() {}
int gturnoff() {}
int grdmsg() {}
int grddata() {}
int gwrmsg() {}
int gwrdata() {}

main()
{
	l_vs_i();
	rl_vs_i();
	l_vs_ri();
	rl_vs_ri();
	v40();
	v41();
	v42();
	v42_a();
	if (!err)
		printf("Far Freaking Out\n");
	else
		printf("Tough Luck.....\n");
}

l_vs_i()		/* longs versus ints... v. 4.0 addition */
{
	long autol, autol1;
	int i;

	i = 3;
	autol = autol1 = 100;
	autol1 *= i;
	autol =autol * i;
	if (autol1 != autol) error(0x00);

	autol = autol1 = 100;
	autol1 /= i;
	autol =autol / i;
	if (autol1 != autol) error(0x01);

	autol = autol1 = 100;
	autol1 += i;
	autol =autol + i;
	if (autol1 != autol) error(0x02);

	autol = autol1 = 100;
	autol1 -= i;
	autol =autol - i;
	if (autol1 != autol) error(0x03);

	autol = autol1 = 100;
	autol1 %= i;
	autol =autol % i;
	if (autol1 != autol) error(0x04);

	autol = autol1 = 100;
	autol1 <<= i;
	autol =autol << i;
	if (autol1 != autol) error(0x05);

	autol = autol1 = 100;
	autol1 >>= i;
	autol =autol >> i;
	if (autol1 != autol) error(0x06);

	autol = autol1 = 100;
	autol1 &= i;
	autol =autol & i;
	if (autol1 != autol) error(0x07);

	autol = autol1 = 100;
	autol1 |= i;
	autol =autol | i;
	if (autol1 != autol) error(0x08);

	autol = autol1 = 100;
	autol1 ^= i;
	autol =autol ^ i;
	if (autol1 != autol) error(0x09);
}

rl_vs_i()		/* register longs versus ints... v. 4.0 addition */
{
	register long regl, regl1;
	int i;

	i = 3;
	regl = regl1 = 100;
	regl1 *= i;
	regl = regl * i;
	if (regl1 != regl) error(0x10);

	regl = regl1 = 100;
	regl1 /= i;
	regl = regl / i;
	if (regl1 != regl) error(0x11);

	regl = regl1 = 100;
	regl1 += i;
	regl = regl + i;
	if (regl1 != regl) error(0x12);

	regl = regl1 = 100;
	regl1 -= i;
	regl = regl - i;
	if (regl1 != regl) error(0x13);

	regl = regl1 = 100;
	regl1 %= i;
	regl = regl % i;
	if (regl1 != regl) error(0x14);

	regl = regl1 = 100;
	regl1 <<= i;
	regl = regl << i;
	if (regl1 != regl) error(0x15);

	regl = regl1 = 100;
	regl1 >>= i;
	regl = regl >> i;
	if (regl1 != regl) error(0x16);

	regl = regl1 = 100;
	regl1 &= i;
	regl = regl & i;
	if (regl1 != regl) error(0x17);

	regl = regl1 = 100;
	regl1 |= i;
	regl = regl | i;
	if (regl1 != regl) error(0x18);

	regl = regl1 = 100;
	regl1 ^= i;
	regl = regl ^ i;
	if (regl1 != regl) error(0x19);
}

l_vs_ri()		/* longs versus register ints... v. 4.0 addition */
{
	long autol, autol1;
	register int i;

	i = 2;
	autol = autol1 = 100;
	autol1 *= i;
	autol =autol * i;
	if (autol1 != autol) error(0x20);

	autol = autol1 = 100;
	autol1 /= i;
	autol =autol / i;
	if (autol1 != autol) error(0x21);

	autol = autol1 = 100;
	autol1 += i;
	autol =autol + i;
	if (autol1 != autol) error(0x22);

	autol = autol1 = 100;
	autol1 -= i;
	autol =autol - i;
	if (autol1 != autol) error(0x23);

	autol = autol1 = 100;
	autol1 %= i;
	autol =autol % i;
	if (autol1 != autol) error(0x24);

	autol = autol1 = 100;
	autol1 <<= i;
	autol =autol << i;
	if (autol1 != autol) error(0x25);

	autol = autol1 = 100;
	autol1 >>= i;
	autol =autol >> i;
	if (autol1 != autol) error(0x26);

	autol = autol1 = 100;
	autol1 &= i;
	autol =autol & i;
	if (autol1 != autol) error(0x27);

	autol = autol1 = 100;
	autol1 |= i;
	autol =autol | i;
	if (autol1 != autol) error(0x28);

	autol = autol1 = 100;
	autol1 ^= i;
	autol =autol ^ i;
	if (autol1 != autol) error(0x29);
}

rl_vs_ri()		/* reg longs versus reg ints... v. 4.0 addition */
{
	register long regl, regl1;
	register int i;

	i = 3;
	regl = regl1 = 100;
	regl1 *= i;
	regl = regl * i;
	if (regl1 != regl) error(0x30);

	regl = regl1 = 100;
	regl1 /= i;
	regl = regl / i;
	if (regl1 != regl) error(0x31);

	regl = regl1 = 100;
	regl1 += i;
	regl = regl + i;
	if (regl1 != regl) error(0x32);

	regl = regl1 = 100;
	regl1 -= i;
	regl = regl - i;
	if (regl1 != regl) error(0x33);

	regl = regl1 = 100;
	regl1 %= i;
	regl = regl % i;
	if (regl1 != regl) error(0x34);

	regl = regl1 = 100;
	regl1 <<= i;
	regl = regl << i;
	if (regl1 != regl) error(0x35);

	regl = regl1 = 100;
	regl1 >>= i;
	regl = regl >> i;
	if (regl1 != regl) error(0x36);

	regl = regl1 = 100;
	regl1 &= i;
	regl = regl & i;
	if (regl1 != regl) error(0x37);

	regl = regl1 = 100;
	regl1 |= i;
	regl = regl | i;
	if (regl1 != regl) error(0x38);

	regl = regl1 = 100;
	regl1 ^= i;
	regl = regl ^ i;
	if (regl1 != regl) error(0x39);
}

v40()	/* 4.0 fixed and tested */
{
	long l, l1;
	register int i;

	/* bit assignments */
	low.b1 = high.b2;
	high.b1 = low.b2;

	l = l1 = 45L;
	/* non-code-generating simple expressions */
	l *= 1L;
	if (l != l1) error(0x40);
	l += 0L;
	if (l != l1) error(0x41);
	l -= 0L;
	if (l != l1) error(0x42);
	l *= 0L;
	if (l != 0L) error(0x43);
	l1 %= 1L;
	if (l1 != 0L) error(0x44);

	/*SMC Bug - not properly extending i register on stack*/
	i = 2;
	ca[i] = f();
	if( ca[i] != f() ) error(0x45);
}

f()
{
	return(0);
}

struct xyzzytype {
	long x, y, z;
	char *xyz, xyzzy;
} saray1[3], saray2[4];

#define max(x,y)	((x > y) ? 'x' : 'y')
#define strings(s1,s2)	"s1s2"
#define substitute(x)	"x,"
v40a()		/* bug fixes and enhancements for v 4.0 ammended */
{
	char ch, *s, *t;

	ch = max(3,9);
	if(ch != '9') error(0x50);
	s = strings(s,t);
	if (strcmp(s,"s1s2") != 0) error(0x51);
	s = substitute(5);
	if (strcmp(s,"5,") != 0) error(0x52);
	saray2[0].y = 4L; saray2[0].xyzzy = 'p'; saray2[0].x = 450L;
	saray1[2] = saray2[0];
}

int v41_testvalue;
v41()
{
	register char *p;
	register int i;
	register int j;
	int form;
	long m, n;
	long al[10];
	int aray[3][4][5];

	aray[2][3][4] = 4;					/* multi dimensional arrays */
	if (aray[2][3][4] != 4) error(0x53);
	if (lconst1 != 45) error(0x54);		/* long constant initialization */
	if (lconst2 != 45) error(0x55);
	if (lconst3 != 45) error(0x56);
	if (lconst4 != 45) error(0x57);
	if (lconst5 != 45) error(0x58);

	m = 12; n = -1;				/* arrays involving long mult for indices */
	al[2] = 33L;
	n = al[m + n * 10];
	if (n != 33L) error(0x59);
	al[2L] = 31L;				/* arrays involving long indices */
	n = al[2];
	if (n != 31L) error(0x5a);

	1 ? 0 : 2;
	1 ? (form = 0) : (form = 1);
	if (form != 0) error(0x5b);
	form = 3;
	0 ? (form = form | ( 1 << 0)) : (form = form & ~(1<<0));
	if (form != 2) error(0x5c);
	form = 3;
	0 ? (form |= (1 << 0)) : (form &= ~(1 << 0));
	if (form != 2) error(0x5d);

	for( i = 0; i < sizeof(ca); i++ )
		ca[i] = i;
	i = 0;
	p = ca;
	if( ca[*p++]++ != 0 ) error(0x61);
	if( p != &ca[1] ) error(0x62);
	if( ca[0] != 1 ) error(0x63);
	if( ca[i+=1]++ != 1 ) error(0x64);
	if( i != 1 ) error(0x65);
	if( ca[1] != 2 ) error(0x66);
	if( ca[i+= *p++]++ != 3 ) error(0x67);
	if( i != 3 ) error(0x68);
	if( p != &ca[2] ) error(0x69);
	if( ca[3] != 4 ) error(0x6a);
	j = 1;
	i = j++ && j++;
	if( j != 3 ) error(0x6b);
	if( i != 1 ) error(0x6c);
	j = 0;
	i = j++ && j++;
	if( j != 1 ) error(0x6d);
	if( i != 0 ) error(0x6e);
	j = 1;
	i = --j || --j;
	if( j != -1 ) error(0x6f);
	if( i != 1 ) error(0x70);
	j = 1;
	i = j ? --j : --j;
	if( j != 0 ) error(0x71);
	if( i != 0 ) error(0x72);

	v41_testvalue = 128;
	if (v41_testvalue != ((long) 128)) error(0x73);
	if (v41_testvalue != ((int)(long) 128)) error(0x74);
	if (v41_testvalue != (int)((long) 128)) error(0x75);
	v41_testvalue = 128 * 3;
	if (v41_testvalue != (int)((long) 128*3)) error(0x76);

	if ((long)p & 0x80000000);
}

struct qtest {
	int sar[10];
};

struct zip {
	int i; char c[6]; int j;
} zippo[] = {
	0, "zero",      00,
	1, "one",       11,
	2, "two",       22,
	3, "three",     33,
	4, 'fo', "ur",  44,
	5, "five",      55,
	6, 'si', 'x', 0, 0, 0, 66,
	7, 'se', 've', 'n\0', 77,
	0
};

struct Abit {
	char x0;
	char x1:1;
	int  x2:1;
	int  x3:1;
} bit1;

struct Bbit {
	char bx1;
	int  bx2:1;
	int  bx3:1;
} bit2;

v42()
{	
	int case_val, index, tmp;
	int aar[10];
	struct qtest a;

	v41_testvalue = 128 * 3;
	if (v41_testvalue != ((int)(long) 128*3)) error(0x80);
	case_val = 0200| 'P';
	switch(case_val) {
		case (0200|'P'): break;
		default: error(0x81); break;
	}
	if (sizeof(a.sar) != sizeof(aar)) error(0x82);
	index = 2;
	aar[index+1] = 'c';
	if (aar[index+'\01'] != 'c') error(0x83);
	if (c0[3] != 3) error(0x84);
	if (i0[5] != 5) error(0x85);
	if (l0[12] != 12) error(0x86);

	if (eqstr(zippo[1].c,"one")==0) error(0x87);
	if (eqstr(zippo[4].c,"four")==0) error(0x88);
	if (eqstr(zippo[6].c,"six")==0) error(0x89);
	if (eqstr(zippo[7].c,"seven")==0) error(0x8a);

	tmp = 'd';
	index = tmp - 'a';
	if (ca[index] != ca[tmp - 'a']) error(0x8b);

	if (sizeof(struct Proto) != 26) error(0x8c);
	if ((sizeof(struct Proto)*2) != sizeof(Ptbl)) error(0x8d);
	tmp = 0x7000;
	switch(tmp & 0xf000) {
		default: 
			case_val = tmp&0xf000; error(0x8e); printf("val == %x\n",case_val);
		case 0x7000: 
			break;
	}
}

struct termio {
	int c_iflag;
	int c_oflag;
	int c_cflag;
	int c_lflag;
	int c_line;
	char c_cc[8];
};

struct termio ttys;
int ttysave;

v42_a()
{
	long l;

	bit1.x2 = 1;
	bit1.x3 = 0;
	bit2.bx2 = 1;
	bit2.bx3 = 0;
	if( bit1.x2 != bit2.bx2) error(0x90);
	if( bit1.x3 != bit2.bx3) error(0x91);

	if (ch_test() != (DOHIST|QUOTED)) error(0x92);

	l = 0x8000L;
	if (!(l & 0x8000)) error(0x93);

	if (iox(0, (('T'<<8)|1), &ttys) >= 0)
		ttysave = ttys.c_oflag;
	
	if (eqstr(star.ctag[3],"three")==0) error(0x94);
	if (sa.a[1] != 2) error(0x95);
	if (sa.b[1] != 'b') error(0x96);
	if (sa.c[1] != 4) error(0x97);
	if (eqstr(sa.d[1],"def")==0) error(0x98);
}

iox(i,j,k)
int i, j;
long k;
{
	return (1);
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

ch_test()
{
	return(DOHIST|QUOTED);
}

error(ernum)
int ernum;
{
	printf("error %x\n",ernum);
	err++;
}


