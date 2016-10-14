#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

typedef struct
{
    char      d_reserved[21];  /* reserved for GEMDOS   */
    char      d_attrib;        /* file attribut         */
    short     d_time;          /* packed time           */
    short     d_date;          /* packed date           */
    long      d_length;        /* filesize              */
    char      d_fname[12];     /* file name             */ /* BUG: no space for terminating 0 */
} DTA;

#include <osbind.h>

#define BOOLEAN int
#define FALSE 0
#define TRUE 1

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#define _(x) x

#define MAXPATH 256

/*
 * these 2 defines are somehow needed to give the external
 * symbols a different name, so that the linker assigns
 * the common symbols in the correct order
 */
#define in_hndl numstr4
#define bra_total tmp_ine


char numstr[10];
int clr1_total;
long size;
int imov1_total;
int mov1_total;
int mv1_total;
int mv2_total;
int adda_total;
DTA dta_buf;
char *in_buff;
char numstr1[10];
char numstr2[10];
char numstr3[10];
int in_hndl;
char *freemem;
long tmp_ind;
int bra_total;
char savereg;
int out_hndl;
long out_ind;
char *out_buff;
int clr_total;
long memsize;
long nxt_line;
int imov_total;
int mov_total;
char opath[MAXPATH];
int num;
int xyzzx;
char *begmem;
long in_ind;
char *endmem;
int num1;
int num2;
char regnum;
char hexstr[9];


long filesize PROTO((const char *name));
BOOLEAN init_optimize PROTO((NOTHING));
BOOLEAN open_file PROTO((const char *name));
BOOLEAN close_file PROTO((const char *name));
VOID Error PROTO((const char *str));
VOID usage PROTO((NOTHING));
VOID do_optimize PROTO((NOTHING));
VOID clrwtol PROTO((NOTHING));
VOID clr2wtol PROTO((NOTHING));
VOID mvwtol PROTO((NOTHING));
VOID mv2wtol PROTO((NOTHING));
VOID imoveop PROTO((NOTHING));
VOID imov1_op PROTO((NOTHING));
VOID clrop PROTO((NOTHING));
VOID mv2pea1 PROTO((NOTHING));
VOID mv2pea2 PROTO((NOTHING));
VOID adda PROTO((NOTHING));
VOID samebra PROTO((NOTHING));
VOID reverse PROTO((char *str));
VOID htoa PROTO((char *str, int num));
VOID itoa PROTO((int num, char *str));
int atoi PROTO((const char *str));
BOOLEAN getnum PROTO((char *str));
VOID puthex PROTO((int i, char *str));
VOID nextline PROTO((NOTHING));
VOID panic PROTO((const char *str));
BOOLEAN add4munch PROTO((NOTHING));
BOOLEAN addinrange PROTO((NOTHING));


long filesize(P(const char *) name)
PP(register const char *name;)
{
	int ret;
	DTA *old_dta;
	
	old_dta = (DTA *)Fgetdta();
	Fsetdta(&dta_buf);
	ret = Fsfirst(name, 0);
	Fsetdta(old_dta);
	if (ret == 0)
	{
		return dta_buf.d_length;
	} else
	{
		return 0;
	}
}


BOOLEAN init_optimize(NOTHING)
{
	memsize = Malloc(-1L);
	if (memsize == 0)
		return FALSE;
	begmem = (char *)Malloc(memsize);
	if (begmem == NULL)
		return FALSE;
	endmem = begmem + memsize;
	freemem = begmem + 64000L;
	mov_total = imov_total = mov1_total = clr1_total = imov1_total = clr_total = bra_total = 0;
#ifdef __ALCYON__
	asm("dc.w $303c,$0001")
	return;
#else
	return TRUE;
#endif
}


BOOLEAN open_file(P(const char *) name)
PP(const char *name;)
{
	size = filesize(name);
	in_hndl = Fopen(name, 0);
	if (size == 0 || in_hndl < 0)
	{
		printf(_("Error opening %s for reading -Abort.\r\n"), name);
		return FALSE;
	}
	if ((size * 2 + 16) > ((intptr_t)endmem - (intptr_t)freemem))
	{
		printf(_("Not enough memory for operation on %s"), name);
		printf(_(" -Abort\r\n"));
		return FALSE;
	}
	if (Fread(in_hndl, size, in_buff) != size)
	{
		printf(_("Error reading %s. -Abort\r\n"), name);
		return FALSE;
	}
	Fclose(in_hndl);
#ifdef __ALCYON__
	asm("dc.w $303c,$0001")
	return;
#else
	return TRUE;
#endif
}


BOOLEAN close_file(P(const char *) name)
PP(const char *name;)
{
	char buf[MAXPATH];
	
	strcpy(buf, opath);
	strcat(buf, name);
	if ((out_hndl = Fcreate(buf, 0)) < 0)
	{
		printf(_("Error creating %s"), buf);
		printf(_(" for output -Abort.\r\n"));
		return FALSE;
	}
	if (Fwrite(out_hndl, out_ind, out_buff) != out_ind)
	{
		printf(_("Error writing %s%s"), opath, name);
		printf(_(". -Abort."));
		return FALSE;
	}
	Fclose(out_hndl);
#ifdef __ALCYON__
	asm("dc.w $303c,$0001")
	return;
#else
	return TRUE;
#endif
}


VOID __NORETURN Error(P(const char *)str)
PP(const char *str;)
{
	fprintf(stderr, "\r\n%s\r\n", str);
	exit(-1);
}



VOID __NORETURN usage(NOTHING)
{
	Error(_("USAGE: optimize.prg [ -oPATH ] files ..."));
}


int main(P(int) argc, P(char **) argv)
PP(int argc;)
PP(char **argv;)
{
	int argi;
	
#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
	asm(" xref _trap");
	asm(" xdef _trap");
	asm("___BDOS equ _trap");
#endif
	if (argc < 2)
		usage();
	if (argv[1][0] == '-')
	{
		if (argv[1][1] == 'o' || argv[1][1] == 'O')
		{
			if (argv[1][2] == '\0')
			{
				if (argc < 3)
					usage();
				strcpy(opath, argv[2]);
				argi = 3;
			} else
			{
				strcpy(opath, &argv[1][2]);
				if (argc == 2)
					usage();
				argi = 2;
			}
		} else
		{
			usage();
		}
	} else
	{
		argi = 1;
	}
	
	if (init_optimize() == FALSE)
	{
		Error(_("Initialization Failure !!!"));
	}
	while (argi < argc)
	{
		in_buff = freemem;
		if (open_file(argv[argi]))
		{
			printf(" %d.    %s.\r\n", argi, argv[argi]);
			out_buff = (char *)(((long)in_buff + size + 2) & ~1);
			do_optimize();
			close_file(argv[argi]);
#ifdef __ALCYON__
			asm ("dc.w $4a40,$6700,2"); /* tst.w d0; beq *+4; could be optimized out */
#endif
		}
		argi++;
	}
	
	printf(_("%6d Total clr's removed\r\n"), clr_total);
	printf(_("%6d Total clr's (w/stk mod) removed\r\n"), clr1_total);
	printf(_("%6d Total moves's removed\r\n"), mov_total);
	printf(_("%6d Total moves (w/stk mod) removed\r\n"), mov1_total);
	printf(_("%6d Total move immeditates removed\r\n"), imov_total);
	printf(_("%6d Total move immed.(w/stk mod) removed\r\n"), imov1_total);
	printf(_("%6d Total redundant bra's removed\r\n"), bra_total);
	printf(_("%6d Total pea's used (4 bytes)\r\n"), mv1_total);
	printf(_("%6d Total pea's used (w/stk mod) (2 or 4 bytes)\r\n"), mv2_total);
	printf(_("%6d Total adda.l's improved (2 bytes)\r\n\0"), adda_total);
	
	exit(EXIT_SUCCESS);
#ifndef __ALCYON__
	return EXIT_SUCCESS;
#endif
}


VOID do_optimize(NOTHING)
{
	char *tmp;

#define SWAP() \
	tmp = out_buff; \
	out_buff = in_buff; \
	in_buff = tmp
	
	clrwtol();
	SWAP();
	size = out_ind - 1;

	clr2wtol();
	SWAP();
	size = out_ind - 1;

	mvwtol();
	SWAP();
	size = out_ind - 1;

	mv2wtol();
	SWAP();
	size = out_ind - 1;

	imoveop();
	SWAP();
	size = out_ind - 1;

	clrop();
	SWAP();
	size = out_ind - 1;

	mv2pea1();
	SWAP();
	size = out_ind;

	mv2pea2();
	SWAP();
	size = out_ind;

	adda();
	SWAP();
	size = out_ind;

	samebra();
	out_ind -= 1;

#undef SWAP
}


VOID reverse(P(char *) str)
PP(char *str;)
{
	int c;
	int i;
	int len;
	
	for (i = 0, len = strlen(str) - 1; i < len; i++, len--)
	{
		c = *(str + (long)i); /* todo */
		*(i + str) = *(len + str);
		*(len + str) = c;
	}
}


size_t strlen(P(const char *)str)
PP(const char *str;)
{
	const char *p;

	for (p = str; *p; p++)				/* advance *p until NULL.   */
		;
	return (long) p - (long)str;
}


VOID htoa(P(char *)str, P(int) val)
PP(char *str;)
PP(int val;)
{
	int i;
	
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


VOID itoa(P(int) val, P(char *)str)
PP(int val;)
PP(char *str;)
{
	int i;
	int x;
	
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


int atoi(P(const char *)str)
PP(const char *str;)
{
	int i;
	int sign;
	long val;
	
	i = 0;
	sign = 1;
	if (*(i + str) == '-')
	{
		sign = -1;
		++i;
	}
	val = 0;
	while (*(i + str) >= '0' && *(i + str) <= '9')
	{
		val = val * 10L + *(i + str) - '0';
		++i;
		if (val > 32767L)
			panic(_("atoi overflow"));
	}
	return sign * val;
}


BOOLEAN getnum(P(char *)str)
PP(char *str;)
{
	int i;
	
	i = 0;
	if (in_buff[tmp_ind] == '(')
	{
		*(i + str) = '0';
		++i;
		*(i + str) = '\0';
		++i;
		return TRUE;
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
		return TRUE;
	} else
	{
		return FALSE;
	}
}


#define COPYC() \
	*(out_ind + out_buff) = *(in_ind + in_buff); \
	++in_ind; \
	++out_ind

#define ADDC(c) \
	*(out_ind + out_buff) = c; \
	++out_ind

/*
 * change 
 *     clr -(sp)
 *     clr -(sp)
 * into
 *     clr.l -(sp)
 */
VOID clr2wtol(NOTHING)
{
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'c' && in_ind < size)
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
}


/*
 * change 
 *     move x+2(a6),-(sp)
 *     move x(a6),-(sp)
 * into
 *     move.l x,-(sp)
 */
VOID mv2wtol(NOTHING)
{
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'm' && in_ind < size)
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ')
		{
			tmp_ind = in_ind + 5;
			if (getnum(numstr) &&
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
					tmp_ind = nxt_line + 5;
					if (getnum(numstr1) &&
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
						atoi(numstr) - atoi(numstr1) == 2)
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
}


/*
 * change 
 *     move x+2(a6),(sp)
 *     move x(a6),-(sp)
 * into
 *     move.l x,-(sp)
 *     with stack correction for 2 bytes
 */
VOID mvwtol(NOTHING)
{
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'm' && in_ind < size)
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'm' &&
			(in_buff + in_ind)[1] == 'o' &&
		/*	(in_buff + in_ind)[2] == 'v' && */
		/*	(in_buff + in_ind)[3] == 'e' && */
			(in_buff + in_ind)[4] == ' ')
		{
			tmp_ind = in_ind + 5;
			if (getnum(numstr) &&
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
				(in_buff + tmp_ind)[10] == '\r') /* BUG: should check '\n', too */
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
					tmp_ind = nxt_line + 5;
					if (getnum(numstr1) &&
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
						atoi(numstr) - atoi(numstr1) == 2 &&
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
}


/*
 * change 
 *     clr (sp)
 *     clr -(sp)
 * into
 *     clr.l -(sp)
 *     with stack correction for 2 bytes
 */
VOID clrwtol(NOTHING)
{
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'c' && in_ind < size)
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
			(in_buff + in_ind)[8] == '\r') /* BUG: should check '\n', too */
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
}


/*
 * change 
 *     move #x,-(sp)
 *     move #y,-(sp)
 * into
 *     move.l #yx,-(sp)
 */
VOID imoveop(NOTHING)
{
	int i;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'm' && in_ind < size)
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
			tmp_ind = in_ind + 6;
			if (getnum(numstr) &&
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
					tmp_ind = nxt_line + 6;
					if (getnum(numstr1) &&
						(in_buff + tmp_ind)[0] == ',' &&
						(in_buff + tmp_ind)[1] == '-' &&
						(in_buff + tmp_ind)[2] == '(' &&
						(in_buff + tmp_ind)[3] == 's' /* &&
						(in_buff + tmp_ind)[4] == 'p' &&
						(in_buff + tmp_ind)[5] == ')' && */
						)
					{
						num = atoi(numstr);
						num1 = atoi(numstr1);
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
						while ((in_buff + in_ind)[0] != ',')
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
}


/*
 * change 
 *     move #x,(sp)
 *     move #y,-(sp)
 * into
 *     move.l #yx,-(sp)
 *     with stack correction for 2 bytes
 */
VOID imov1_op(NOTHING)
{
	int i;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'm' && in_ind < size)
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
			tmp_ind = in_ind + 6;
			if (getnum(numstr) &&
				(in_buff + tmp_ind)[0] == ',' &&
				(in_buff + tmp_ind)[1] == '(' &&
				(in_buff + tmp_ind)[2] == 's' && /*
				(in_buff + tmp_ind)[3] == 'p' &&
				(in_buff + tmp_ind)[4] == ')' && */
				(in_buff + tmp_ind)[5] == '\r' /* BUG: should check '\n', too */
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
					tmp_ind = nxt_line + 6;
					if (getnum(numstr1) &&
						(in_buff + tmp_ind)[0] == ',' &&
						(in_buff + tmp_ind)[1] == '-' &&
						(in_buff + tmp_ind)[2] == '(' &&
						(in_buff + tmp_ind)[3] == 's' && /*
						(in_buff + tmp_ind)[4] == 'p' &&
						(in_buff + tmp_ind)[5] == ')' && */
						addinrange()
						)
					{
						num = atoi(numstr);
						num1 = atoi(numstr1);
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
						while ((in_buff + in_ind)[0] != ',')
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
}


VOID puthex(P(int) i, P(char *) str)
PP(int i;)
PP(char *str;)
{
	if (i < 10)
	{
		*str = i + '0';
	} else
	{
		i -= 10;
		*str = i + 'a';
	}
}


VOID nextline(NOTHING)
{
	while (in_buff[nxt_line] != '\n' && nxt_line < size)
	{
		++nxt_line;
	}
	if (in_buff[nxt_line] == '\n')
		++nxt_line;
	if (in_buff[nxt_line] == '*')
		nextline();
	if (in_buff[nxt_line] == ';')
		nextline();
}


/*
 * change 
 *     clr Rx
 *     move y,Rx
 * into
 *     move y,Rx
 */
VOID clrop(NOTHING)
{
	int i;
	
	in_ind = 0;
	out_ind = 0;
	while (in_ind < size)
	{
		while (in_buff[in_ind] != 'c' && in_ind < size)
		{
			COPYC();
		}
		if ((in_buff + in_ind)[0] == 'c' &&
			(in_buff + in_ind)[1] == 'l' &&
			(in_buff + in_ind)[2] == 'r' &&
			(in_buff + in_ind)[3] == ' ' &&
			(in_buff + in_ind)[4] == 'R' &&
		/*	(in_buff + in_ind)[5] == '0' && */
		/*	(in_buff + in_ind)[6] == '\r' && */
			(in_buff + in_ind)[7] == '\n')
		{
			savereg = (in_buff + in_ind)[5];
			i = 0;
			nxt_line = in_ind;
			nextline();
			if ((in_buff + nxt_line)[0] == 'm' &&
				(in_buff + nxt_line)[1] == 'o' &&
				(in_buff + nxt_line)[2] == 'v' &&
				(in_buff + nxt_line)[3] == 'e' &&
				(in_buff + nxt_line)[4] == ' ')
			{
				while ((in_buff + nxt_line)[i++] != ',')
					;
				if ((i + nxt_line + in_buff)[0] == 'R' &&
					(i + nxt_line + in_buff)[1] == savereg)
				{
					in_ind = nxt_line;
					++clr_total;
				}
			}
		}
		COPYC();
	}
}


/*
 * change 
 *     bra Lx
 *     bra Lx
 * into
 *     bra Lx
 */
VOID samebra(NOTHING)
{
	int i;
	int nxt;
	
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
				i = in_ind + 5; /* BUG: truncation from long to int */
				nxt = nxt_line + 5; /* BUG: truncation from long to int */
				while ((i + in_buff)[0] != '\n')
				{
					if ((i + in_buff)[0] != (nxt + in_buff)[0])
						break;
					i++;
					nxt++;
				}
				if ((i + in_buff)[0] == '\n')
				{
					++bra_total;
					in_ind = nxt_line;
				}
			}
		}
		COPYC();
	}
}


/*
 * change 
 *     move.l An,-(sp)
 *     add.l #x,(sp) or sub.l #x,(sp)
 * into
 *     pea x(an)
 */
VOID mv2pea1(NOTHING)
{
	register long i;
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
							val = val * 10;
							val += (tmp + in)[0] - '0';
							++tmp;
						}
						if (val <= 32767L) /* BUG: should also check for >= -32768 */
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
							i = nxt_line - 2; /* assumes CR/LF */
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
}


/*
 * change 
 *     move.l An,(sp)
 *     add.l #x,(sp) or sub.l #x,(sp)
 * into
 *     pea x(an)
 *     (with stack correction)
 */
VOID mv2pea2(NOTHING)
{
	register long i;
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
							val = val * 10;
							val += (tmp + in)[0] - '0';
							++tmp;
						}
						if (val < 32768L && add4munch()) /* BUG: should also check for >= -32768 */
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
							i = nxt_line - 2; /* assumes CR/LF */
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
}


/*
 * change 
 *     add.l #x,An
 * into
 *     add.w #x,An
 *     for x <= 32767
 */
VOID adda(NOTHING)
{
	register long i;
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
						val = val * 10;
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
}


VOID panic(P(const char *)str)
PP(const char *str;)
{
	printf(_("PANIC: %s"), str);
	printf(_("\r\nHIT ^C TO QUIT: "));
	while ((Cconin() & 0xff) != 3)
		;
	Pterm(-1);
}


BOOLEAN add4munch(NOTHING)
{
	long i;
	long colon;
	long nxt;
	int j;
	int unused;
	BOOLEAN ret;
	int val;
	
	UNUSED(unused);
	ret = FALSE;
	nxt = nxt_line;
	while (1)
	{
		if (nxt_line == size) /* should be >= */
			goto fail;
		nextline();
		colon = 0;
		i = nxt_line;
		while (in_buff[i] != '\n' /* && i < size */)
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
	while (in_buff[i] != '\n' /* && i < size */)
	{
		++i;
	}
	if ((i + in_buff)[-1] == '\r')
		--i;
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
			val = atoi(in_buff + i);
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


BOOLEAN addinrange(NOTHING)
{
	long i;
	long colon;
	long nxt;
	int j;
	int unused;
	BOOLEAN ret;
	int val;
	
	UNUSED(unused);
	ret = FALSE;
	nxt = nxt_line;
	while (1)
	{
		if (nxt_line == size) /* should be >= */
			goto fail;
		nextline();
		colon = 0;
		i = nxt_line;
		while (in_buff[i] != '\n' /* && i < size */)
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
	i = nxt_line;
	while (in_buff[i] != '\n' /* && i < size */)
	{
		++i;
	}
	if ((i + in_buff)[-1] == '\r')
		--i;
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
			val = atoi(in_buff + i);
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
