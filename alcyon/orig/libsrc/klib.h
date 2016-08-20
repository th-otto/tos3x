/*
 *	This file defines procedure names used by the assembler, compiler,
 *	etc. to be the names used in this library.  
 *
 *	(In case you haven't guessed, the "k" in "klib" stands for:
 *
 *			K L U D G E !)
 */

#undef getc
#undef putc
#undef putchar
#undef getchar

#define	creat	xcreat
#define	open	xopen
#define	fopen(n,f,m)	xfopen(n, (FILE *)(f), m)
#define	getc(f)	xgetc((FILE *)(f))
#define	getw(f)	xgetw((FILE *)(f))
#define	fcreat	xfcreat
#define	putw(w)	xputw(w, (FILE *)(f))
#define putc(c, f)	xputc(c, (FILE *)(f))
#ifdef __ALCYON__
#define putchar(c) fputc(c, (FILE *)stdout) /* BUG: fputc uses struct _iobuf, not struct iob */
#else
#define putchar(c) fputc(c, stdout) /* BUG: fputc uses struct _iobuf, not struct iob */
#endif
#define getchar() xgetchar()
#define	strcmp	xstrcmp
#define	index	xindex
#define	strcpy	xstrcpy
#define dup     xdup
#define fflush(f)  xfflush((FILE *)(f))
#define seek    xseek

#define BLEN 512

struct iob
{
	int _fd;							/* file descriptor */
	int cc;								/* char count */
	char *cp;							/* ptr to next char */
	char cbuf[BLEN];					/* char buffer */
};

#undef FILE
#define FILE struct iob

/*
 *	Defining the names in this manner allows using the V7 UNIX library
 *	to run the asinine V6 calls in the Alcyon software.
 *
 */

int xopen PROTO((const char *fname, int mode, int binary));
int xfopen PROTO((const char *fname, FILE *ibuf, int binary));
int xcreat PROTO((const char *fname, mode_t prot, int binary));
int xgetc PROTO((FILE *ibuf));
int xgetw PROTO((FILE *ibuf));
VOID cputc PROTO((char c, int fn));
int xdup PROTO((int n));
int xgetchar PROTO((NOTHING));
int xfcreat PROTO((const char *fname, FILE *ibuf, int binary));
int xputc PROTO((char c, FILE *ibuf));
int xputw PROTO((int w, register FILE *ibuf));
int xfflush PROTO((FILE *ibuf));
int xfoflush PROTO((NOTHING));
int xputchar PROTO((char cc));
int xseek PROTO((int fildes, int offset, int whence));
int xstrcmp PROTO((const char *s1, const char *s2));

VOID xwritefail PROTO((NOTHING)) __attribute__((noreturn));
