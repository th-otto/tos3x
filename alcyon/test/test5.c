/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

char *version = "@(#)test9.c	1.3    12/12/83";

/*
	the following code is a battery of tests directed
	'against' the C compiler.  these tests reflect difficulties
	dicovered in prior compiler releases.

	cr
*/

char array[33000];		/* test for array size > 32k bytes */

struct {
	int a;
	char b[7];
	long d;
} s[5] = {				/* test structure array initialization */
	1,      "one", 10,
	-32768, "two", 010,
	0,      "three", 0x10,
	32767,  "four h", 1000000000,
	-1,     "five", -99999999
};

main() {
	int i = 100;
	register int ri = 1000;
	long l;
	register long rl;
	char *p1, *p2;
	int *pi;

	if ((array[32999] = 'z') == 'z')
		printf("array[32999] is accessible\n");
	else
		printf("can't access array[32999]\n");
	pi = &array[0];
	pi += 0x4000L;
	if (pi != &array[0x8000L]) error(0x1);
	printf("1 = %d, ", s[0].a);
	printf("-32768 = %d, ", s[1].a);
	printf("0 = %d, ", s[2].a);
	printf("32767 = %d, ", s[3].a);
	printf("-1 = %d\n", s[4].a);
	printf("one = [%s], ", s[0].b);
	printf("two = [%s], ", s[1].b);
	printf("three = [%s], ", s[2].b);
	printf("four h = [%s], ", s[3].b);
	printf("five = [%s]\n", s[4].b);
	printf("10 decimal = %ld, ", s[0].d);
	printf("10 octal   = %lo, ", s[1].d);
	printf("10 hex     = %lx\n", s[2].d);
	printf("100000000  = %ld, ", s[3].d);
	printf("-99999999  = %ld, ", s[4].d);

	l = i * i;				/* test assembler code (long=int*int) */
	rl = i * i;				/* ditto */

	rl = i * ri / i;		/* test sign extend of 32-bit product */

	i = 4;
	printf("-4 / i = %d, ",-4/i);
	printf("-4 / 4 = %d\n",-4/4);

	p1 = 0;
	p2 = 4;
	printf("(0-4)ptr - ptr = (long)%ld, ", p1-p2);
	printf("(0-4)ptr - ptr = (int)%d\n",(int)(p1-p2));
}

error(enum)
int enum;
{
	printf("error - %d\n",enum);
}

