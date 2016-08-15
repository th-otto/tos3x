#include <math.h>

typedef struct { double a[6]; } joint;

static joint lower_limit = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 };

float *f1();
float f2();
float ret_0();

struct type {
	float ft;
	double dt;
	float *pft;
} fstruc;

char *fstr = "-2.3E-4";
char *astr = "8.6324";
float ar[10];
float global_f;

int err = 0;

main()
{
	double d;
	float f, fp2, *fp;
	char *s, buf[20];
	long l;
	int i;

	if ((fp = f1()) != fstruc.pft) error(0xf1);
	if ((f = f2()) != 2.0) error(0xf2);
	d = f;
	if (d != f) error(0xf3);
	f = atof(fstr);
	s = ftoa(f,buf,6);
	printf("%s => %f => %s\n",fstr,f,s);
	f = 3.4;
	l = f;
	if (l != 3L) { error(0xf4); printf("%f => %ld != 3L\n",f,l); }
	i = f;
	if (i != 3)  { error(0xf5); printf("%f => %d != 3\n",f,i); }
	l = 7L;
	f = l;
	printf("6.9 < %f < 7.1\n",f);
	i = 7;
	f = i;
	printf("6.9 < %f < 7.1\n",f);
	f = atof(astr);
	printf("%s ==> ~ %f or %e\n",astr,f,f);
	f = 0;
	if (f != 0.0) error(0xf6);
	i = 3;
	ar[i] = 3.0;
	f = 4.0;
	fp2 = 10.0;
	printf("%f += ",ar[i]);
	printf("%f * %f ==> ",f,fp2);
	ar[i] += f * fp2;
	printf("%f\n",ar[i]);
	global_f = 0.0;
	f = 0.0;
	if_test(f);
	infinity();
	ar_test();
	if (!err)
		printf("Far Freaking Out....\n");
	else
		printf("Tough Luck...\n");
}

float *
f1()
{
	return(fstruc.pft);
}

float
f2()
{
	return(2.0);
}

float
ret_0()
{
	return(0.0);
}

if_test(pf)
float pf;
{
	float af, *ptr;

	ar[2] = 0.0;
	af = 0.0;
	ptr = &af;
	if (global_f) error(0x10);
	if (ar[2]) error(0x11);
	if (af) error(0x12);
	if (pf) error(0x13);
	if (*ptr) error(0x14);
	if (ret_0()) error(0x14);
}

infinity()
{
	float a, b;

	a = 1.0E+6;
	b = 0.0;

	while(a > b) {
		printf("%e > %e\n",a,b);
		b = a;
		a = a * a;
	}
	printf("%e <= %e\n",a,b);
}

ar_test()
{
	float a[5], b[5], c[5], d[5];
	int i;

	a[0] = 0.0; b[0] = 0.0; c[0] = 0.0; d[0] = 0.0;
	a[1] = 1.0; b[1] = 1.0; c[1] = 1.0; d[1] = 1.0;
	a[2] = 2.0; b[2] = 2.0; c[2] = 2.0; d[2] = 2.0;
	a[3] = 3.0; b[3] = 3.0; c[3] = 3.0; d[3] = 3.0;
	a[4] = 4.0; b[4] = 4.0; c[4] = 4.0; d[4] = 4.0;
	for(i = 0 ; i < 5; i++) {
		a[i] += b[i];
		c[i] = c[i] + d[i];
	}
	printf("0.0 += 0.0 => %f\n",a[0]);
	printf("1.0 += 1.0 => %f\n",a[1]);
	printf("2.0 += 2.0 => %f\n",a[2]);
	printf("3.0 += 3.0 => %f\n",a[3]);
	printf("4.0 += 4.0 => %f\n",a[4]);
	printf("%f = 0.0 + 0.0\n",c[0]);
	printf("%f = 1.0 + 1.0\n",c[1]);
	printf("%f = 2.0 + 2.0\n",c[2]);
	printf("%f = 3.0 + 3.0\n",c[3]);
	printf("%f = 4.0 + 4.0\n",c[4]);
}

error(enum)
int enum;
{
	printf("error: %x\n",enum);
	err++;
}

