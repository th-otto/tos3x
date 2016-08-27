#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sendc68.h>
#include "fext.h"

#define _(x) x

#define INBSIZE 16384L

char c1 = 1;

#define buf copybuf
int infil;
int outfil;
char buf[INBSIZE];


VOID relmod PROTO((int fi, int fo));
VOID strcpy PROTO((char *dst, const char *src));

VOID randw PROTO((long l));
long lrandw PROTO((NOTHING));


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register int i;
	char ifilnam[128];
	char ofilnam[128];
	char *arg;
	int fi;
	int fo;
	
	if (argc == 1)
	{
		puts(_("RELMOD version 1.01\n"));
		puts(_("Usage: relmod [-]inputfile[.68K] [outputfile[.PRG]] ...\n"));
		exit(EXIT_FAILURE);
	}
	for (i = 1; i < argc; )
	{
		arg = argv[i];
		if (*arg == '-')
		{
			arg++;
		}
		strcpy(ifilnam, arg);
		if (++i < argc && argv[i][0] != '-')
		{
			strcpy(ofilnam, argv[i]);
			++i;
		} else
		{
			strcpy(ofilnam, ifilnam);
		}
		fext(ifilnam, ".68K", 0);
		fext(ofilnam, ".PRG", 0);
		if ((fi = openb(ifilnam, O_RDONLY)) < 0)
		{
			puts(_("Cannot open: "));
			puts(ifilnam);
			puts("\n");
			continue;
		}
		if ((fo = creatb(ofilnam, O_WRONLY)) < 0)
		{
			close(fi);
			puts(_("Cannot create: "));
			puts(ofilnam);
			puts("\n");
			continue;
		}
		relmod(fi, fo);
		close(fi);
		close(fo);
	}
	
#ifndef __ALCYON__
	return EXIT_SUCCESS;
#endif
}


VOID relmod(P(int) fi, P(int) fo)
PP(int fi;)
PP(int fo;)
{
	long header[4];
	long ipos;
	long opos;
	int i;
	short w;
	short diff; /* BUG: int(16 bit) only */
	char c;
	
	infil = fi;
	outfil = fo;
	randw(2L);
	for (i = 0; i < 4; i++)
	{
		header[i] = lrandw();
	}
	randw(10L);
	randw(header[0] + header[1] + header[3]);
	ipos = opos = 0;
	for (; read(infil, &w, 2) == 2; ipos += 2)
	{
		w &= RBMASK;
		if (w == LUPPER)
		{
			read(infil, &w, 2);
			w = (w - 1) & RBMASK;
			if (w < BRELOC)
			{
				if (opos == 0)
				{
					write(outfil, &ipos, 4);
				} else
				{
					diff = ipos - opos;
					while (diff > 254)
					{
						diff = diff + -254;
						write(outfil, &c1, 1);
					}
					c = diff;
					write(outfil, &c, 1);
				}
				opos = ipos;
			}
			ipos += 2;
		}
	}
	c = 0;
	write(outfil, &c, 1);
}


VOID randw(P(long) l)
PP(long l;)
{
	while (l > INBSIZE)
	{
		randw(INBSIZE);
		l -= INBSIZE;
	}
	read(infil, buf, (size_t) l);
	write(outfil, buf, (size_t) l);
}


long lrandw(NOTHING)
{
	union {
		char buf[4];
		long l;
	} u;
	read(infil, u.buf, 4);
	write(outfil, u.buf, 4);
	return u.l;
}


/*
 * ugly hack, but, although not returning any value,
 * this function was actually named strcpy.
 * The reference in xnwmain.o was resolved here.
 */
VOID strcpy(P(char *) dst, P(const char *) src)
PP(char *dst;)
PP(const char *src;)
{
	while (*src)
		*dst++ = *src++;
	*dst++ = '\0';
}


/*
 * these are here only to be able to compile & link with non-Alcyon compilers;
 * running the program will not work
 */

#ifndef __ALCYON__
int openb PROTO((const char *fname, int mode, ...)) { return -1; }
int creatb PROTO((const char *fname, int mode)) { return -1; }
#endif
