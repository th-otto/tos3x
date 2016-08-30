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
	return (ibuf->_fd = xcreat(fname, 2, binary));
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
	return c;
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
	return w;
}


int xputwp(P(unsigned short *) w, P(FILE *) ibuf)
PP(unsigned short *w;)
PP(FILE *ibuf;)
{
	return xputw(*w, ibuf);
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
	return 0;
}
