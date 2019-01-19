#include "../include/compiler.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/stdarg.h"
#include "klib.h"

#if KLUDGE

#ifndef O_BINARY
#define O_BINARY 0
#endif

static VOID xwritefail(NOTHING)
{
	write(2, "Write error on output file\n", sizeof("Write error on output file\n") - 1);
	exit(1);
}


FILE *xfopen(P(const char *) fname, P(register FILE *) f)
PP(const char *fname;)
PP(register FILE *f;)
{
	f->cc = 0;
	f->cp = &(f->cbuf[0]);
	f->_fd = open(fname, O_RDONLY|O_BINARY, 0644);
	if (f->_fd < 0)
		return NULL;
	return f;
}


FILE *xfcreat(P(const char *) fname, P(register FILE *) f)
PP(const char *fname;)
PP(register FILE *f;)
{
	f->cc = BLEN;					/* no chars */
	f->cp = &(f->cbuf[0]);
	f->_fd = open(fname, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0644);
	if (f->_fd < 0)
		return NULL;
	return f;
}


int xputc(P(char) c, P(register FILE *) f)
PP(char c;)
PP(register FILE *f;)
{
	if (f->cc <= 0)
	{
		xfflush(f);
	}
	*(f->cp)++ = c;
	f->cc--;
	return c;
}


int xfflush(P(register FILE *) f)
PP(register FILE *f;)
{
	register int i;
	int res;
	
	i = BLEN - f->cc;
	f->cc = BLEN;
	f->cp = &(f->cbuf[0]);
	res = write(f->_fd, f->cp, i);
	if (res != i)
	{
		xwritefail();
	}
	return 0;
}


int xgetc(P(register FILE *) f)
PP(register FILE *f;)
{
	if (f->cc <= 0)
	{
		f->cp = &(f->cbuf[0]);
		f->cc = read(f->_fd, f->cp, BLEN);
	}
	if (f->cc <= 0)
	{
		return (-1);
	}
	f->cc--;
	return (int) (*(f->cp)++) & 0xff;
}


int xfclose(P(register FILE *) f)
PP(register FILE *f;)
{
	if (f->_fd >= 3)
	{
		close(f->_fd);
		f->_fd = -1;
	}
	return 0;
}

#endif
