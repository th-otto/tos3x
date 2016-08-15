#include <stdio.h>
#include <klib.h>
#include "cout.h"
#define MAGIC1 MAGIC+1

int fd = 0;
char *fn = { "c.out" };

main(argc, argv)
char **argv;
{

	register i,
	 j;

	long l;

	i = 1;
	if (argc < 2)
		goto dodef;
	for (i = 1; i < argc; i++)
	{
		fn = argv[i];
	  dodef:
		if ((fd = open(fn, 0, 1)) < 0)
		{
			printf("unable to open %s\n", fn);
			exit();
		}
		if ((j = read(fd, &couthd, HDSIZE)) != HDSIZE)
		{
			printf("read error on %s\n", fn);
			exit();
		}
		if (couthd.ch_magic != MAGIC && couthd.ch_magic != MAGIC1)
		{
			printf("File format error: %s\n", fn);
			exit();
		}
		printf("%s:", fn);
		l = couthd.ch_tsize + couthd.ch_dsize + couthd.ch_bsize;
		ldec(couthd.ch_tsize);
		putchar('+');
		ldec(couthd.ch_dsize);
		putchar('+');
		ldec(couthd.ch_bsize);
		putchar('=');
		ldec(l);
		printf(" (");
		lhex(l);
		printf(" )  stack size = ");
		ldec(couthd.ch_stksize);
		putchar('\n');
		if (couthd.ch_magic == MAGIC1)
		{
			read(fd, &l, 4);
			printf("data start=");
			lhex(l);
			read(fd, &l, 4);
			printf("  bss start=");
			lhex(l);
			putchar('\n');
		}
	}
}

char ostr[80] = { 0 };

lhex(al)
long al;
{

	register i;

	register char *p;

	register j;

	p = &ostr[80];
	*--p = 0;
	for (i = 0; i < 8; i++)
	{
		j = al & 017;
		if (j < 10)
			j += '0';
		else
			j += 'A' - 10;
		*--p = j;
		al >>= 4;
		if (al == 0)
			break;
	}
	printf("%s", p);
}

ldec(al)
long al;
{

	register i;

	register char *p;

	register j;

	p = &ostr[80];
	*--p = 0;
	for (i = 0; i < 10; i++)
	{
		j = al % 10;
		j += '0';
		*--p = j;
		al /= 10;
		if (al == 0)
			break;
	}
	printf("%s", p);
}
