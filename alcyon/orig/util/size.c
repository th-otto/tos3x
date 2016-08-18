#include <stdio.h>
#include <stdlib.h>
#include <cout.h>

extern int nofloat;

long l;
FILE *stream;

#ifdef __GNUC__
#define fopenb(f, mode) fopen(f, "rb")

#undef getw
#define getw get16be
static int getw(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	unsigned int c1 = getc(sp) & 0xff;
	unsigned int c2 = getc(sp) & 0xff;
	unsigned int w = (c1 << 8) | c2;
	return w;
}

#undef getl
#define getl get32be
static long getl(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	unsigned int w1 = getw(sp);
	unsigned int w2 = getw(sp);
	long ll = ((long)w1 << 16) | w2;
	return ll;
}

#endif

VOID print_l PROTO((const char *tag));


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	int w;
	int i;

	if (argc < 2)
	{
		puts("ABORT.  Usage is:  size68 file_name_list\n");
	} else
	{
		for (i = 1; i < argc; i++)
		{
			if ((stream = fopenb(argv[i], "r")) == NULL)
			{
				printf("\tCannot open %s.\n", argv[i]);
				continue;
			}
			printf("\n%s:\n", argv[i]);
			w = getw(stream);
			if (w == MAGIC)
			{
				puts("\tContiguous");
			} else if (w == MAGIC1)
			{
				puts("\tNon-contiguous");
			} else 
			{
				puts("\tNot a program file.");
				fclose(stream);
				continue;
			}
			print_l(".text length\t\t");
			print_l(".data length\t\t");
			print_l(".bss length\t\t");
			print_l("Symbol table length\t");
			getl(stream); /* skip stksize */
			print_l("Start of .text\t\t");
			if (getw(stream) != 0)
			{
				puts("\tNo relocation information in file.");
			} else
			{
				puts("\tFile is relocatable.");
			}
			if (w == MAGIC1)
			{
				print_l("Start of .data\t\t");
				print_l("Start of .bss\t\t");
			}
			fclose(stream);
		}
	}
#ifdef __GNUC__
	return 0;
#endif
}


VOID print_l(P(const char *) tag)
PP(const char *tag;)
{
	long ll;
	
	ll = getl(stream);
	printf("\t%s= %6ld\t%8lx\n", tag, ll, ll);
}
