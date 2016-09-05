#include "../libsrc/lib.h"
#include "../libsrc/klib.h"

int xfcreat(P(const char *) fname, P(register FILE *) ibuf, P(int) binary)
PP(const char *fname;)
PP(register FILE *ibuf;)
PP(int binary;)
{
	ibuf->cc = BLEN;					/* no chars */
	ibuf->cp = &(ibuf->cbuf[0]);
	binary = (binary == 0) ? 0 : 1;
#ifdef __ALCYON__
	ibuf->_fd = xcreat(fname, 2, binary);
	asm("nop");
#else
	return ibuf->_fd = xcreat(fname, 2, binary);
#endif
}


int xputc(P(char) c, P(register FILE *) ibuf)
PP(char c;)
PP(register FILE *ibuf;)
{
	if (ibuf->cc <= 0)
	{
		ibuf->cp = &(ibuf->cbuf[0]);
		if (write(fileno(ibuf), ibuf->cp, BLEN) != BLEN)
			xwritefail();
		ibuf->cc = BLEN;
	}
	*(ibuf->cp)++ = c;
	ibuf->cc--;
#ifdef __ALCYON__
	asm("move.b    9(a6),d0");
	asm("ext.w     d0");
	asm("nop");
#else
	return c;
#endif
}


int xputw(P(int) w, P(register FILE *) ibuf)
PP(int w;)
PP(register FILE *ibuf;)
{
	register int j;
	int i;

	putc((w >> 8) & 0xff, ibuf);
	putc(w & 0xff, ibuf);
	UNUSED(i);
	UNUSED(j);
#ifdef __ALCYON__
	asm("move.w    8(a6),d0");
	asm("nop");
#else
	return w;
#endif
}


int xputwp(P(unsigned short *) w, P(FILE *) ibuf)
PP(unsigned short *w;)
PP(FILE *ibuf;)
{
#ifdef __ALCYON__
	xputw(*w, ibuf);
	asm("nop");
#else
	return xputw(*w, ibuf);
#endif
}


int xfflush(P(register FILE *) ibuf)
PP(register FILE *ibuf;)
{
	register int i;

	i = BLEN - ibuf->cc;
	ibuf->cc = BLEN;
	ibuf->cp = &(ibuf->cbuf[0]);
	if (write(fileno(ibuf), ibuf->cp, i) != i)
		xwritefail();
#ifdef __ALCYON__
	asm("clr.w     d0");
	asm("nop");
#else
	return 0;
#endif
}
