#include <stdio.h>
#include <stdlib.h>

#define _(x) x


static VOID usage(NOTHING)
{
	puts(_("Usage: doit [-i] [-d] [-z] fname\n"));
	puts(_("-d: display the archive index verbatim (dump it)"));
	puts(_("-i: display an imports/exports index of the archive"));
	puts(_("-w: display warnings about multiply-defined externals"));
	puts(_("-z: display (lots of) debugging information"));
	exit(EXIT_FAILURE);
}


VOID main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	if (argc == 1)
		usage();
}
