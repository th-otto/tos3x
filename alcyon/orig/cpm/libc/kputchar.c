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
			xwritefail();
		fout.cc = BLEN;
	}
	*(fout.cp)++ = cc;
	fout.cc--;
#ifdef __ALCYON__
	asm("move.b 9(a6),d0");
	asm("ext.w d0");
	asm("nop");
#else
	return cc;
#endif
}

int xfoflush(NOTHING)
{
	register int i;

	i = BLEN - fout.cc;
	fout.cc = BLEN;
	fout.cp = &(fout.cbuf[0]);
	if (write(fileno(&fout), fout.cp, i) != i)
		xwritefail();
#ifdef __ALCYON__
	asm("clr.w d0");
	asm("nop");
#else
	return 0;
#endif
}
