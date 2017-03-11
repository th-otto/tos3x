/* always defined when compiling the library */
#define _LIBC 1

#include <stdarg.h>
#include <sys/types.h>
#ifndef NO_STDIO
#include <stdio.h>
#endif
#include <stddef.h>

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

#ifndef NO_CONST
#  ifdef __GNUC__
#	 define NO_CONST(p) __extension__({ union { const void *cs; void *s; } x; x.cs = p; x.s; })
#  else
#    ifdef __ALCYON__ /* Alcyon parses the cast as function call??? */
#      define NO_CONST(p) p
#    else
#      define NO_CONST(p) ((void *)(p))
#    endif
#  endif
#endif

#ifndef __attribute__
#  ifndef __GNUC__
#    define __attribute__(x)
#  endif
#endif

#define	MAXCCBS 16					/* Maximum Num CCBs     */

/* file descriptors */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

#ifndef NO_STDIO
extern char __pname[];					/* Program name         */
extern char *_break;					/* -> Program break location */
#endif

char *petoa PROTO((double fp, char *buf, int prec));
char *pftoa PROTO((double fp, char *buf, int prec));
char *pgtoa PROTO((double fp, char *buf, int prec));

typedef char *(*printfunc) PROTO((long n, char **pbuf, int base, int issigned, char *digs));

char *__prtshort PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtld PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtint PROTO((long n, char *pbuf, int base, int issigned, printfunc f, int upper));

int __doprint PROTO((FILE *stream, const char *fmt, int mode, va_list args));
int _doscan PROTO((FILE *stream, const char *fmt, va_list args));

long fptoffp PROTO((double f));
double ffptof PROTO((long lf));
long fpftol PROTO((long f));
long fpltof PROTO((long l));
int _doprt PROTO((FILE *sp, const char * fmt, char *pb));

long ldiv PROTO((long al1, long al2));
long lrem PROTO((long al1, long al2));

int __getmode PROTO((const char *mode));

#define __set_errno(e) (errno = (e))
#define __validfp(p) ((p) != NULL && (p) >= &_iob[0] && (p) < &_iob[MAXFILES])

union ll {
	struct
	{
		unsigned short hiword;
		unsigned short loword;
	} w;
	long l;
};

long lmul PROTO((long l1, long l2));
VOID almul PROTO((register long * l1, register long l2));
long uldiv PROTO((long al1, long al2));
VOID aldiv PROTO((long * al1, long al2));
VOID alrem PROTO((long *al1, long al2));

int _open PROTO((const char *name, int mode, int binary));
int __open PROTO((int fd, const char *name, int search));
int _chkuser PROTO((int newu));
int _uchkuser PROTO((int newu, int prevu));
VOID ucase PROTO ((char *str));

VOID _optoff PROTO((const char * msg)) __attribute__((noreturn));

VOID _chinit PROTO((NOTHING));
VOID __chinit PROTO((int fd));
int _allocc PROTO((NOTHING));						/* gets a channel       */
int _freec PROTO((int fd));

int _main PROTO((char *com, int len));
int __main PROTO((char *com, int len));
VOID _cleanup PROTO((NOTHING));

int _filbuf PROTO((FILE *));
long _filesz PROTO((int fd));						/* computes CP/M file size  */
char *_salloc PROTO((size_t size));					/* Stack allocation routine */
int _flsbuf PROTO((int c, FILE *fp));
char *_dosify PROTO((char *s));

VOID blkmove PROTO((char *to, const char *from, size_t nn));

#ifdef __OSIF_H__
FD *_fd_get PROTO((int flags));
char *_parsefn PROTO((const char *filename, struct fcbtab *fdb));
FD *_chkc PROTO((unsigned int fd));						/* Converts fd to fp */
size_t _ttyin PROTO((FD *fp, VOIDPTR buff, size_t bytes));					/* Read from tty rtn        */
long _rdasc PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read ascii rtn       */
long _rdbin PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read binary rtn      */
long _wrtasc PROTO((FD *fp, const VOIDPTR buff, long bytes));
long _wrtbin PROTO((FD *fp, const VOIDPTR buff, long bytes));
long _wrtchr PROTO((FD *fp, const VOIDPTR buff, long bytes));
long _pc_readblk PROTO((FD *ccbp, long offset, VOIDPTR buff, long bytes));
long _pc_writeblk PROTO((FD *ccbp, long offset, const VOIDPTR buff, long bytes));
int _ttyout PROTO(( const char *buff ));
int _lstout PROTO(( const char *buff ));
long _blkio PROTO((FD *ccbp, long sector, VOIDPTR buffer, long count, int bdosfunc));
#endif
int _ttyinraw PROTO((NOTHING));

#ifdef __TIME_H__
struct tm *_conSD_r PROTO((time_t clock, int local, struct tm *tp));
struct tm *_conSD PROTO((time_t clock, int local));
time_t _cnvDS PROTO((const struct tm *tm));
time_t ftimtosec PROTO((DOSTIME *f));
#endif

/*
 * calculate Pseudo-Pid from basepage value.
 * A basepage is at least 256 bytes, so the least significant 8
 * bits can safely be dropped.
 * Note that getpid() etc. are not supposed to return an
 * error code (they always succeed), and thus the resulting
 * value might be negative.
 */
#define pid_from_basepage(bp)  ((pid_t) (((long) (bp)) >> 8))

/************************* #include "malloc.h"  *****************************/
#define FB_HDR struct hdr				/* free block header type   */
#define NULLFBH ((FB_HDR *)0)			/* Null of above struct     */

FB_HDR
{										/* mem_block header     */
	struct hdr *ptr;					/* ptr next blk (when freed) */
	size_t size;						/* block size (always)      */
	size_t chksize;						/* 1's complement of size   */
};

#define ERR_FINDBLOCK 	1				/* error in 'findblock()'   */
#define ERR_GETMEM	2					/* error in 'getmemory()'   */
#define ERR_FREE	3					/* error in 'free()'        */

#define AOFFS 1L						/* alignment offset: 0=byte, 1=word, 3=quad */
#define AMASK(c) ((char *)((long)(c) & ~AOFFS))	/* mask alignment bits      */
#define AFUDGE 4						/* leeway for passing block as is */
#define ACHUNKS 64						/* chunks to alloc from O.S. */

VOID _errmalloc PROTO((int code));
int malloc_debug PROTO((NOTHING));

/*** end of "malloc.h" ******/
