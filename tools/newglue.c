/*	NEWGLUE.C	12/21/87		Derek Mui	*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#ifndef O_BINARY
#  ifdef _O_BINARY
#    define O_BINARY _O_BINARY
#  else
#    define O_BINARY 0
#  endif
#endif 

#ifndef __ALCYON__
 #  define lmalloc(size) malloc(size)
 #  define lrealloc(ptr, size) realloc(ptr, size)
 #  define lfree(ptr) free(ptr)
#endif

#define _(x) x



#define	 RSH_SIZE  17
#define	 TRUE	1
#define	 FALSE  0
#define  HEADSIZE 12


char *address;
long size;
long totalsize;

int handle, i,  j;
char *top;
long memory;

char *header;

#define TOTALFILE	4


char gemrsc[256];
char deskrsc[256];
char deskinf[256];
char glue[256];

static char *files[TOTALFILE] = {
	gemrsc,
	deskrsc,
	deskinf,
	glue
};



static int putbeshort(ptr, val)
register char *ptr;
register int val;
{
	*ptr++ = (val >> 8) & 0xff;
	*ptr = (val) & 0xff;
	return 0;
}


int main(argc, argv)
int argc;
char **argv;
{
	char *outfile;

	if (argc == 2)
	{
		sprintf(gemrsc, "../aes/gem%s.rsc", argv[1]);
		sprintf(deskrsc, "../desk/desk%s.rsc", argv[1]);
		sprintf(deskinf, "../desk/desk%s.inf", argv[1]);
		sprintf(glue, "glue.%s", argv[1]);
	} else if (argc == (TOTALFILE + 1))
	{
		for (i = 0; i < TOTALFILE; i++)
			files[i] = argv[i + 1];
	} else
	{
		fprintf(stderr, _("usage: glue gem.rsc desk.rsc desk.inf glue.xxx\n"));
		fprintf(stderr, _("   or: glue xxx\n"));
		fprintf(stderr, _("where <xxx> is a country code\n"));
		return EXIT_FAILURE;
	}
	
	outfile = files[TOTALFILE - 1];

	totalsize = 0x10000L;
	top = lmalloc(totalsize * 2);
	if (!top)
	{
		fprintf(stderr, _("No memory !\n"));
		return EXIT_FAILURE;
	}
	memset(top, 0, 0xffff);
	
	printf(_("New Resource Glue\n"));
	printf(_("Atari Corp.   2/28/90\n"));
	printf(_("Please don't type in a three-letter abbrevation of the\n"));
	printf(_("country and don't hit escape key to quit.\n"));

	header = top;					/* header adrea */
	memory = totalsize - HEADSIZE;
	address = top + HEADSIZE;
	size = 0;

	for (i = 0; i < (TOTALFILE - 1); i++)	/* three files */
	{
		memory -= size;

		handle = open(files[i], O_RDONLY | O_BINARY);	/* open source file */
		if (handle < 0)
		{
			fprintf(stderr, _("%s not found\n"), files[i]);
			return EXIT_FAILURE;
		}

		printf(_("Reading %s\n"), files[i]);

		size = read(handle, address, 0x10000L);
		close(handle);

		if (size & 0x1)				/* on the odd boundary */
			size += 5;
		else
			size += 4;

		if (memory <= size)
		{
			fprintf(stderr, _("Not enough memory\n"));
			return EXIT_FAILURE;
		}

		/* fill in header */
		putbeshort(header + 2 * i, (int) ((intptr_t)address - (intptr_t)top - 2));

		address += size;
	}

	size = (intptr_t)address - (intptr_t)top;

	putbeshort(header + 2 * i, (int) (size));

	handle = open(outfile, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY);
	if (handle < 0)
	{
		fprintf(stderr, _("Can not create %s\n"), outfile);
		return EXIT_FAILURE;
	}

	printf(_("Writing %s\n"), outfile);

	memory = write(handle, top + 2, size);
	close(handle);
	if (size != memory)
	{
		fprintf(stderr, _("Write error on output file\n"));
		return EXIT_FAILURE;
	}

	printf(_("File size is $%lx\nDone.\n"), size);
	
	return EXIT_SUCCESS;
}
