#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sendc68.h>

#define _(x) x

#define INBSIZE 16384L

static FILE *infil;
static FILE *outfil;
static char buf[INBSIZE];




static unsigned short get16be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned int w1;
	w1 = getc(sp);
	w1 <<= 8;
	return w1 | getc(sp);
}


static long get32be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned int w1;
	w1 = get16be(sp);
	return ((long)w1 << 16) | get16be(sp);
}


static VOID put16be(P(unsigned short) w, P(FILE *) sp)
PP(unsigned short w;)
PP(register FILE *sp;)
{
	fputc(w >> 8, sp);
	fputc(w, sp);
}


static VOID put32be(P(long) l, P(FILE *) sp)
PP(long l;)
PP(register FILE *sp;)
{
	put16be((unsigned short)(l >> 16), sp);
	put16be((unsigned short)l, sp);
}


static char *fext(P(char *) filename, P(const char *) ext, P(int) flag)
PP(char *filename;)
PP(const char *ext;)
PP(int flag;)
{
	char *pext;
	char *p;
	
	for (p = filename; *p != '\0'; p++)
		;
	for (; p > filename && p[-1] != '\\'; )
		--p;
	if (flag)
	{
		for (pext = p; *pext != '\0' && *pext != '.'; pext++)
			;
		*pext = '\0';
	}
	for (pext = p; *pext != '.'; pext++)
	{
		if (*pext == '\0')
		{
			strcat(p, ext);
			break;
		}
	}
	return filename;
}




static VOID randw(P(long) l)
PP(long l;)
{
	while (l > INBSIZE)
	{
		randw(INBSIZE);
		l -= INBSIZE;
	}
	fread(buf, 1, (size_t) l, infil);
	fwrite(buf, 1, (size_t) l, outfil);
}


static long lrandw(NOTHING)
{
	long l;
	l = get32be(infil);
	put32be(l, outfil);
	return l;
}


static VOID relmod(NOTHING)
{
	long header[4];
	long ipos;
	long opos;
	int i;
	short w;
	long diff;
	
	randw(2L);
	for (i = 0; i < 4; i++)
	{
		header[i] = lrandw();
	}
	randw(10L);
	randw(header[0] + header[1] + header[3]);
	ipos = opos = 0;
	w = get16be(infil);
	for (; !feof(infil); ipos += 2)
	{
		w &= RBMASK;
		if (w == LUPPER)
		{
			w = get16be(infil);
			w = (w - 1) & RBMASK;
			if (w < BRELOC)
			{
				if (opos == 0)
				{
					put32be(ipos, outfil);
				} else
				{
					diff = ipos - opos;
					while (diff > 254)
					{
						diff -= 254;
						fputc(1, outfil);
					}
					fputc((char)diff, outfil);
				}
				opos = ipos;
			}
			ipos += 2;
		}
		w = get16be(infil);
	}
	fputc(0, outfil);
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register int i;
	char ifilnam[128];
	char ofilnam[128];
	char *arg;
	
	if (argc == 1)
	{
		puts(_("RELMOD version 1.03\n"));
		puts(_("Usage: relmod [-]inputfile[.68k] [outputfile[.prg]] ...\n"));
		return EXIT_FAILURE;
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
		fext(ifilnam, ".68k", 0);
		fext(ofilnam, ".prg", 0);
		if ((infil = fopen(ifilnam, "rb")) == NULL)
		{
			fprintf(stderr, _("Cannot open: %s\n"), ifilnam);
			continue;
		}
		if ((outfil = fopen(ofilnam, "wb")) == NULL)
		{
			fclose(infil);
			fprintf(stderr, _("Cannot create: %s\n"), ofilnam);
			continue;
		}
		relmod();
		fclose(infil);
		fclose(outfil);
	}
	
	return EXIT_SUCCESS;
}
