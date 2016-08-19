#include <stdio.h>
#include <stdlib.h>
#include <cout.h>
#include "util.h"

extern int nofloat;

static FILE *stream;

static unsigned short get16be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	unsigned int c1 = getc(sp) & 0xff;
	unsigned int c2 = getc(sp) & 0xff;
	return (c1 << 8) | c2;
}


static long get32be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	unsigned int w1 = get16be(sp);
	unsigned int w2 = get16be(sp);
	return ((long)w1 << 16) | w2;
}


static VOID print_l(P(const char *) tag)
PP(const char *tag;)
{
	long l;
	
	l = get32be(stream);
	printf("  %-30s- %6ld  %8lx\n", tag, l, l);
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register unsigned short w;
	register int i;

	if (argc < 2)
	{
		puts("ABORT.  Usage is:  size68 file_name_list\n");
	} else
	{
		for (i = 1; i < argc; i++)
		{
			if ((stream = fopen(argv[i], "rb")) == NULL)
			{
				printf("  Cannot open %s.\n", argv[i]);
				continue;
			}
			printf("\n%s:\n", argv[i]);
			if ((w = get16be(stream)) == MAGIC)
			{
				puts("  Contiguous");
			} else if (w == MAGIC1)
			{
				puts("  Non-contiguous");
			} else 
			{
				puts("  Not a program file.");
				fclose(stream);
				continue;
			}
			print_l(".text length");
			print_l(".data length");
			print_l(".bss length");
			print_l("Symbol table length");
			print_l("Stacksize");
			print_l("Program flags");
			if (getw(stream) != 0)
			{
				puts("  No relocation information in file.");
			} else
			{
				puts("  File is relocatable.");
			}
			if (w == MAGIC1)
			{
				print_l("Start of .data");
				print_l("Start of .bss");
			}
			fclose(stream);
		}
	}
	return 0;
}
