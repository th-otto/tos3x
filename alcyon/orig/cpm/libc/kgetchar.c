#include "lib.h"
#include "klib.h"

FILE fin;

int xgetchar(NOTHING)
{
	char c;
	register int i;

	if (fileno(&fin) == 0)
	{
		if (read(0, &c, 1) <= 0 || c == 4)
			return 0;
		i = c;
		return (i & 0xff);
	}
	if (fin.cc <= 0)
	{
		fin.cp = &(fin.cbuf[0]);
		fin.cc = read(fileno(&fin), fin.cp, BLEN);
	}
	if (fin.cc <= 0)
	{
		return (0);
	}
	fin.cc--;
	i = *(fin.cp)++;
	return (i & 0xff);
}
