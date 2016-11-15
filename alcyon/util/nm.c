/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca. 92121
*/

#include <stdio.h>
#include <stdlib.h>

#include <cout.h>
#include <sendc68.h>
#include <option.h>
#include "util.h"

const char *ifilname;
FILE *ifp;
struct hdr2 couthd;

char eflag, gflag, qflag, xflag, dflag, tflag, bflag, aflag;



static VOID prtflags(P(unsigned int) af)
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
	putchar('\n');
}


static int accept(P(int) af, P(int) optioncount)
PP(int af;)
PP(int optioncount;)
{
	register int f;
	int pft;

	pft = 0;
	if (optioncount == 0)
		return 1;

	f = af;
	if (eflag && (f & SYEQ))
		pft = 1;
	if (gflag && (f & SYGL))
		pft = 1;
	if (qflag && (f & SYER))
		pft = 1;
	if (xflag && (f & SYXR))
		pft = 1;
	if (dflag && (f & SYDA))
		pft = 1;
	if (tflag && (f & SYTX))
		pft = 1;
	if (bflag && (f & SYBS))
		pft = 1;
	if (aflag && ((!(f & SYDA)) && (!(f & SYTX)) && (!(f & SYBS))))
		pft = 1;

	return pft;
}


static int readhdr(NOTHING)
{
	if (getchd(ifp, &couthd) != 0)
	{
		fprintf(stderr, "error reading %s\n", ifilname);
		return 0;
	}

	if (couthd.ch_magic < MAGIC || couthd.ch_magic > MAGICST)
	{
		fprintf(stderr, "file format error: %s %x\n", ifilname, couthd.ch_magic);
		return 0;
	}
	return 1;
}


static int openfile(P(const char *) ap)
PP(const char *ap;)
{
	register const char *p;

	p = ap;
	if ((ifp = fopen(p, "rb")) == NULL)
	{
		fprintf(stderr, "unable to open %s\n", p);
		return 0;
	}
	ifilname = p;						/* point to current file name for error msgs */
	return readhdr();					/* read file header */
}


#ifdef PDP11
static int longseek(al, fildes, pn)
long al;
int fildes;
int pn;
{
	long l;
	register b, o;

	l = al >> 9;
	b = l.loword;						/* block # */
	o = al.loword & 0777;				/* offset in block */
	if (doseek(fildes, b, pn) == 0)		/* seek to block */
		return 0;
	if (doseek(fildes, o, SEEK_CUR) == 0)	/* do offset */
		return 0;
	return 1;
}


static int doseek(afd, aoff, apnam)
{
	if (seek(afd, aoff, apnam) < 0)
	{
		fprintf(stderr, "seek error on file %s\n", ifilname);
		return 0;
	}
	return 1;
}
#endif


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register char *p;
	register long symsize;
	register int i, c, tellem;
	long l, value;
	unsigned short flags;
	int optioncount, argc_old;

	char symbol[SYNAMLEN + 1];

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (argc < 2)
	{
		printf("Usage: nm68 objectfile\n");
		return EXIT_FAILURE;
	}
	eflag = gflag = qflag = xflag = dflag = tflag = bflag = aflag = 0;
	argc_old = argc;
	optioncount = 0;

	while (argc > 1)					/* set flags, ignore filenames  */
	{
		if (*argv[argc - 1] == '-')
		{
			optioncount++;
			switch (*++argv[--argc])
			{
			case 'e':
				eflag = 1;
				break;
			case 'g':
				gflag = 1;
				break;
			case 'q':
				qflag = 1;
				break;
			case 'x':
				xflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			case 't':
				tflag = 1;
				break;
			case 'b':
				bflag = 1;
				break;
			case 'a':
				aflag = 1;
				break;
			default:
				fprintf(stderr, "nm: nonexistent option %s\n", argv[argc]);
				optioncount--;
				break;
			}
			--argv[argc];
		} else
		{
			argc--;
		}
	}
	argc = argc_old;

	if (((argc - optioncount) < 1) || (argc > (3 + optioncount)))
		tellem = 1;
	else
		tellem = 0;
	if ((argc - optioncount) <= 1)
		*argv = "c.out";				/* default */
	else
	{
		--argc;
		++argv;
	}

	/*  process each file - ignore options in parameter list    */

	while (argc--)
	{
		if (**argv == '-')
		{
			argv++;
			continue;
		}
		if (openfile(*argv++) == 0)
			continue;
		if (tellem)
			printf("%s:\n", ifilname);
		l = couthd.ch_tsize + couthd.ch_dsize + HDSIZE;
#ifdef PDP11
		if (longseek(l, fileno(ifp), 3) == 0)
#else
		if (fseek(ifp, l, 0) == EOF)
#endif
		{
			fclose(ifp);
			continue;
		}

		for (symsize = couthd.ch_ssize; symsize > 0; symsize -= OSTSIZE)
		{
			p = symbol;
			for (i = SYNAMLEN; --i >= 0;)
			{
				if ((c = getc(ifp)) > 0)
					*p++ = c;
			}
			*p = '\0';
			lgetw(&flags, ifp);
			lgetl(&value, ifp);
			if (accept(flags, optioncount))
			{
				printf("%-*s", SYNAMLEN + 3, symbol);
				printf("%8lx", value);
				prtflags(flags);
			}
		}
	}
	return EXIT_SUCCESS;
}
