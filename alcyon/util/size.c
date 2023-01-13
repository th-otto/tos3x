#include "../common/linux/libcwrap.h"
#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <cout.h>
#undef MAXFILES
#include "../include/option.h"
#include "util.h"

static FILE *stream;

static unsigned short get16be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned int w1;
	w1 = getc(sp) & 0xff;
	w1 <<= 8;
	w1 |= getc(sp) & 0xff;
	return w1;
}


static long get32be(P(FILE *) sp)
PP(register FILE *sp;)				/* the stream to get from   */
{
	register unsigned short w1, w2;
	w1 = get16be(sp);
	w2 = get16be(sp);
	return ((long)w1 << 16) | w2;
}


static VOID print_l(P(const char *) tag)
PP(const char *tag;)
{
	long l;
	
	l = get32be(stream);
	printf("  %-30s- %6ld  %8lx\n", tag, l, l);
}


#if defined(__LINUX_GLIBC_WRAP_H)

/* ugly hack to get __libc_start_main versioned */

#if __GLIBC_PREREQ(2, 34)

#define STR_(s) #s
#define STR(s)  STR_(s)
#include <dlfcn.h>

#ifdef __UCLIBC__
#define __libc_start_main       __uClibc_main
#endif

#ifdef __cplusplus
extern "C" {
#endif
int __libc_start_main(
        int (*main)(int,char**,char**), int ac, char **av,
        int (*init)(void), void (*fini)(void),
        void (*rtld_fini)(void), void *stack_end);
int __libc_start_main(
        int (*main)(int,char**,char**), int ac, char **av,
        int (*init)(void), void (*fini)(void),
        void (*rtld_fini)(void), void *stack_end)
{
	typeof(__libc_start_main) *real_lsm;
	if ((*(void**)&real_lsm = dlsym(RTLD_NEXT, STR(__libc_start_main))) != 0)
		return real_lsm(main, ac, av, init, fini, rtld_fini, stack_end);
	fputs("BUG: dlsym error\n", stderr);
	return 1;
}
#ifdef __cplusplus
}
#endif
#undef STR
#undef STR_
#endif
#endif


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	register unsigned short w;
	register int i;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

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
