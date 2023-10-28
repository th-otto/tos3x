#include "../libsrc/lib.h"
#include "../libsrc/klib.h"
#include <fcntl.h>


int xfopen(P(const char *) fname, P(register FILE *) ibuf, P(int) binary)
PP(const char *fname;)
PP(register FILE *ibuf;)
PP(int binary;)
{
	ibuf->cc = 0;						/*no chars */
	binary = (binary == 0) ? 0 : 1;
#ifdef __ALCYON__
	ibuf->_fd = xopen(fname, O_RDONLY, binary);
	asm("nop");
#else
	return ibuf->_fd = xopen(fname, O_RDONLY, binary);
#endif
}


int xgetc(P(register FILE *) ibuf)
PP(register FILE *ibuf;)
{
	if (ibuf->cc <= 0)
	{
		ibuf->cp = &(ibuf->cbuf[0]);
		ibuf->cc = read(fileno(ibuf), ibuf->cp, BLEN);
	}
	if (ibuf->cc <= 0)
	{
		return (-1);
	}
	ibuf->cc--;
#ifdef __ALCYON__
	asm("movea.l    4(a5),a0");
	asm("move.b     (a0),d0");
	asm("ext.w      d0");
	asm("and.w      #$00FF,d0");
	asm("addq.l     #1,4(a5)");
	asm("nop");
#else
	return (int) (*(ibuf->cp)++) & 0xff;
#endif
}


int xgetw(P(register FILE *) ibuf)
PP(register FILE *ibuf;)
{
	register int j;
	register int i;

	if ((j = getc(ibuf)) == EOF)
		return EOF;
	i = (j & 0377) << 8;
	if ((j = getc(ibuf)) == EOF)
		return EOF;
	i |= (j & 0377);
	if (i & 0100000)
		i |= 0xffff0000;				/* make it negative */
#ifdef __ALCYON__
	asm("move.w     d6,d0");
	asm("nop");
#else
	return i;
#endif
}
