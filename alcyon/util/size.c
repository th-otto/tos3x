/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
*/

char *version "@(#)size68 - Apr 28, 1983";

#ifdef VAX11
#	include "cout.h"
#else
#	include <cout.h>
#endif

struct hdr couthd;

#define BSIZE 512
struct iob
{
	int fd;
	int cc;
	char *cp;
	char cbuf[BSIZE];
} iobuf;

int fflag,
 err;

main(argc, argv)
int argc;

char **argv;
{
	register char *p;

	for (argv++; argc > 1; argc--, argv++)
	{
		p = *argv;
		if (*p++ != '-')
			break;
		for (; *p != '\0'; p++)
		{
			switch (*p)
			{

			case 'f':
				fflag++;
				break;

			default:
				printf("bad flag %c\n", *p);
				exit(-1);
			}
		}
	}
	if (argc <= 1)
		dofile("a.out");
	else
	{
		while (--argc > 0)
			dofile(*argv++);
	}
	exit(err);
}

dofile(fn)
char *fn;
{
	register int i,
	 j;

	register struct hdr *hd;

	long l;

	hd = &couthd;
	if (fopen(fn, &iobuf) < 0)
	{
		printf("unable to open %s\n", fn);
		err++;
		return;
	}
	if (getchd(&iobuf, &couthd) == -1)
	{
		printf("read error on %s\n", fn);
		err++;
	} else
	{
		printf("%s:", fn);
		switch (hd->ch_magic)
		{

		case EX_MAGIC:
			break;

		case EX_ABMAGIC:
			printf("(stand alone)");
			break;

		case EX_2KSTXT:
			printf("(2k shared text)");
			break;

		case EX_IDMAGIC:
			printf("(I/D split)");
			break;

		case EX_4KSTXT:
			printf("(4k shared text)");
			break;

		default:
			printf(" not c.out format\n");
			close(iobuf.fd);
			return;
		}
		l = hd->ch_tsize + hd->ch_dsize + hd->ch_bsize;
		ldec(hd->ch_tsize);
		putchar('+');
		ldec(hd->ch_dsize);
		putchar('+');
		ldec(hd->ch_bsize);
		putchar('=');
		ldec(l);
		printf(" (");
		lhex(l);
		printf(")");
		if (hd->ch_stksize || fflag)
		{
			printf(" stack=");
			ldec(hd->ch_stksize);
		}
		if (hd->ch_entry < 0 || fflag)
		{
			printf(" break=");
			ldec(-hd->ch_entry);
		}
		if (fflag)
		{
			if (hd->ch_rlbflg == 0)
				printf(" (bits)");
			else
				printf(" (nobits)");
		}
		putchar('\n');
	}
	close(iobuf.fd);
}

char ostr[80];

lhex(al)
long al;
{

	register int i;

	register int j;

	register char *p;

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
	register char *p;

	p = &ostr[80];
	*--p = 0;
	do
	{
		*--p = (al % 10L) + '0';
		al = /10L;
	} while (al != 0);
	printf("%s", p);
}
