#ifdef __linux__
#include "../common/linux/libcwrap.h"
#endif
#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <cout.h>
#include <sendc68.h>

#define _(x) x

#define FALSE 0
#define TRUE 1

static FILE *infil;
static FILE *outfil;




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


static char *fext(P(char *) filename, P(const char *) ext, P(int) flag)
PP(char *filename;)
PP(const char *ext;)
PP(int flag;)
{
	char *pext;
	char *p;
	
	for (p = filename; *p != '\0'; p++)
		;
	for (; p > filename && p[-1] != '\\' && p[-1] != '/'; )
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




static VOID skipbytes(P(long) l)
PP(long l;)
{
	fseek(infil, (size_t) l, SEEK_CUR);
}


static int reldump(P(const char *) fname)
PP(const char *fname;)
{
	long header[4];
	long ipos;
	int i;
	short w;
	unsigned short magic;
	
	magic = get16be(infil);
	for (i = 0; i < 4; i++)
	{
		header[i] = get32be(infil);
	}
	if (magic == MAGIC)
	{
		skipbytes((long)HDSIZE - 2);
	} else if (magic == MAGIC1)
	{
		skipbytes((long) HDSIZ2 - 2);
	} else
	{
		fprintf(stderr, _("%s: unknown magic $%04x\n"), fname, magic);
		return FALSE;
	}
	
	/*
	 * skip text, data and symbols
	 */
	skipbytes(header[0] + header[1] + header[3]);

	if (magic == MAGIC)
	{
		ipos = 0;
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
					fprintf(outfil, "%08lx\n", ipos);
				}
				ipos += 2;
			}
			w = get16be(infil);
		}
	}
	return TRUE;
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register int i;
	char ifilnam[128];
	const char *ofilnam;
	char *arg;
	int ret;
	int status;
	
#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (argc == 1)
	{
		puts(_("RELDUMP version 1.00\n"));
		puts(_("Usage: relmod [-]inputfile[.68k] [outputfile] ...\n"));
		return EXIT_FAILURE;
	}
	status = EXIT_SUCCESS;
	ofilnam = NULL;
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
			ofilnam = argv[i];
			++i;
		}
		fext(ifilnam, ".68k", 0);
		if ((infil = fopen(ifilnam, "rb")) == NULL)
		{
			fprintf(stderr, _("Cannot open: %s\n"), ifilnam);
			continue;
		}
		if (ofilnam == NULL || strcmp(ofilnam, "-") == 0)
		{
			outfil = stdout;
		} else
		{
			if ((outfil = fopen(ofilnam, "w")) == NULL)
			{
				fclose(infil);
				fprintf(stderr, _("Cannot create: %s\n"), ofilnam);
				continue;
			}
		}
		ret = reldump(ifilnam);
		fclose(infil);
		fflush(outfil);
		if (outfil != stdout)
			fclose(outfil);
		if (ret == FALSE)
		{
			status = EXIT_FAILURE;
		}
	}
	
	return status;
}
