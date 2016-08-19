#include "lib.h"
#include "klib.h"

FILE fout = { 0, BLEN, &fout.cbuf[0] };

int xputchar(P(char) cc)
PP(char cc;)
{
	if (fileno(&fout) <= 1)
	{
		if (write(1, &cc, 1) != 1)
			return EOF;
		return cc;
	}
	/* buffered output */
	if (fout.cc <= 0)
	{
		fout.cp = &(fout.cbuf[0]);
		if (write(fileno(&fout), fout.cp, BLEN) != BLEN)
			return (-1);
		fout.cc = BLEN;
	}
	*(fout.cp)++ = cc;
	fout.cc--;
	return cc;
}

int xfoflush(NOTHING)
{
	register int i;

	i = BLEN - fout.cc;
	fout.cc = BLEN;
	fout.cp = &(fout.cbuf[0]);
	if (write(fileno(&fout), fout.cp, i) != i)
		return EOF;
	return 0;
}
