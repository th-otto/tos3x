#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cout.h"
struct hdr couthd = { 0 };
#include "util.h"
#include "../libsrc/klib.h"
#include "sendc68.h"

#ifdef __ALCYON__
FILE ibuf = { 0 };
#else
struct _iobuf ibuf;
#endif
const char *ifilname = 0;

int hdsize = 0;
int loctr = 0; /* BUG: unused */
int pflg = 0; /* BUG: unused */
int syno = 0;
int symflg; /* BUG: unused */

#ifdef __ALCYON__
extern int fout;
#endif

#ifdef __ALCYON__
#define EXIT() exit()
#else
#define EXIT() exit(EXIT_FAILURE)
#endif

VOID prtflags PROTO((unsigned int af));
VOID openfile PROTO((const char *ap));
VOID readhdr PROTO((NOTHING));

extern int nofloat;
extern int maxfiles5;
extern int nowildcards;


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	union mlong l;
	long l1;
	register int i, j, k;

	if (argc < 2)
	{
		printf("Usage: nm68 objectfile\n");
		EXIT();
	}
	hdsize = HDSIZE;
#ifdef __ALCYON__
	fout = dup(1);
#endif
	i = 1;
	openfile(argv[i]);
	l.l = couthd.ch_tsize + couthd.ch_dsize + hdsize;
	lseek(fileno(&ibuf), l.l, 0);
#ifdef __ALCYON__
	ibuf.cc = 0;
#endif
	l1 = couthd.ch_ssize;
	syno = 0;
	while (l1)
	{
		syno++;
		for (i = 0; i < 8; i++)			/* Print Symbol name */
		{
			k = getc(&ibuf) & 0377;
			if (k < ' ')
				k = ' ';				/* Filter ctrl chars */
			putchar(k);
		}
		putchar((char) '\t');
		j = getw(&ibuf);				/* flags */
		l.u.hiword = getw(&ibuf);
		l.u.loword = getw(&ibuf);
		printf("%8lx", l.l);
		prtflags(j);
		l1 -= OSTSIZE;
	}
#ifndef __ALCYON__
	return EXIT_SUCCESS;
#endif
}


VOID prtflags(P(unsigned int) af)
PP(unsigned int af;)
{
	register unsigned int f;

	f = af;
	if (f & SYEQ)
		printf(" equ");
	if (f & SYGL)
		printf(" global");
	if (f & SYER)
		printf(" reg");
	if (f & SYXR)
		printf(" external");
	if (f & SYDA)
		printf(" data");
	else if (f & SYTX)
		printf(" text");
	else if (f & SYBS)
		printf(" bss");
	else
		printf(" abs");
	putchar((char) '\n');
}


VOID openfile(P(const char *) ap)
PP(const char *ap;)
{
	register const char *p;

	p = ap;
	if ((ibuf._fd = fopen(p, &ibuf, 1)) < 0)
	{
		printf("unable to open %s\n", p);
		EXIT();
	}
	ifilname = p;						/*point to current file name for error msgs */
	if (read(fileno(&ibuf), &couthd, 2) != 2)
	{
		printf("read error on file: %s\n", ifilname);
		EXIT();
	}
#ifdef __ALCYON__
	ibuf.cc = 0;
#endif
	lseek(fileno(&ibuf), 0L, SEEK_SET);
	readhdr();							/*read file header */
}


VOID readhdr(NOTHING)
{
	register int i;
	register short *p;

	p = (short *)&couthd;
	for (i = 0; i < HDSIZE / 2; i++)
		*p++ = getw(&ibuf);
	if (couthd.ch_magic != MAGIC)
	{
		if (couthd.ch_magic == MAGIC1)
		{
			hdsize += 8;
		} else
		{
			printf("file format error: %s\n", ifilname);
			EXIT();
		}
	}
}


/*
 * these are here only to be able to compile & link with non-Alcyon compilers;
 * running the program will not work
 */

#ifndef __ALCYON__
int xfopen PROTO((const char *fname, FILE *i, int binary)) { return 0; }
int xgetw PROTO((FILE *i)) { return EOF; }
int xgetc PROTO((FILE *i)) { return EOF; }
#endif
