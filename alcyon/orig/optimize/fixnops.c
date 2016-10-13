/*
 * fixnops - an ugly hack.
 *
 * The compiler often generates branches to a label of the next
 * instruction (i.e. this always happens with a return statement
 * as the last line of a function). The assembler included in
 * the package detects those branches and removes them,
 * unless the -n switch is ommitted. If you omit the switch,
 * a later pass will turn those branches into nops.
 * Apparently, the original version of optimize.c was compiled
 * with an older version of the assembler, which does not
 * insert the nops, and leaves the original "bra.w *+4".
 * This hack re-inserts the branch instruction into the object file.
 */
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <osbind.h>

#define _(x) x

struct hdr
{
	unsigned short ch_magic;			/* c.out magic number 060016 = $600E */
	long ch_tsize;						/* text size */
	long ch_dsize;						/* data size */
	long ch_bsize;						/* bss size */
	long ch_ssize;						/* symbol table size */
	long ch_stksize;					/* stack size */
	long ch_entry;						/* location of entry point */
	unsigned short ch_rlbflg;			/* relocation bits present flag, must be signed */
};
#define MAGIC		(unsigned short) 0x601a	/*  bra .+26 instruction */

#define BOOLEAN int
#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define _(x) x

static char const program_name[] = "fixnops";

static struct hdr hdr;


#ifdef __ALCYON__
char *_petoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	return 0;
}


char *_pftoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	return 0;
}


char *_pgtoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	return 0;
}
#endif



static VOID Error(P(const char *)str)
PP(const char *str;)
{
	fprintf(stderr, "%s: %s\n", program_name, str);
	exit(EXIT_FAILURE);
}



static VOID usage(NOTHING)
{
	Error(_("usage: %s <files>"));
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register int fd;
	register const char *filename;
	register long i, size;
	unsigned short w;
	register long count;
	
	if (argc != 2)
		usage();
	
	filename = argv[1];
	fd = Fopen(filename, 2);
	if (fd < 0)
	{
		fprintf(stderr, _("can't open %s\n"), filename);
		return EXIT_FAILURE;
	}
	Fread(fd, (long)sizeof(hdr), &hdr);
	if (hdr.ch_magic != MAGIC)
	{
		fprintf(stderr, _("not an object file: %s\n"), filename);
		return EXIT_FAILURE;
	}
	size = hdr.ch_tsize;
	count = 0;
	for (i = 0; i < size; i += 2)
	{
		Fread(fd, 2L, &w);
		if (w == 0x4e71)
		{
			w = 0;
			Fread(fd, 2L, &w);
			i += 2;
			if (w == 0x4e71)
			{
				Fseek(-4L, fd, SEEK_CUR);
				w = 0x6000;
				Fwrite(fd, 2L, &w);
				w = 0x0002;
				Fwrite(fd, 2L, &w);
				count++;
			}
		}
	}
	Fclose(fd);
	printf("%ld nops removed\n", count);
	
	return EXIT_SUCCESS;
}
