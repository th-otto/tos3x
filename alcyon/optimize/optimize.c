#include "../include/compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef __ALCYON__
#include <mint/osbind.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef __GNUC__
 # ifndef _LIBC
 #  define lmalloc(size) malloc(size)
 #  define lrealloc(ptr, size) realloc(ptr, size)
 #  define lfree(ptr) free(ptr)
 # endif
#endif

#define BOOLEAN int
#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define _(x) x

#undef MAXPATH
#define MAXPATH 256

static int clr_total;
static int clr1_total;
static int mov_total;
static int mov1_total;
static int imov_total;
static int imov1_total;
static int bra_total;
static int mv1_total;
static int mv2_total;
static int adda_total;

static long size;
static char *in_buff;
static char *out_buff;
static long nxt_line;
static char opath[MAXPATH];
static int stp_lines;
static int stp_symbols;
static int quiet;

/* stupid alcyon does not handle multiplication with constants */
#define MUL10(x) ((((x) << 2) + x) << 1)

#define ISNL(c) ((c) == '\n')

/*****************************************************************************/
/* ------------------------------------------------------------------------- */
/*****************************************************************************/

static VOID reverse(P(char *) str)
PP(register char *str;)
{
	register int c;
	register int i;
	register int len;
	
	for (i = 0, len = strlen(str) - 1; i < len; i++, len--)
	{
		c = str[i];
		str[i] = str[len];
		str[len] = c;
	}
}

/* ------------------------------------------------------------------------- */

static VOID panic(P(const char *)str)
PP(const char *str;)
{
	fprintf(stderr, _("PANIC: %s\nABORTING\n"), str);
	exit(EXIT_FAILURE);
}

/* ------------------------------------------------------------------------- */

static VOID htoa(P(char *)str, P(int) val)
PP(register char *str;)
PP(register int val;)
{
	register int i;
	
	for (i = 3; i >= 0; --i, val >>= 4)
	{
		*(i + str) = val;
		*(i + str) &= 0x0f;
		if (*(i + str) >= 10)
		{
			*(i + str) = *(i + str) + 'A' - 10;
		} else
		{
			*(i + str) = *(i + str) + '0';
		}
	}
}

/* ------------------------------------------------------------------------- */

static VOID itoa(P(int) val, P(char *)str)
PP(register int val;)
PP(register char *str;)
{
	register int i;
	register int x;
	
	if ((x = val) < 0)
	{
		val = -val;
	}
	i = 0;
	do {
		*(i + str) = val % 10 + '0';
		++i;
	} while ((val /= 10) > 0);
	if (x < 0)
	{
		*(i + str) = '-';
		++i;
	}
	*(i + str) = '\0';
	reverse(str);
}

/* ------------------------------------------------------------------------- */

static int myatoi(P(const char *)str)
PP(register const char *str;)
{
	register int i;
	register BOOLEAN sign;
	register long val;
	
	i = 0;
	sign = FALSE;
	if (*(i + str) == '-')
	{
		sign = TRUE;
		++i;
	}
	val = 0;
	while (*(i + str) >= '0' && *(i + str) <= '9')
	{
		val = MUL10(val) + *(i + str) - '0';
		++i;
		if (val > 32767L)
			panic(_("atoi overflow"));
	}
	if (sign)
		val = -val;
	return val;
}

/* ------------------------------------------------------------------------- */

static long getnum(P(char *)str, P(long) tmp_ind)
PP(register char *str;)
PP(register long tmp_ind;)
{
	register int i;
	
	i = 0;
	if (in_buff[tmp_ind] == '(')
	{
		*(i + str) = '0';
		++i;
		*(i + str) = '\0';
		++i;
		return tmp_ind;
	}
	if ((in_buff[tmp_ind] >= '0' && in_buff[tmp_ind] <= '9') ||
		(in_buff[tmp_ind] == '-' && (in_buff + tmp_ind)[1] >= '0' && (in_buff + tmp_ind)[1] <= '9'))
	{			
		if (in_buff[tmp_ind] == '-')
		{
			*(i + str) = *(tmp_ind + in_buff);
			++tmp_ind;
			++i;
		}
		while (in_buff[tmp_ind] >= '0' && in_buff[tmp_ind] <= '9')
		{
			*(i + str) = *(tmp_ind + in_buff);
			++tmp_ind;
			++i;
		}
		*(i + str) = '\0';
		return tmp_ind;
	}
	return -1;
}

/* ------------------------------------------------------------------------- */

static VOID nextline(NOTHING)
{
	do
	{
		while (in_buff[nxt_line] != '\n' && nxt_line < size)
		{
			++nxt_line;
		}
		if (in_buff[nxt_line] == '\n')
			++nxt_line;
	} while (in_buff[nxt_line] == '*' || in_buff[nxt_line] == ';');
}

/*****************************************************************************/
/* ------------------------------------------------------------------------- */
/*****************************************************************************/

#define COPYC() \
	*(out_ind + out_buff) = *(in_ind + in_buff); \
	++in_ind; \
	++out_ind

#define ADDC(c) \
	*(out_ind + out_buff) = c; \
	++out_ind

/* ------------------------------------------------------------------------- */

static VOID stripcr(NOTHING)
{
	register char *in;
	register char *end;
	register char *out;
	
	in = in_buff;
	end = in_buff + size;
	out = out_buff;
	while (in < end)
	{
		if (in[0] == 0x0d && in[1] == 0x0a)
		{
			*out++ = '\n';
			in += 2;
		} else if (in[0] == 0x0d || in[0] == 0x0a)
		{
			*out++ = '\n';
			++in;
		} else
		{
			*out++ = *in++;
		}
	}
	if (out > out_buff && out[-1] != '\n')
		*out++ = '\n';
	size = (intptr_t)out - (intptr_t)out_buff;
}

/* ------------------------------------------------------------------------- */

static VOID striplines(NOTHING)
{
	register char *in;
	register char *end;
	register char *out;
	
	in = in_buff;
	end = in_buff + size;
	out = out_buff;
	while (in < end)
	{
		if (in[0] == '*' || in[0] == ';')
		{
			while (in < end && in[0] != '\n')
				in++;
			in++;
		} else
		{
			while (in < end && in[0] != '\n')
				*out++ = *in++;
			*out++ = *in++;
		}
	}
	size = (intptr_t)out - (intptr_t)out_buff;
}

/* ------------------------------------------------------------------------- */

static VOID stripsyms(NOTHING)
{
	register char *in;
	register char *end;
	register char *out;
	
	in = in_buff;
	end = in_buff + size;
	out = out_buff;
	while (in < end)
	{
		if (in[0] == '~')
		{
			while (in < end && in[0] != '\n')
				in++;
			in++;
		} else
		{
			while (in < end && in[0] != '\n')
				*out++ = *in++;
			*out++ = *in++;
		}
	}
	size = (intptr_t)out - (intptr_t)out_buff;
}

/* ------------------------------------------------------------------------- */

static BOOLEAN addinrange(NOTHING)
{
	long i;
	long colon;
	long nxt;
	int j;
	BOOLEAN ret;
	int val;
	char numstr[10];
	char numstr1[10];
	
	ret = FALSE;
	nxt = nxt_line;
	for (;;)
	{
		if (nxt_line == size) /* should be >= */
			goto fail;
		nextline();
		colon = 0;
		i = nxt_line;
		while (i < size && in_buff[i] != '\n')
		{
			if (in_buff[i] == ':')
				colon = i;
			++i;
		}
		if (colon != 0)
			i = colon + 1;
		else
			i = nxt_line;
		if (((in_buff + i)[0] == 'j' || (in_buff + i)[0] == 'b') &&
			(in_buff + i)[1] == 's' &&
			(in_buff + i)[2] == 'r')
			break;
	}
	nextline();
	i = nxt_line;
	while (i < size && in_buff[i] != '\n')
	{
		++i;
	}
	if ((i + in_buff)[-3] != ',' ||
		(i + in_buff)[-2] != 's' ||
		(i + in_buff)[-1] != 'p')
		goto fail;
	if ((in_buff + nxt_line)[0] == 'a' &&
		(in_buff + nxt_line)[1] == 'd' /* &&
		(in_buff + nxt_line)[2] == 'd' */
		)
	{
		if ((in_buff + nxt_line)[3] == 'q')
		{
			/* addq can only be 2,4,6,8 */
			if ((in_buff + nxt_line)[8] == '8')
			{
				/* change addq.l to add.w */
				(in_buff + nxt_line)[3] = '.';
				(in_buff + nxt_line)[4] = 'w';
				(in_buff + nxt_line)[5] = ' ';
				(in_buff + nxt_line)[6] = '#';
				(in_buff + nxt_line)[7] = '1';
				(in_buff + nxt_line)[8] = '0';
			} else
			{
				(in_buff + nxt_line)[8] += 2;
			}
		} else
		{
			if ((in_buff + nxt_line)[3] == 'a')
			{
				i = nxt_line + 8;
			} else
			{
				i = nxt_line + 7;
			}
			val = myatoi(in_buff + i);
			itoa(val, numstr);
			itoa(val + 2, numstr1);
			if (strlen(numstr) != strlen(numstr1))
				goto fail;
			for (j = strlen(numstr1) - 1; j >= 0; --j)
				*(in_buff + (i + j)) = *(j + numstr1);
		}
		goto ok;
	} else
	{
		goto fail;
	}
ok:
	ret = TRUE;
fail:
	nxt_line = nxt;
	return ret;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     clr (sp)
 *     clr -(sp)
 * into
 *     clr.l -(sp)
 *     with stack correction for 2 bytes
 */
static VOID clrwtol(NOTHING)
{
	register long in_ind;
	register long out_ind;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'c')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'c' &&
			(in_buff + in_ind)[1] == 'l' &&
		/*	(in_buff + in_ind)[2] == 'r' && */
			(in_buff + in_ind)[3] == ' ' &&
			(in_buff + in_ind)[4] == '(' &&
			(in_buff + in_ind)[5] == 's' &&
		/*	(in_buff + in_ind)[6] == 'p' && */
		/*	(in_buff + in_ind)[7] == ')' && */
			ISNL((in_buff + in_ind)[8]))
		{
			nxt_line = in_ind;
			nextline();
			if ((in_buff + nxt_line)[0] == 'c' &&
				(in_buff + nxt_line)[1] == 'l' &&
			/*  (in_buff + nxt_line)[2] == 'r' && */
				(in_buff + nxt_line)[3] == ' ' &&
				(in_buff + nxt_line)[4] == '-' &&
				(in_buff + nxt_line)[5] == '(' &&
				(in_buff + nxt_line)[6] == 's' &&
				addinrange())
			{
				in_ind = nxt_line;
				COPYC();
				COPYC();
				COPYC();
				ADDC('.');
				ADDC('l');
				++clr1_total;
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     clr -(sp)
 *     clr -(sp)
 * into
 *     clr.l -(sp)
 */
static VOID clr2wtol(NOTHING)
{
	register long in_ind;
	register long out_ind;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'c')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'c' &&
			(in_buff + in_ind)[1] == 'l' &&
		/*	(in_buff + in_ind)[2] == 'r' && */
			(in_buff + in_ind)[3] == ' ' &&
			(in_buff + in_ind)[4] == '-' &&
			(in_buff + in_ind)[5] == '(' &&
			(in_buff + in_ind)[6] == 's')
		{
			nxt_line = in_ind;
			nextline();
			if ((in_buff + nxt_line)[0] == 'c' &&
				(in_buff + nxt_line)[1] == 'l' &&
			/*  (in_buff + nxt_line)[2] == 'r' && */
				(in_buff + nxt_line)[3] == ' ' &&
				(in_buff + nxt_line)[4] == '-' &&
				(in_buff + nxt_line)[5] == '(' &&
				(in_buff + nxt_line)[6] == 's')
			{
				in_ind = nxt_line;
				COPYC();
				COPYC();
				COPYC();
				ADDC('.');
				ADDC('l');
				++clr_total;
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move x+2(a6),(sp)
 *     move x(a6),-(sp)
 * into
 *     move.l x,-(sp)
 *     with stack correction for 2 bytes
 */
static VOID mvwtol(NOTHING)
{
	register long in_ind;
	register long out_ind;
	register long tmp_ind;
	
	char numstr[10];
	char numstr1[10];
	register char regnum;

	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'm')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ')
		{
			if ((tmp_ind = getnum(numstr, in_ind + 5)) >= 0 &&
				(in_buff + tmp_ind)[0] == '(' &&
				(in_buff + tmp_ind)[1] == 'R' &&
				(in_buff + tmp_ind)[2] == '1' &&
			/*	(in_buff + tmp_ind)[3] == '4' && */
				(in_buff + tmp_ind)[4] == ')' &&
			/*	(in_buff + tmp_ind)[5] == ',' && */
				(in_buff + tmp_ind)[6] == '(' &&
				(in_buff + tmp_ind)[7] == 's' &&
			/*	(in_buff + tmp_ind)[8] == 'p' && */
			/*	(in_buff + tmp_ind)[9] == ')' && */
				ISNL((in_buff + tmp_ind)[10]))
			{
				regnum = (in_buff + tmp_ind)[3];
				nxt_line = in_ind;
				nextline();
				if ((in_buff + nxt_line)[0] == 'm' &&
					(in_buff + nxt_line)[1] == 'o' &&
				/*	(in_buff + nxt_line)[2] == 'v' && */
				/*	(in_buff + nxt_line)[3] == 'e' && */
					(in_buff + nxt_line)[4] == ' ')
				{
					if ((tmp_ind = getnum(numstr1, nxt_line + 5)) >= 0 &&
						(in_buff + tmp_ind)[0] == '(' &&
						(in_buff + tmp_ind)[1] == 'R' &&
						(in_buff + tmp_ind)[2] == '1' &&
					/*	(in_buff + tmp_ind)[3] == '4' && */
						(in_buff + tmp_ind)[4] == ')' &&
					/*	(in_buff + tmp_ind)[5] == ',' && */
						(in_buff + tmp_ind)[6] == '-' &&
					/*	(in_buff + tmp_ind)[7] == '(' && */
						(in_buff + tmp_ind)[8] == 's' &&
					/*	(in_buff + tmp_ind)[9] == 'p' && */
						(in_buff + tmp_ind)[3] == regnum &&
						myatoi(numstr) - myatoi(numstr1) == 2 &&
						addinrange())
					{
						in_ind = nxt_line;
						COPYC();
						COPYC();
						COPYC();
						COPYC();
						ADDC('.');
						ADDC('l');
						++mov1_total;
					}
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move x+2(a6),-(sp)
 *     move x(a6),-(sp)
 * into
 *     move.l x,-(sp)
 */
static VOID mv2wtol(NOTHING)
{
	register long in_ind;
	register long out_ind;
	register long tmp_ind;
	
	char numstr[10];
	char numstr1[10];
	register char regnum;

	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'm')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ')
		{
			if ((tmp_ind = getnum(numstr, in_ind + 5)) >= 0 &&
				(in_buff + tmp_ind)[0] == '(' &&
				(in_buff + tmp_ind)[1] == 'R' &&
				(in_buff + tmp_ind)[2] == '1' &&
			/*	(in_buff + tmp_ind)[3] == '4' && */
				(in_buff + tmp_ind)[4] == ')' &&
			/*	(in_buff + tmp_ind)[5] == ',' && */
				(in_buff + tmp_ind)[6] == '-' &&
			/*	(in_buff + tmp_ind)[7] == '(' && */
				(in_buff + tmp_ind)[8] == 's' /* &&
				(in_buff + tmp_ind)[9] == 'p' && 
				(in_buff + tmp_ind)[10] == ')' */
				)
			{
				regnum = (in_buff + tmp_ind)[3];
				nxt_line = in_ind;
				nextline();
				if ((in_buff + nxt_line)[0] == 'm' &&
					(in_buff + nxt_line)[1] == 'o' &&
				/*	(in_buff + nxt_line)[2] == 'v' && */
				/*	(in_buff + nxt_line)[3] == 'e' && */
					(in_buff + nxt_line)[4] == ' ')
				{
					if ((tmp_ind = getnum(numstr1, nxt_line + 5)) >= 0 &&
						(in_buff + tmp_ind)[0] == '(' &&
						(in_buff + tmp_ind)[1] == 'R' &&
						(in_buff + tmp_ind)[2] == '1' &&
					/*	(in_buff + tmp_ind)[3] == '4' && */
						(in_buff + tmp_ind)[4] == ')' &&
					/*	(in_buff + tmp_ind)[5] == ',' && */
						(in_buff + tmp_ind)[6] == '-' &&
					/*	(in_buff + tmp_ind)[7] == '(' && */
						(in_buff + tmp_ind)[8] == 's' &&
					/*	(in_buff + tmp_ind)[9] == 'p' && */
						(in_buff + tmp_ind)[3] == regnum &&
						myatoi(numstr) - myatoi(numstr1) == 2)
					{
						in_ind = nxt_line;
						COPYC();
						COPYC();
						COPYC();
						COPYC();
						ADDC('.');
						ADDC('l');
						++mov_total;
					}
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move #x,-(sp)
 *     move #y,-(sp)
 * into
 *     move.l #yx,-(sp)
 */
static VOID imoveop(NOTHING)
{
	register long in_ind;
	register long out_ind;
	register long tmp_ind;
	
	char numstr[10];
	char numstr1[10];
	register int i;
	register int num;
	register int num1;
	char hexstr[9];
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'm')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ' &&
			(in_buff + in_ind)[5] == '#')
		{
			if ((tmp_ind = getnum(numstr, in_ind + 6)) >= 0 &&
				(in_buff + tmp_ind)[0] == ',' &&
				(in_buff + tmp_ind)[1] == '-' &&
				(in_buff + tmp_ind)[2] == '(' &&
				(in_buff + tmp_ind)[3] == 's' /*
				(in_buff + tmp_ind)[4] == 'p' &&
				(in_buff + tmp_ind)[5] == ')' && */
				)
			{
				nxt_line = in_ind;
				nextline();
				if ((in_buff + nxt_line)[0] == 'm' &&
					(in_buff + nxt_line)[1] == 'o' &&
				/*	(in_buff + nxt_line)[2] == 'v' && */
				/*	(in_buff + nxt_line)[3] == 'e' && */
					(in_buff + nxt_line)[4] == ' ' &&
					(in_buff + nxt_line)[5] == '#')
				{
					if ((tmp_ind = getnum(numstr1, nxt_line + 6)) >= 0 &&
						(in_buff + tmp_ind)[0] == ',' &&
						(in_buff + tmp_ind)[1] == '-' &&
						(in_buff + tmp_ind)[2] == '(' &&
						(in_buff + tmp_ind)[3] == 's' /* &&
						(in_buff + tmp_ind)[4] == 'p' &&
						(in_buff + tmp_ind)[5] == ')' && */
						)
					{
						num = myatoi(numstr);
						num1 = myatoi(numstr1);
						htoa(hexstr, num1);
						htoa(hexstr + 4, num);
						in_ind = nxt_line;
						COPYC();
						COPYC();
						COPYC();
						COPYC();
						ADDC('.');
						ADDC('l');
						COPYC();
						COPYC();
						ADDC('$');
						for (i = 0; i < 8; i++)
						{
							ADDC(*(i + hexstr));
						}
						while (in_ind < size && in_buff[in_ind] != ',')
						{
							++in_ind;
						}
						++imov_total;
					}
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move #x,(sp)
 *     move #y,-(sp)
 * into
 *     move.l #yx,-(sp)
 *     with stack correction for 2 bytes
 */
#if 0 /* unussed */
static VOID imov1_op(NOTHING)
{
	register long in_ind;
	register long out_ind;
	register long tmp_ind;
	
	char numstr[10];
	char numstr1[10];
	register int i;
	register int num;
	register int num1;
	char hexstr[9];
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'm')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ' &&
			(in_buff + in_ind)[5] == '#')
		{
			if ((tmp_ind = getnum(numstr, in_ind + 6)) >= 0 &&
				(in_buff + tmp_ind)[0] == ',' &&
				(in_buff + tmp_ind)[1] == '(' &&
				(in_buff + tmp_ind)[2] == 's' && /*
				(in_buff + tmp_ind)[3] == 'p' &&
				(in_buff + tmp_ind)[4] == ')' && */
				ISNL((in_buff + tmp_ind)[5])
				)
			{
				nxt_line = in_ind;
				nextline();
				if ((in_buff + nxt_line)[0] == 'm' &&
					(in_buff + nxt_line)[1] == 'o' &&
				/*	(in_buff + nxt_line)[2] == 'v' && */
				/*	(in_buff + nxt_line)[3] == 'e' && */
					(in_buff + nxt_line)[4] == ' ' &&
					(in_buff + nxt_line)[5] == '#')
				{
					if ((tmp_ind = getnum(numstr1, nxt_line + 6)) >= 0 &&
						(in_buff + tmp_ind)[0] == ',' &&
						(in_buff + tmp_ind)[1] == '-' &&
						(in_buff + tmp_ind)[2] == '(' &&
						(in_buff + tmp_ind)[3] == 's' && /*
						(in_buff + tmp_ind)[4] == 'p' &&
						(in_buff + tmp_ind)[5] == ')' && */
						addinrange()
						)
					{
						num = myatoi(numstr);
						num1 = myatoi(numstr1);
						htoa(hexstr, num1);
						htoa(hexstr + 4, num);
						in_ind = nxt_line;
						COPYC();
						COPYC();
						COPYC();
						COPYC();
						ADDC('.');
						ADDC('l');
						COPYC();
						COPYC();
						ADDC('$');
						for (i = 0; i < 8; i++)
						{
							ADDC(*(i + hexstr));
						}
						while (in_ind < size && in_buff[in_ind] != ',')
						{
							++in_ind;
						}
						++imov1_total;
					}
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}
#endif

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     clr Rx
 *     move y,Rx
 * into
 *     move y,Rx
 */
static VOID clrop(NOTHING)
{
	register long in_ind;
	register long out_ind;
	register long i;
	register char savereg;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_ind < size && in_buff[in_ind] != 'c')
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'c' &&
			(in_buff + in_ind)[1] == 'l' &&
			(in_buff + in_ind)[2] == 'r' &&
			(in_buff + in_ind)[3] == ' ' &&
			(in_buff + in_ind)[4] == 'R' &&
		 /*	(in_buff + in_ind)[5] == '0' && */
			ISNL((in_buff + in_ind)[6]))
		{
			savereg = (in_buff + in_ind)[5];
			nxt_line = in_ind;
			nextline();
			i = nxt_line;
			if ((in_buff + nxt_line)[0] == 'm' &&
				(in_buff + nxt_line)[1] == 'o' &&
				(in_buff + nxt_line)[2] == 'v' &&
				(in_buff + nxt_line)[3] == 'e' &&
				(in_buff + nxt_line)[4] == ' ')
			{
				while (i < size && in_buff[i++] != ',')
					;
				if (in_buff[i] == 'R' &&
					in_buff[i + 1] == savereg)
				{
					in_ind = nxt_line;
					++clr_total;
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move.l An,-(sp)
 *     add.l #x,(sp) or sub.l #x,(sp)
 * into
 *     pea x(an)
 */
static VOID mv2pea1(NOTHING)
{
	register long i;
	register long out_ind;
	register char *in;
	int reg;
	BOOLEAN add;
	long val;
	long tmp;
	long nxt;
	long start;
	
	i = out_ind = 0;
	in = in_buff;
	while (i < size)
	{
		if ((i + in)[0] == 'm' &&
			(i + in)[1] == 'o' &&
		/*	(i + in)[2] == 'v' && */
		/*	(i + in)[3] == 'e' && */
			(i + in)[4] == '.' &&
			(i + in)[5] == 'l' &&
		/*	(i + in)[6] == ' ' && */
			(i + in)[7] == 'R')
		{
			tmp = i + 8;
			if ((tmp + in)[0] == '1')
			{
				if ((tmp + in)[1] >= '0' &&
					(tmp + in)[1] <= '5')
				{
					reg = (in + tmp)[1] - '0' + 10;
					tmp += 2;
				} else
				{
					goto nextc;
				}
			} else if ((tmp + in)[0] > '7')
			{
				reg = (in + tmp)[0] - '0';
				tmp += 1;
			} else
			{
				goto nextc;
			}
			if ((tmp + in)[1] == '-' &&
				(tmp + in)[3] == 's')
			{
				nxt_line = i;
				nextline();
				if ((((in + nxt_line)[0] == 'a' &&
					  (nxt_line + in)[1] == 'd' &&
					  (nxt_line + in)[2] == 'd') ||
					 ((nxt_line + in)[0] == 's' &&
					  (nxt_line + in)[1] == 'u' &&
					  (nxt_line + in)[2] == 'b')) &&
					(nxt_line + in)[3] == '.' &&
					(nxt_line + in)[4] == 'l' &&
					(nxt_line + in)[5] == ' ' &&
					(nxt_line + in)[6] == '#' &&
					((nxt_line + in)[7] == '-' ||
					 ((nxt_line + in)[7] >= '0' && (nxt_line + in)[7] <= '9')))
				{
					add = (in + nxt_line)[0] != 'a' ? FALSE : TRUE;
					if ((nxt_line + in)[7] == '-')
					{
						nxt = nxt_line + 8;
						add = add ? FALSE : TRUE;
					} else
					{
						nxt = nxt_line + 7;
					}
					start = nxt;
					while ((nxt + in)[0] >= '0' && (nxt + in)[0] <= '9')
					{
						++nxt;
					}
					if ((nxt + in)[1] == '(' &&
						(nxt + in)[2] == 's')
					{
						val = 0;
						tmp = start;
						while (tmp < nxt)
						{
							val = MUL10(val);
							val += (tmp + in)[0] - '0';
							++tmp;
						}
						if (val <= 32767L)
						{
							ADDC('p');
							ADDC('e');
							ADDC('a');
							ADDC(' ');
							if (!add)
							{
								ADDC('-');
							}
							itoa((int)val, out_buff + out_ind);
							while ((out_ind + out_buff)[0] != 0)
								++out_ind;
							ADDC('(');
							ADDC('R');
							if (reg > 9)
							{
								ADDC('1');
							}
							ADDC((reg % 10) + '0');
							ADDC(')');
							nextline();
							i = nxt_line - 1; /* points to newline before next line */
							++mv1_total;
						}
					}
				}
			}
		}
	nextc:;
		*(out_ind + out_buff) = *(i + in);
		++i;
		++out_ind;
	}

	size = out_ind;
}

/* ------------------------------------------------------------------------- */

static BOOLEAN add4munch(NOTHING)
{
	long i;
	long colon;
	long nxt;
	int j;
	BOOLEAN ret;
	int val;
	char numstr[10];
	char numstr1[10];
	
	ret = FALSE;
	nxt = nxt_line;
	for (;;)
	{
		if (nxt_line == size) /* should be >= */
			goto fail;
		nextline();
		colon = 0;
		i = nxt_line;
		while (i < size && in_buff[i] != '\n')
		{
			if (in_buff[i] == ':')
				colon = i;
			++i;
		}
		if (colon != 0)
			i = colon + 1;
		else
			i = nxt_line;
		if (((in_buff + i)[0] == 'j' || (in_buff + i)[0] == 'b') &&
			(in_buff + i)[1] == 's' &&
			(in_buff + i)[2] == 'r')
			break;
	}
	nextline();
	i = nxt_line;
	while (i < size && in_buff[i] != '\n')
	{
		++i;
	}
	if ((i + in_buff)[-3] != ',' ||
		(i + in_buff)[-2] != 's' ||
		(i + in_buff)[-1] != 'p')
		goto fail;
	if ((in_buff + nxt_line)[0] == 'a' &&
		(in_buff + nxt_line)[1] == 'd' /* &&
		(in_buff + nxt_line)[2] == 'd' */
		)
	{
		if ((in_buff + nxt_line)[3] == 'q')
		{
			/* addq can only be 2,4,6,8 */
			if ((in_buff + nxt_line)[8] == '8' || (in_buff + nxt_line)[8] == '6')
			{
				/* change addq.l to add.w */
				(in_buff + nxt_line)[3] = '.';
				(in_buff + nxt_line)[4] = 'w';
				(in_buff + nxt_line)[5] = ' ';
				(in_buff + nxt_line)[6] = '#';
				(in_buff + nxt_line)[7] = '1';
				(in_buff + nxt_line)[8] -= 6;
			} else
			{
				(in_buff + nxt_line)[8] += 4;
			}
		} else
		{
			if ((in_buff + nxt_line)[3] == 'a')
			{
				i = nxt_line + 8;
			} else
			{
				i = nxt_line + 7;
			}
			val = myatoi(in_buff + i);
			itoa(val, numstr);
			itoa(val + 4, numstr1);
			if (strlen(numstr) != strlen(numstr1))
				goto fail;
			for (j = strlen(numstr1) - 1; j >= 0; --j)
				*(in_buff + (i + j)) = *(j + numstr1);
		}
		goto ok;
	} else
	{
		goto fail;
	}
ok:
	ret = TRUE;
fail:
	nxt_line = nxt;
	return ret;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     move.l An,(sp)
 *     add.l #x,(sp) or sub.l #x,(sp)
 * into
 *     pea x(an)
 *     (with stack correction)
 */
static VOID mv2pea2(NOTHING)
{
	register long i;
	register long out_ind;
	register char *in;
	int reg;
	BOOLEAN add;
	long val;
	long tmp;
	long nxt;
	long start;
	
	i = out_ind = 0;
	in = in_buff;
	while (i < size)
	{
		if ((i + in)[0] == 'm' &&
			(i + in)[1] == 'o' &&
		/*	(i + in)[2] == 'v' && */
		/*	(i + in)[3] == 'e' && */
			(i + in)[4] == '.' &&
			(i + in)[5] == 'l' &&
		/*	(i + in)[6] == ' ' && */
			(i + in)[7] == 'R')
		{
			tmp = i + 8;
			if ((tmp + in)[0] == '1')
			{
				if ((tmp + in)[1] >= '0' &&
					(tmp + in)[1] <= '5')
				{
					reg = (in + tmp)[1] - '0' + 10;
					tmp += 2;
				} else
				{
					goto nextc;
				}
			} else if ((tmp + in)[0] > '7')
			{
				reg = (in + tmp)[0] - '0';
				tmp += 1;
			} else
			{
				goto nextc;
			}
			if ((tmp + in)[1] == '(' &&
				(tmp + in)[2] == 's')
			{
				nxt_line = i;
				nextline();
				if ((((in + nxt_line)[0] == 'a' &&
					  (nxt_line + in)[1] == 'd' &&
					  (nxt_line + in)[2] == 'd') ||
					 ((nxt_line + in)[0] == 's' &&
					  (nxt_line + in)[1] == 'u' &&
					  (nxt_line + in)[2] == 'b')) &&
					(nxt_line + in)[3] == '.' &&
					(nxt_line + in)[4] == 'l' &&
					(nxt_line + in)[5] == ' ' &&
					(nxt_line + in)[6] == '#' &&
					((nxt_line + in)[7] == '-' ||
					 ((nxt_line + in)[7] >= '0' && (nxt_line + in)[7] <= '9')))
				{
					add = (in + nxt_line)[0] != 'a' ? FALSE : TRUE;
					if ((nxt_line + in)[7] == '-')
					{
						nxt = nxt_line + 8;
						add = add ? FALSE : TRUE;
					} else
					{
						nxt = nxt_line + 7;
					}
					start = nxt;
					while ((nxt + in)[0] >= '0' && (nxt + in)[0] <= '9')
					{
						++nxt;
					}
					if ((nxt + in)[1] == '(' &&
						(nxt + in)[2] == 's')
					{
						val = 0;
						tmp = start;
						while (tmp < nxt)
						{
							val = MUL10(val);
							val += (tmp + in)[0] - '0';
							++tmp;
						}
						if (val < 32768L && add4munch())
						{
							ADDC('p');
							ADDC('e');
							ADDC('a');
							ADDC(' ');
							if (!add)
							{
								ADDC('-');
							}
							itoa((int)val, out_buff + out_ind);
							while ((out_ind + out_buff)[0] != 0)
								++out_ind;
							ADDC('(');
							ADDC('R');
							if (reg > 9)
							{
								ADDC('1');
							}
							ADDC((reg % 10) + '0');
							ADDC(')');
							nextline();
							i = nxt_line - 1; /* points to newline before next line */
							++mv2_total;
						}
					}
				}
			}
		}
	nextc:;
		*(out_ind + out_buff) = *(i + in);
		++i;
		++out_ind;
	}

	size = out_ind;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     add.l #x,An
 * into
 *     add.w #x,An
 *     for x <= 32767
 */
static VOID adda(NOTHING)
{
	register long i;
	register long out_ind;
	register char *in;
	long val;
	long tmp;
	long start;
	
	i = out_ind = 0;
	in = in_buff;
	while (i < size)
	{
		if ((((i + in)[0] == 'a' &&
			  (i + in)[1] == 'd' &&
			  (i + in)[2] == 'd') ||
			 ((i + in)[0] == 's' &&
			  (i + in)[1] == 'u' &&
			  (i + in)[2] == 'b')))
		{
			tmp = i + 3;
			if ((tmp + in)[0] == 'a')
				++tmp;
			if ((tmp + in)[0] != ' ')
			{
				++tmp;
				if ((tmp + in)[0] == 'l')
				{
					start = tmp;
					tmp += 2;
					if ((tmp + in)[0] != '#' ||
						(tmp + in)[1] > '9' ||
						(tmp + in)[1] < '0')
					{
						goto nextc;
					}
					val = 0;
					while ((tmp + in)[0] >= '0' && (tmp + in)[0] <= '9')
					{
						val = MUL10(val);
						val += (tmp + in)[0] - '0';
						++tmp;
					}
					if (val <= 32767L)
					{
						if ((i + in)[3] != 'a')
						{
							if ((tmp + in)[1] != 'R')
								goto nextc;
							if ((tmp + in)[2] == '1' && (tmp + in)[3] < '0')
								goto nextc;
							if ((tmp + in)[3] > '5')
								goto nextc;
							if ((tmp + in)[2] < '8')
								goto nextc;
							if ((tmp + in)[2] > '9')
								goto nextc;
						}
						*(start + in) = 'w';
						++adda_total;
					}
				}
			}
		}
	nextc:;
		*(out_ind + out_buff) = *(i + in);
		++i;
		++out_ind;
	}

	size = out_ind;
}

/* ------------------------------------------------------------------------- */

/*
 * change 
 *     bra Lx
 *     bra Lx
 * into
 *     bra Lx
 */
static VOID samebra(NOTHING)
{
	register long in_ind;
	register long out_ind;
	long i;
	long nxt;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'b' && in_ind < size)
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'b' &&
			(in_buff + in_ind)[1] == 'r' &&
			(in_buff + in_ind)[2] == 'a' &&
			(in_buff + in_ind)[3] == ' ')
		{
			nxt_line = in_ind;
			nextline();
			if ((in_buff + nxt_line)[0] == 'b' &&
				(in_buff + nxt_line)[1] == 'r' &&
				(in_buff + nxt_line)[2] == 'a' &&
				(in_buff + nxt_line)[3] == ' ')
			{
				i = in_ind + 5;
				nxt = nxt_line + 5;
				while (i < size && (i + in_buff)[0] != '\n')
				{
					if ((i + in_buff)[0] != (nxt + in_buff)[0])
						break;
					i++;
					nxt++;
				}
				if (ISNL((i + in_buff)[0]))
				{
					++bra_total;
					in_ind = nxt_line;
				}
			}
		}
		COPYC();
	}

	size = out_ind - 1;
}

/*****************************************************************************/
/* ------------------------------------------------------------------------- */
/*****************************************************************************/

static VOID do_optimize(NOTHING)
{
	char *tmp;

#define SWAP() \
	tmp = out_buff; \
	out_buff = in_buff; \
	in_buff = tmp; \
	in_buff[size] = '\0'
	
	stripcr();
	SWAP();
	
	if (stp_lines)
	{
		striplines();
		SWAP();
	}
	
	if (stp_symbols)
	{
		stripsyms();
		SWAP();
	}
	
	clrwtol();
	SWAP();

	clr2wtol();
	SWAP();

	mvwtol();
	SWAP();

	mv2wtol();
	SWAP();

	imoveop();
	SWAP();

	clrop();
	SWAP();

	mv2pea1();
	SWAP();

	mv2pea2();
	SWAP();

	adda();
	SWAP();

	samebra();

#undef SWAP
}

/* ------------------------------------------------------------------------- */

static long filesize(P(int) fd)
PP(register int fd;)
{
	long fsize;
	
	if (fd < 0)
		return 0;
#ifdef __ALCYON__
	fsize = Fseek(0L, fd, SEEK_END);
	Fseek(0L, fd, SEEK_SET);
#else
	fsize = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
#endif
	return fsize;
}

/* ------------------------------------------------------------------------- */

static BOOLEAN open_file(P(const char *) name)
PP(const char *name;)
{
	register int in_hndl;
	register long got;
	
#ifdef __ALCYON__
	char buf[MAXPATH];
	/*
	 * have to use TOS-functions here, since read() only takes unsigned int
	 * as parameter
	 */
	strcpy(buf, name);
	_dosify(buf);
	in_hndl = Fopen(buf, O_RDONLY);
#else
	in_hndl = open(name, O_RDONLY | O_BINARY);
#endif

	in_buff = NULL;
	out_buff = NULL;

	size = filesize(in_hndl);
	if (size < 0 || in_hndl < 0)
	{
		fprintf(stderr, _("Error opening %s for reading -Abort.\n"), name);
		return FALSE;
	}
	in_buff = lmalloc(size + 2);
	out_buff = lmalloc(size + 2);
	if (in_buff == NULL || out_buff == NULL)
	{
		fprintf(stderr, _("Not enough memory for operation on %s -Abort\n"), name);
		return FALSE;
	}
#ifdef __ALCYON__
	got = Fread(in_hndl, size, in_buff);
#else
	got = read(in_hndl, in_buff, size);
#endif
	if (got != size)
	{
		fprintf(stderr, _("Error reading %s (got %ld, expected %ld). -Abort\n"), name, got, size);
		return FALSE;
	}
	in_buff[size] = '\0';
#ifdef __ALCYON__
	Fclose(in_hndl);
#else
	close(in_hndl);
#endif
	return TRUE;
}

/* ------------------------------------------------------------------------- */

static BOOLEAN close_file(P(const char *) name)
PP(const char *name;)
{
	char buf[MAXPATH];
	register int out_hndl;
	register long got;
	
	if (out_buff == NULL)
		return FALSE;
	strcpy(buf, opath);
	strcat(buf, name);
#ifdef __ALCYON__
	_dosify(buf);
	out_hndl = Fcreate(buf, 0);
#else
	out_hndl = open(buf, O_WRONLY|O_CREAT|O_TRUNC, 0644);
#endif
	if (out_hndl < 0)
	{
		fprintf(stderr, _("Error creating %s for output -Abort.\n"), buf);
		return FALSE;
	}
#ifdef __ALCYON__
	got = Fwrite(out_hndl, size, out_buff);
#else
	got = write(out_hndl, out_buff, size);
#endif
	if (got != size)
	{
		fprintf(stderr, _("Error writing %s%s (got %ld, expected %ld). -Abort.\n"), opath, name, got, size);
		return FALSE;
	}
#ifdef __ALCYON__
	Fclose(out_hndl);
#else
	close(out_hndl);
#endif
	return TRUE;
}

/* ------------------------------------------------------------------------- */

static BOOLEAN init_optimize(NOTHING)
{
	mov_total = imov_total = mov1_total = clr1_total = imov1_total = clr_total = bra_total = 0;
	return TRUE;
}

/* ------------------------------------------------------------------------- */

static VOID __NORETURN Error(P(const char *)str)
PP(const char *str;)
{
	fprintf(stderr, "\n%s\n", str);
	exit(EXIT_FAILURE);
}

/* ------------------------------------------------------------------------- */

static VOID __NORETURN usage(NOTHING)
{
	Error(_("USAGE: optimize.prg [-l] [-s] [-q] [ -oPATH ] files ..."));
}

/* ------------------------------------------------------------------------- */

int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(register char **argv;)
{
	register int argi;
	
#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	argi = 1;
	while (argi < argc && argv[argi][0] == '-')
	{
		char c;
		
		c = argv[argi][1];
		if (c == 'o' || c == 'O')
		{
			int l;
			
			if (argv[argi][2] == '\0')
			{
				if ((argi + 2) >= argc)
					usage();
				strcpy(opath, argv[argi + 1]);
				argi += 2;
			} else
			{
				strcpy(opath, &argv[argi][2]);
				argi++;
			}
			l = strlen(opath);
			if (l != 0 && opath[l - 1] != '/' && opath[l - 1] != '\\')
				strcat(opath, "/");
		} else if (c == 'l' || c == 'L')
		{
			stp_lines = TRUE;
			argi++;
		} else if (c == 's' || c == 'S')
		{
			stp_symbols = TRUE;
			argi++;
		} else if (c == 'q' || c == 'Q')
		{
			quiet = TRUE;
			argi++;
		} else
		{
			usage();
		}
	}
	if (argi >= argc)
		usage();
	
	if (init_optimize() == FALSE)
	{
		Error(_("Initialization Failure !!!"));
	}
	
	argv += argi;
	argc -= argi;
	argi = 0;
	while (argi < argc)
	{
		if (open_file(argv[argi]))
		{
			if (!quiet)
				printf(" %d.    %s.\n", argi, argv[argi]);
			do_optimize();
			close_file(argv[argi]);
		}
		lfree(in_buff);
		lfree(out_buff);
		argi++;
	}
	
	if (!quiet)
	{
		printf(_("%6d Total clr's removed\n"), clr_total);
		printf(_("%6d Total clr's (w/stk mod) removed\n"), clr1_total);
		printf(_("%6d Total moves's removed\n"), mov_total);
		printf(_("%6d Total moves (w/stk mod) removed\n"), mov1_total);
		printf(_("%6d Total move immediates removed\n"), imov_total);
		printf(_("%6d Total move immed.(w/stk mod) removed\n"), imov1_total);
		printf(_("%6d Total redundant bra's removed\n"), bra_total);
		printf(_("%6d Total pea's used (4 bytes)\n"), mv1_total);
		printf(_("%6d Total pea's used (w/stk mod) (2 or 4 bytes)\n"), mv2_total);
		printf(_("%6d Total adda.l's improved (2 bytes)\n"), adda_total);
	}
	
	return EXIT_SUCCESS;
}
