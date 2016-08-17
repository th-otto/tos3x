/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#ifdef VAX11
#	include <c68/cout.h>
#else
#	include <cout.h>
#endif

struct hdr2 couthd;

/* set the stack size for a c.out format file */

char *version = "@(#)setstack - Mar 23, 1983";

main(argc, argv)
int argc;

char **argv;
{
	register int fd;

	register char *p;

#ifdef PDP11
	register int stksize,
	 brksize;

#	define atol(x)	atoi(x)
#else
	register long stksize,
	 brksize;

	long atol();
#endif

	if (argc == 4)
	{
		if ((brksize = atol(argv[3])) < 0)
		{
			printf("bad break size\n");
			exit();
		}
		argc--;
	}
	if (argc != 3)
	{
		printf("usage: %s filename stksize [brksize]\n", argv[0]);
		exit(-1);
	}
	if ((stksize = atol(argv[2])) < 0)
	{
		printf("bad stack size\n");
		exit();
	}
	if ((fd = open(argv[1], 2)) < 0)
	{
		printf("can't open %s\n", argv[1]);
		exit(-1);
	}
	if (fdgetchd(fd, &couthd) != 0)
	{
		printf("can't read %s\n", argv[1]);
		exit(-1);
	}
	brksize = (brksize + 1023) & (~1023);	/* round to 1K boundary */
	stksize = (stksize + 1023) & (~1023);	/* round to 1K boundary */

	switch (couthd.ch_magic)
	{
	case MAGIC:
	case MAGIC + 2:
	case MAGIC + 3:
	case MAGIC + 4:
		break;

	default:
		printf("%s: not c.out format\n", *argv);
		exit(-1);
	}
	if (stksize > 0)
		couthd.ch_stksize = stksize;
	if (brksize > 0)
		couthd.ch_entry = -brksize;
#ifdef PDP11
	seek(fd, 0, 0);
#else
	lseek(fd, 0L, 0);
#endif
	if (fdputchd(fd, &couthd) != 0)
	{
		printf("can't write %s\n", argv[1]);
		exit(-1);
	}
	exit(0);
}
