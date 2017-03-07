/*
 *	This file defines procedure names used by the assembler, compiler,
 *	etc. to be the names used in this library.  
 *
 *	(In case you haven't guessed, the "k" in "klib" stands for:
 *
 *			K L U D G E !)
 */

#define KLUDGE 1

#if KLUDGE

#define	kfopen(n, b)	xfopen(n, b)
#define	kfcreat(n, b)	xfcreat(n, b)
#define	kgetc(f)	xgetc(f)
#define kputc(c, f)	xputc(c, f)
#define kflush(f) xfflush(f)
#define kfclose(f)	xfclose(f)

#define BLEN 512

struct kludge_iob
{
	int _fd;							/* file descriptor */
	int cc;								/* char count */
	char *cp;							/* ptr to next char */
	char cbuf[BLEN];					/* char buffer */
};

#undef FILE
#define FILE struct kludge_iob

/*
 *	Defining the names in this manner allows using the V7 UNIX library
 *	to run the asinine V6 calls in the Alcyon software.
 *
 */

FILE *xfopen PROTO((const char *fname, FILE *ibuf));
FILE *xfcreat PROTO((const char *fname, FILE *ibuf));
int xfflush PROTO((FILE *ibuf));
int xputc PROTO((char c, register FILE *f));
int xgetc PROTO((register FILE *f));
int xfclose PROTO((register FILE *f));

#else

#define	kfopen(n, b)	fopen(n, "r")
#define	kfcreat(n, b)	fopen(n, "w")
#define	kgetc(f)	getc(f)
#define kputc(c, f)	putc(c, f)
#define kflush(f) fflush(f)
#define kfclose(f) fclose(f)

#endif
