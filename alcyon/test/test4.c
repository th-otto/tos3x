/* Post 4.2 compilers */
char *version = "%W%    %G%";

struct xxx {
    int x;
    char m,n,o,p;
};

struct zzz {
    int b;
    struct aaa {
        int f;
        char g, h, i, j;
    } bb[3];
    int c;
} A = { 
    1, 
    2, 'a', 'b', 'c', 'd', 
    3, 'e', 'f', 'g', 'h'
};

struct yyy {
    int a;
    struct xxx y[3];
    int q;
} B = {
    1, 
    2, 'a', 'b', 'c', 'd', 
    3, 'e', 'f', 'g', 'h'
};

char ar[3][3] = { 'a', 'b', 'c', 'd', 'e' };
int x[2][2][2] = { 1, 2, 3, 4, 5, 6, 7, 8, };
int errors;

main()
{
    xtest();
    ytest();
    ztest();
    dec_inc();
    div_test();
    s_u_test();
    next_to_last_test();
    last_test();
    if(!errors)
        printf("Far Freaking Out\n");
    else
        printf("Tough Luck\n");
}

error(enum)
int enum;
{
    printf("error %x\n",enum);
    errors++;
}

char *xstr1 = "8999999988";
char *xstr2 = "8999999988";
char *xstr3 = "8999999988";

xtest()
{
    register char *rcp, rc;
    char *cp, c;
    int i;
    long *laddr;
    int *iaddr;
    short *saddr;
    long addr;

    if(A.bb[1].i != 'g') error(0x1);
    if(A.bb[1].i != B.y[1].o) error(0x2);
    if(A.c != 0) error(0x3);
    if(B.y[3].f != 0) error(0x4);
    i = 257;
    i = (char)(i + 3);
    if (i != 4) error(0x5);

    saddr = laddr = iaddr = addr = 0xff000;
    if(laddr != addr) error(0x6);
    if(iaddr != addr) error(0x7);
    if(saddr != addr) error(0x8);

    rcp = &xstr1[6];
    while((*rcp)++ == '9')
        *rcp-- = '0';
    rcp = &xstr2[7];
    while((*--rcp)++ == '9')
        *rcp = '0';
    cp = &xstr3[7];
    while((*--cp)++ == '9')
        *cp = '0';
    if(strcmp(xstr1,"9000000988") != 0) error(0x9);
    if(strcmp(xstr2,"9000000988") != 0) error(0xa);
    if(strcmp(xstr3,"9000000988") != 0) error(0xb);

    rc = '9';
    rc = *rcp++ = '3';
    if(rc != '3') error(0xc);
    if(*(rcp-1) != '3') error(0xd);

    c = '8';
    c = (char)0;
    if(c != '\0') error(0xe);
    if(multest() != 4) error(0xf);
}

struct bfs {
    int bf1: 1;
} bfx, *bfa[3] = { (char *)-1, &bfx, (char *)-1} ;

char xar[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

dec_inc()
{
    register struct bfs **bfp;
    register char *p, *q, c;

    bfx.bf1 = 0;
    bfp = &bfa[2];
    (*--bfp)->bf1 += 1;
    if( bfx.bf1 != 1 ) error(0x10);
    p = &xar[10];
    (*--p)++;
    if( xar[9] != 10 || p != &xar[9] ) error(0x11);
    c = (*--p)++;
    if( c != 8 ||  xar[8] != 9 || p != &xar[8] ) error(0x12);
    c = 7;
    if( (*--p)++ != c || xar[7] != 8 || p != &xar[7] ) error(0x13);
    p = &xar[0];
    (*p++)++;
    if( xar[0] != 1 || p != &xar[1] ) error(0x14);
    c = (*p++)++;
    if( c != 1 || xar[1] != 2 || p != &xar[2] ) error(0x15);
    c = 2;
    if( (*p++)++ != c || xar[2] != 3 || p != &xar[3] ) error(0x16);
    --(*--p);
    if( xar[2] != 2 || p != &xar[2] ) error(0x17);
    c = --(*--p);
    if( c != 1 || xar[1] != 1 || p != &xar[1] ) error(0x18);
    c = 0;
    if( --(*--p) != c || xar[0] != 0 || p != &xar[0] ) error(0x19);
    --(*p++);
    if( xar[0] != -1 || p != &xar[1] ) error(0x1a);
    c = --(*p++);
    if( c != 0 || xar[1] != 0 || p != &xar[2] ) error(0x1b);
    c = 1;
    if( --(*p++) != c || xar[2] != 1 || p != &xar[3] ) error(0x1c);
}

multest()
{
    char *s;
    register int i;
    int j;

    s = &j;
    *s = 2;
    s[1] = 0;
    i = 2;
    return(i * *s);
}

div_test()
{
    int i, i2;
    long l, l2;
    register int ri, ri2;
    register long rl, rl2;

    rl2 = l2 = 70000;
    ri = rl2 / 10;
    rl = rl2 / 10;
    i  = l2  / 10;
    l  = l2  / 10;
    if(ri != i) error(0x20);
    if(rl != l) error(0x21);
    if(i  != 7000) error(0x22);
    if(i  != l) error(0x23);

    i2 = ri2 = 1;
    ri = rl2 / ri2;
    rl = rl2 / ri2;
    i  = l2  / i2;
    l  = l2  / i2;
    if(ri != i) error(0x24);
    if(rl != l) error(0x25);
    if(rl != rl2) error(0x26);
    if(ri == rl) error(0x27);
}

struct fcbflags {
    unsigned online : 1;
    unsigned buffer_dirty : 1;
    unsigned open_mode : 2;
    unsigned device_type : 3;
    unsigned file_type : 2;
    unsigned unbuffered : 1;
};

struct fcb {
    int fcbfd;
    char *fcpfp;
    struct fcbflags fcbflg;
    char name;
};

#define ONE     1
#define TWO     2

#define IADDRESS ((int *)0xfff900)
#define CADDRESS ((char *)0xfff900)
#define LADDRESS ((long *)0xfff900)
#define SADDRESS ((struct device *)0xfff900)

struct device {
    int mts;
    char mtc;
    long mtl;
    int mti;
};

struct exec {
    int xx[19];
    int y;
};

typedef char *charp; 
struct t {
    char *p1;
    char *p2; 
} ht1 = { 0, ((charp) 30) },
        ht2 = { 0, 0 };

ytest()
{
    register struct fcb *p;
    register long rl;
    register int *rpi;
    register long *rpl;
    register int ri1, ri2;
    struct fcb fs;
    long a;
    int i, j, ari[5];
    long l, arl[5];
    char buf[40];
    struct exec hdr, hdr2;

/* bit field arguments */
    i = 1; p = &fs;
    p->fcbflg.unbuffered = (i == 0) ? 0 : 1;
    if (!p->fcbflg.unbuffered) error(0x30);
    i = 0;
    p->fcbflg.unbuffered = (i == 0) ? 0 : 1;
    if (p->fcbflg.unbuffered) error(0x31);
/* constant coersion */
    l = 32768;  
    if (l != 32768L) error(0x32);
    l = 32 * 1024;
    if (l != 32768L) error(0x33);
/* long to integer conversions including as parameters and return values */
    l = 0xf00030;
    ctest(0x34,0x35,(int)l,27);             /* errors 34 & 35 */
    ctest2(0x36,(int)l);                    /* error 36 */
    l = 0xfff30;
    ctest(0x37,0x38,((int)l)&0xff,27);      /* errors 37 & 38 */
    ctest2(0x39,((int)l)&0xff);             /* error 39 */
    ctest(0x3a,0x3b,(int)(l&0xff),27);      /* errors 3a & 3b */
    ctest2(0x3c,(int)(l&0xff));             /* errors 3c */
    if(func1(1) != 0x30) error(0x3d);
    if(func1(2) != 0x30) error(0x3e);
    if(func1(3) != 0x30) error(0x3f);
/* equal-shift tests */
    rpi = &ari[3]; rpl = &arl[3];
    *rpi = 0x3f02;
    *rpi++ >>= 1;
    if(ari[3] != 0x1f81) error(0x40);
    *--rpi <<= 2;
    if(ari[3] != 0x7e04) error(0x41);
    *rpl = 0x6af010;
    *rpl++ >>= 4;
    if(arl[3] != 0x6af01) error(0x42);
    *--rpl <<=5;
    if(arl[3] != 0xd5e020) error(0x43);
/* sethy-ulman easy expression */
    i = 10; j = 3;
    a = (long)i * (long)i - (long)j * (long)j;
    if(a != 91) error(0x44);
/* tricky coercion of constant to integer pointer with array subscript */
/* indexing off of int/long/char/struct pointers */
    l = &IADDRESS[TWO];
    if(l != 0xfff904) error(0x45);
    l = &LADDRESS[TWO];
    if(l != 0xfff908) error(0x46);
    l = &CADDRESS[TWO];
    if(l != 0xfff902) error(0x47);
    l = &(SADDRESS->mtc);
    if(l != 0xfff902) error(0x48);
    l = &(SADDRESS->mtl);
    if(l != 0xfff904) error(0x49);
    l = &(SADDRESS->mti);
    if(l != 0xfff908) error(0x4a);
/* structure assigned to non structure item... */
    hdr.y = 3; hdr2.y = 5;
    *(struct exec *)buf = hdr;
    hdr2 = *(struct exec *)buf;
    if(hdr2.y != 3) error(0x4b);
/* typedef cast inside structure initialization */
    if(ht1.p2 != 30) error(0x4c);
/* indexing test with register index variables */
    i = j = ri1 = ri2 = 1;
    if (x[ri1][ri2][ri1] != x[1][1][1]) error(0x4d);
    if (x[i][j][i] != x[1][1][1]) error(0x4e);
}

struct zxx {
    int a,b,c,d,e;
};

ztest()
{
    register long longtmp;
    int i, j, k;
    char *cp1, *cp2;
    int *ip1, *ip2;
    long *lp1, *lp2, l;
    struct zxx *sp1, *sp2;

/* int's cast to pointers... */
    i = 0x234;
    cp1 = i; cp2 = (char *)i;
    ip1 = i; ip2 = (int *)i;
    lp1 = i; lp2 = (long *)i;
    sp1 = i; sp2 = (struct zxx *)i;
    if(cp1 != cp2) error(0x70);
    if(ip1 != ip2) error(0x71);
    if(lp1 != lp2) error(0x72);
    if(sp1 != sp2) error(0x73);
/* long easy code skeleton tests */
    i = 0x24;  j = 0x11;
/* addition.... */
    k = i * i + j * j;
    longtmp = (long)i * (long)i + (long)j * (long)j;
    l = (long)i * (long)i + (long)j * (long)j;
    if(longtmp != l) error(0x74);
    if(k != l) error(0x75);
/* subtraction.... */
    k = i * i - j * j;
    longtmp = (long)i * (long)i - (long)j * (long)j;
    l = (long)i * (long)i - (long)j * (long)j;
    if(longtmp != l) error(0x76);
    if(k != l) error(0x77);
/* exclusive or.... */
    k = i * i ^ j * j;
    longtmp = (long)i * (long)i ^ (long)j * (long)j;
    l = (long)i * (long)i ^ (long)j * (long)j;
    if(longtmp != l) error(0x78);
    if(k != l) error(0x79);
/* inclusive or.... */
    k = i * i | j * j;
    longtmp = (long)i * (long)i | (long)j * (long)j;
    l = (long)i * (long)i | (long)j * (long)j;
    if(longtmp != l) error(0x7a);
    if(k != l) error(0x7b);
}

struct _b {
    long y;
    int x;
} bs;

union rec {
    struct _b l;
    struct {
        char c;
        int w;
    } f;
} recs;

struct ty {
    int c;
} tys;

struct _a {
    int x;
} as;

union one {
    int a;
    long b;
    struct {
        int y;
        int x;
    } q;
} ones, f1, f2;

struct two {
    int a;
    long b;
    struct {
        int y;
        int x;
    } q;
} twos;

struct _x {
    long a;
    int b,c,d,e,f;
};

struct _y {
    int t;
    struct _x s;
} _yz = {
    33,
    { 1, 2, 3, 4, 5 }
};

struct _y _yq = { 33, 1, 2, 3, 4, 5 };

struct _s1 {
    int a;
    int b;
};

struct _s2 {
    struct _s1 c;
    int d;
};

struct _s2 test0 = { { 10, 20 }, 30 };
struct _s2 test1 = { { 10, 20 } };
struct _s2 test2 = { { 10 }, 30 };
struct _s2 test3 = { 10, 20, 30 };

s_u_test()
{
    bs.y = 1;
    bs.x = 2;
    recs.l.y = 3;
    recs.l.x = 4;
    tys.c = 'a';
    as.x = 5;
    if(bs.y != 1) error(0x50);
    if(bs.x != 2) error(0x51);
    if(recs.l.y != 3) error(0x52);
    if(recs.l.x != 4) error(0x53);
    if(tys.c != 'a') error(0x54);
    if(as.x != 5) error(0x55);
    recs.f.c = 'b';
    recs.f.w = 6;
    if(recs.f.c != 'b') error(0x56);
    if(recs.f.w != 6) error(0x57);
    if(tys.c != 'a') error(0x58);
    twos.a = 7;
    twos.b = 8;
    twos.q.y = 9;
    twos.q.x = 10;
    ones.a = 11; f1.b = 0; f2.b = 0;
    if(twos.a != 7) error(0x59);
    if(twos.b != 8) error(0x5a);
    if(twos.q.y != 9) error(0x5b);
    if(twos.q.x != 10) error(0x5c);
    if(ones.a != 11) error(0x5d);
    ones.b = 12;
    if(ones.b != 12) error(0x5e);
    if(f1.b != 0) error(0x5f);
    if(f2.b != 0) error(0x60);
    f1.b = f2.b = 0;
    ones.q.y = 13;
    if(ones.q.y != 13) error(0x61);
    if(f1.b != 0) error(0x62);
    if(f2.b != 0) error(0x63);
    f1.b = f2.b = 0;
    ones.q.x = 14;
    if(ones.q.x != 14) error(0x64);
    if(f1.b != 0) error(0x65);
    if(f2.b != 0) error(0x66);

    /* initialization tests */
    if(_yz.s.c != 3) error(0x67);
    if(_yq.s.c != 3) error(0x68);
    if(test0.d != 30) error(0x69);
    if(test0.c.b != 20) error(0x6a);
    if(test1.d != 0) error(0x6b);
    if(test2.c.b != 0) error(0x6c);
    if(test3.c.b != 20) error(0x6d);
    if(test3.d != 30) error(0x6e);
    if(test2.d != 30) error(0x6f);
}

ctest(err1,err2,value1,value2)  /* value1 had better be an integer !!! */
int err1, err2, value1, value2;
{
    if(value1 != 0x30) error(err1);
    if(value2 != 27) error(err2);
}

ctest2(err,value)   /* value had better be an integer !!! */
int err, value;
{
    if(value != 0x30) error(err);
}

func1(ret)      /* had better return an integer....... */
int ret;
{
    long l;

    l = 0xf0030;
    if(ret==1) return((int)l);
    l = 0xfff30;
    if(ret==2) return(((int)l)&0xff);
    return((int)(l&0xff));  /* return 3 */
}

struct {
	char x_p[3][3];
} *p_ca2, ca2;

int artl();

struct {
	int ss_x_z;
	int (*ss_ff)();
} sff[] = {3, 0L, 4, &artl};

next_to_last_test()
{
	char ch;
    int x, y;
	char ar[9];
	register char c;

/* defined character constants handled properly ?? */
    x = '\0';
    if(x != 0) error(0x80);
    x = '\f';
    if(x != 014) error(0x81);
    x = '\t';
    if(x != 011) error(0x82);
    x = '\n';
    if(x != 012) error(0x83);
    x = '\r';
    if(x != 015) error(0x84);
    x = '\b';
    if(x != 010) error(0x85);
    x = '\\';
    if(x != 0134) error(0x86);
/* character indices... */
	ar[1] = '5'; ar[2] = '6'; x = 1;
	ar[ c = (x==1) ? 2 : 1 ] = '3';
	if (c != 2) error(0x87);
	if(ar[2] != '3') error(0x88);
	ar[c & ~128] = '5';
	if(ar[2] != '5') error(0x89);
/* array indices incrementing with pointers and other strange things */
	p_ca2 = &ca2;
	ca2.x_p[1][1] = 'c';
	x = 1; y = 1;
	p_ca2->x_p[x][y++] = 'd';
	if(ca2.x_p[1][1] != 'd') error(0x8a);
	if(y != 2) error(0x8b);
/* struct with function arg return */
	x = 1;
	x = (*sff[x].ss_ff)();
	if (x != 4) error(0x8c);
/* character's compared to integer and long constants */
	ch = 127;
	if(ch > 0xffffff) error(0x8d);
	if(ch > 0xfff) error(0x8e);
	if(ch != 127) error(0x8f);
}

struct {
	char *a;
	char *b;
	char *c[3];
} flxtv = {
	"one",
	"two",
	"three",
	"four",
	"five"
};
char ln[3] = "ab";
struct atbh1 { unsigned sevensta:16, sevenstb:4; };
struct atbh2 { unsigned a:4, b:4, c:4; };

last_test()
{
	struct atbh1 x;
	struct atbh2 y;
	int i;
	char c, *cp;

/* bit field manipulation */
	x.sevensta = 2;
	x.sevenstb = 1;
	if(x.sevensta != 2) error(0x90);
	if(x.sevenstb != 1) error(0x91);
	y.a = 3; y.b = 2; y.c = 1;
	i = (int)y.a;
	if (i != 3) error(0x92);
	i = (int)y.b;
	if (i != 2) error(0x93);
	i = (int)y.c;
	if (i != 1) error(0x94);
/* character manipulation */
	i = ln[0] * ln[1];
	if(i != 9506) error(0x95);
	cp = ln;
	c = 0xf4;
	*cp++ |= (c & 0xf);
	if(ln[0] != 0x65) error(0x96);	/* 'a' | 0x4 */
	cp = flxtv.c[1];
	if(*cp != 'f' || *(cp+1) != 'o' || *(cp+2) != 'u' || *(cp+3) != 'r')
		error(0x97);
}

artl(last_test)
int last_test;	/* argument with same name as function !!! */
{
	last_test = 4;
	return(last_test);
}

