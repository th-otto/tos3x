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
	return ibuf->_fd = xopen(fname, O_RDONLY, binary);
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
	return (int) (*(ibuf->cp)++) & 0xff;
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
	return i;
}
