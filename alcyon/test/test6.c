
/* Compiler Test Program Part III - version 4.2 plus */
char *version = "%W%	%G%";

int x1[5] = { 0, 1, 2, 3, 4, };
int x2[5] = { 0, 1, 2, 3, 4 };
int x3[5] = { 0, 1, 2, 3, };

unsigned u = -1;

char arc[5][3] = {
	0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
	0xffff, 0x1ffff, 0xffff, 0xffff, 0xffff	/* should have warnings on init */
};

int ari[5][3] = {
	0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
	0xffff, 0x1ffff, 0xffff, 0xffff, 0xffff	/* should have 1 warning on init */
};

unsigned int aru[5][3] = {
	0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
	0xffff, 0x1ffff, 0xffff, 0xffff, 0xffff	/* should have 1 warning on init */
};

long arl[5][3] = {
	0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
	0xffff, 0x1ffff, 0xffff, 0xffff, 0xffff
};

short ars[2][2] = {
	0|0100000, 1|0100000, 0xffff, 0x8000|0x040
};

struct Mstruct {
	int ar[3];
	char c;
} *mtab[] = {
	0, 1, 2, 'c'
};

typedef int tdefa[(5)];
typedef int tdefb [10+(2)];

tdefa a;
tdefb b;

int ff[(5)];

struct atype {
	int a;
	long b;
	int c;
} A[] = {
	{ 1, 1, 1 },
	{ 2, 2, 2 },
	{ 3, 3, 3 },
};

struct atype *mikeC[] = {	/* better not have any warnings on init... */
	0xf1e010,
	0xf1e011,
	0xf1e012,
	0xf1e013,
	0,
};

struct btype {
	char *p1;
	int i;
	char c;
} B = {
	"This is a string.....",
	35,
	'p'
};


int err;
char string[] = "9876543210";

int fun1(), fun2();
struct {
	int sz;
	int (*func)();
} fun[2] = { 1, fun1, 2, fun2 };
int fglobal;
int global1 = 4, global2 = 5;

main()
{
	v42();
	test();
	test2();
	aray();
	do_stass();
	do_sizeof();
	dbra_tst();
	new_test();

	if (err)
		printf("Tough Luck...\n");
	else
		printf("Far Freaking Out...\n");
}

error(ernum)
int ernum;
{
	printf("error %x\n",ernum);
	err++;
}

short mstype;
typedef struct MODES {
	short imap;
} MODER, *pMODER;
static MODER amode;
pMODER vmode;

v42()
{
	int i;
	struct test {
		int a;
		long b;
		struct {
			char c;
			int d;
		}y;
	} x;
	
	if (A[0].b != 1) error(0x1);
	if (A[1].c != 2) error(0x2);
	if (A[2].a != 3) error(0x3);
	x.y.d = 3;
	x.a = 0;
	{
		if (!x.y.d) error(0x4);
	}
	if (x.a) error(0x5);

	{
		char *s = &string;
		int i;

		i = *s - '0';
		if (i != 9) error(0x5);
	}
	{
		register int regi;
		int i;

		if ((i=f(3)) > f(1)) ; else error(0x6);
		if ((regi=f(3)) > f(1)) ; else error(0x7);
	}
	if (1) {
		short count = 0;

		mstype = 33;
	}
	if (mstype != 33) error(0x8);
	ff[i=0] = 33;
	if (ff[0] != 33) error(0x9);
	if(!main) error(0x10);
	vmode = &amode;
}

f(val)
int val;
{
	return(val);
}

struct fi_index {
	long	fi_flags;			 
	unsigned fi_count;			 
	char	fi_groupid;			 
	char	fi_nlinks;			 
	int		fi_ownerid;			 
	long	fi_fsize;			 
	long	fi_findblk;			 
	int		fi_dev;				 
	int		fi_lock;			 
	int		fi_spdev;			 
	char	*fi_rlocks;			 
};

char ca[10] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int ica[10] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
char cb[10] =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
char cc[10] =  { '01', '23', '45', '67', '89' };

struct A {
	int a;
	long b;
} atype[] = {
	0, 3,
	1, 4,
	2, 5
};

int three[3][3][3] = {
	0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2
};

/*char ar_test[0xf0000];*/
aray()
{
	register int  n;
	register char c, *bp;
	register struct fi_index *fip;
	struct fi_index xxx;

	fip = &xxx;
	xxx.fi_fsize = 3;
	n = 5000;
	if( n > (4096	  - fip->fi_fsize) ) {
		n = 4096	  - fip->fi_fsize;
		if (n != (4096 - fip->fi_fsize)) error(0x20);
	}
	else 
		error(0x21);

	if (ca[3] != 3) error(0x22);
	if (ica[7] != 7) error(0x23);
	if (cb[5] != '5') error(0x24);
	if (cc[8] != '8') error(0x25);
	if (atype[2].a != 2) error(0x26);
	if (atype[0].b != 3) error(0x27);
	if (three[0][0][2] != 0) error(0x28);
	if (three[1][1][1] != 1) error(0x29);
	if (three[2][2][0] != 2) error(0x2a);
	
	/*
	ar_test[0xa000L] = '7'; bp = &ar_test[0];
	c = bp[0xa000L];
	if (c != '7') error(0x2b);
	ar_test[0x10000] = 'T';
	c = bp[0x10000];
	if (c != 'T') error(0x2c);
	*/
}

struct {
	int _a, _a1, _a2, _a3, _a4;
	long _b, _b1, _b2, _b3, _b4;
	char _c, _c2, _c3, _c4;
} st, *pt, st2, *pt2, sa[3];

do_stass()
{
	pt = &st; pt2 = &st2;
	st._b4 = 3; st2._b4 = 9;
	st = st2;
	if (st._b4 != st2._b4) error(0x28);
	pt->_b1 = 55; pt2->_b1 = 34;
	*pt = *pt2;
	if (pt->_b1 != pt2->_b1) error(0x29);
	st = *pt;
	*pt = st;
	sa[0]._c4 = 'd'; sa[1]._c4 = '8';
	sa[1] = sa[0];
	if (sa[0]._c4 != sa[1]._c4) error(0x2a);
}
	

struct sz_struc {
	int za[10];
	char zc;
	char zb[3];
	struct xxx {
		char zd[5];
		long ze[2];
	}zf;
} *s, t;
int szaray[10];

typedef struct TDS {		  
	char		st;				  
	unsigned 	fConstant : 1;	  
	unsigned 	width : 7;		  
	unsigned 	bt  : 4;		  
	unsigned 	tq1 : 2; 
	unsigned 	tq2 : 2; 
	unsigned 	tq3 : 2; 
	unsigned 	tq4 : 2; 
	unsigned 	tq5 : 2; 
	unsigned 	tq6 : 2;		  
} TDR, *pTDR; 
 
typedef struct {	  
	char	sbVar[8]; 
	TDR		td;	  
	long	valTy; 
} TYR, *pTYR; 

TYR	vrgTySpc[] = {	  /* test of short element initialization */
	{"$cBad",   '\0', '\0', '\0', '\0', 0},
	0
}; 

struct one {
	struct two *p2;
	struct one *p1;
};

struct two {
	int atwo, btwo, ctwo, dtwo;
};

struct one xyzzy[] = {
	0, 0
};

char szof[10];

struct AAAAA {
	char a[3];
	int b[3];
	long c[3];
	double d[3];
	char e[3][3];
	int f[3][3];
	long g[3][3];
	double h[3][3];
	char i;
	int j;
	long k;
	float l;
} *abcdef;

do_sizeof()
{
	char nx[sizeof szof];
	char ny[sizeof (szof)];
	int i;
	long l;
	char *p;
	int *pi;

	if ( sizeof(vrgTySpc) != (2 * sizeof(TYR)) ) error(0x30);
	if ( sizeof(s->za) != 20 ) error(0x31);
	if ( sizeof(s->zb) != 3 ) error(0x32);
	if ( sizeof(s->zf) != 14 ) error(0x33);
	if ( sizeof(s->zf.ze) != 8 ) error(0x34);
	if ( sizeof(szaray) != 20 ) error(0x35);
	if ( sizeof(t.za) != 20 ) error(0x36);
	if ( sizeof(s->za[4]) != 2 ) error(0x37);
	if ( sizeof(t.zc) != 1 ) error(0x38);
	if ( sizeof(p) != 4 ) error(0x39);
	if ( sizeof(pi) != 4 ) error(0x3a);
	if ( sizeof(xyzzy) != 8 ) error(0x3b);
	if ( sizeof(l) != 4 ) error(0x3c);
	if ( sizeof(*p) != 1 ) error(0x3d);
	if ( sizeof(*pi) != 2 ) error(0x3e);
	if ( sizeof(szaray[4]) != 2 ) error(0x3f);
	if ( sizeof(struct one) != 8) error(0x40);
	if ( sizeof(nx) != sizeof ny) error(0x41);
	if ( sizeof(ny) != 10) error(0x42);
	if ( sizeof(abcdef->a) != 3) error(0x43);
	if ( sizeof(abcdef->b) != 6) error(0x44);
	if ( sizeof(abcdef->c) != 12) error(0x45);
	if ( sizeof(abcdef->d) != 12) error(0x46);
	if ( sizeof(abcdef->e) != 9) error(0x47);
	if ( sizeof(abcdef->f) != 18) error(0x48);
	if ( sizeof(abcdef->g) != 36) error(0x49);
	if ( sizeof(abcdef->h) != 36) error(0x4a);
	if ( sizeof(abcdef->i) != 1) error(0x4b);
	if ( sizeof(abcdef->j) != 2) error(0x4c);
	if ( sizeof(abcdef->k) != 4) error(0x4d);
	if ( sizeof(abcdef->l) != 4) error(0x4e);
}

 
typedef long		ADRT;
 
ADRT     vfp;
 
int PutState() 
{ 
    vfp = 1; 	/* just needs to compile.... */
}   

struct saxer {
	struct saxer *ptr;
	unsigned int u_incr;
	int i_incr;
} john[4];

typedef long X;
X typey;
#define STL 0x400L

struct xx {
	int a, b;
};

test()
{
	register struct saxer *p, *q;
	register short mask;
	int dp, z, *zp;
	long l, lo;
	register char rc;
	register int ri;
	char c;
	int i;
	struct xx xa[8/sizeof(struct xx)];
	struct xx xd[8/4];

	dp = 0x720;
	mask = ((dp&0xff)<<1) -1;
	if (mask != 0x3f) error(0x50);

	l = 0x7777ff30;
	casttest((int)(l&0xff));	/* error 0x51 */
	lo = 0;
	l = STL - lo;
	if (l != 0x400L) error(0x52);
	l = 0xffffffff;
	if (!(l & 0x80000000)) error(0x53);
	l &= 0x80000000;
	if (l != 0x80000000) error(0x54);

	mask = 1;
	l = 0xf;
	mask += (l&1);
	if (mask != 2) error(0x55);

	z = 1;
	(*fun[z].func)();
	if (fglobal != 3) error(0x56);

	typey = 0x71234;
	fun1(typey);	/* error 0x57 */
	z = 34;
	z &= 0;
	if (z) error(0x58);
	p = &john[0]; q = &john[0];
	p->u_incr = 2;
	p->i_incr = 2;
	p += p->u_incr;
	q += q->i_incr;
	if (p != q) error(0x59);
	if (sizeof(xa) != sizeof(xd)) error(0x5a);
	zp = &z;
	*zp = 34;
	*zp != 0;
	if (*zp != 34) error(0x5b);
	if (global1 != 4) error(0x5c);
	if (global2 != 5) error(0x5d);
	rc = 6; c = 8; ri = 2; i = 4;
	c /= ri;
	if (c != 4) error(0x5e);
	c /= i;
	if (c != 1) error(0x5f);
	rc /= 2;
	if (rc != 3) error(0x60);
}

casttest(i)
int i;
{
	if (i != 0x30) error(0x51);
}

fun1(l)
long l;
{
	fglobal = 2;
	if (l != 0x71234) error(0x57);
}

fun2()
{
	fglobal = 3;
}

char *shifts[4] = { "as", "ls", "rox", "ro" };

test2()
{
	int i;
	long l;
	int y();
	char *mnem;
	int opcode;

	l = 0xfe00;		/* hex constant should be int value, which should go neg */
	if (l > 0) error(0x61);
	l = 32768;
	if (l < 0) error(0x62);
	f2(fun2,fun2);		/* error 0x63 */

	mnem = shifts[(opcode>>9)&3];
	is_512L(0x64,512L);			/* error 0x64 */
	is_512L(0x65,(long)512);	/* error 0x65 */
	i = 512;
	l = 512;
	is_512L(0x66,l);			/* error 0x66 */
	is_512L(0x67,(long)i);		/* error 0x67 */
	l = 500; i = 12;
	is_512L(0x68,l+i);			/* error 0x68 */
	is_512L(0x69,512L*1);		/* error 0x69 */
	is_512L(0x6a,511L+1);		/* error 0x6a */
	is_512L(0x6b,1*512L);		/* error 0x6b */
	is_512L(0x6c,1+511L);		/* error 0x6c */
	is_512L(0x6d,(long)(512*1));/* error 0x6d */
	is_512L(0x6e,513L-1);		/* error 0x6e */
	is_512L(0x6f,512L/1);		/* error 0x6f */
}

is_512L(enum,lval)
int enum;
long lval;
{
	if (lval != 512L)
		error(enum);
}

static f2(a1,a2)
char *a1, *a2;
{
	if (a1 != a2) error(0x63);
}

int i_global;

dbra_tst()
{
	register long reg_l;
	register int localreg;
	int local;
	long local_l;

	local_l = i_global = local = localreg = reg_l = 1L;
	if(--i_global == -1) error(0x70);
	if(--local == -1) error(0x71);
	if(--localreg == -1) error(0x72);
	if(--local_l == -1) error(0x73);
	if(--reg_l == -1) error(0x74);

	if(--i_global == -1) ; else error(0x75);
	if(--local == -1) ; else error(0x76);
	if(--localreg == -1) ; else error(0x77);
	if(--local_l == -1L) ; else error(0x78);
	if(--reg_l == -1L) ; else error(0x79); 
}

typedef char TBOOL;

TBOOL tboolx[2], tbooly[(2)];

struct xtype {
	char *p1;
	char *ar[20];
	char *p2;
};

struct xtype newff = {
	"p1",
	"zero",
	"one",
};

char *xfp = "abc";

struct xtype ff0 = {
	0L,
	1L,
	2L,
	3L,
	4L,
	5L,
	6L,
	7L,
};

struct xtype ff1 = {
	"p1",
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"p2",
};

struct xtype ff2 = {
	"p1",
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"p2"
};

struct xtype ff3 = {
	"p1",
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
};

struct xtype ff4 = {
	"p1",
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five"
};

struct xtype ff5 = {
	"p1",
	"zero",
	"one",
	"two",
};

struct xtype ff6 = {
	"p1",
	"zero",
	"one",
	"two"
};

new_test()
{
	char *cp;
	extern int err;
	char str[9];
	int i;
	unsigned u;

/* typedef and paren'd strangeness */
	if(sizeof(tboolx) != sizeof(tbooly)) error(0x80);
	if(sizeof(tbooly) != 2) error(0x81);
	tboolx[1] = 1; tbooly[1] = 1;
	cp = tboolx;
	if(*(cp+1) != 1) error(0x82);
	cp = tbooly;
	if(*(cp+1) != 1) error(0x83);
/* assignment as a subscript */
	str[i=0] = 3;
	if(str[i] != 3) error(0x84);
	if(i != 0) error(0x85);
/* structure pointer array initializations */
	newff.ar[1] = "xyz";
	if(*xfp != 'a' || *(xfp+1) != 'b' || *(xfp+2) != 'c') error(0x86);
/* unsigned increment to pointer */
	cp = str; i = u = 3;
	*(cp+i) = 'c';
	if (*(cp+u) != 'c') error(0x87);
}
