#include <stdio.h>

main(argc, argv)
int argc;
char *argv[];
{
	FILE *fopena(), *in, *out;
	int c;

	in=fopena(*++argv, "r");
	out=fopena(*++argv, "w");
	while ((c=getc(in))!=EOF)
		putc(c, out);
	fclose(in);
	fclose(out);
}
