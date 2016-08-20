#ifndef __LIBC_H__
#define __LIBC_H__ 1

#ifdef __cplusplus
#  define __USE_STDARG 1
#endif
#ifdef __STDC__
#  define __USE_STDARG 1
#endif
#ifdef __USE_STDARG
#  include <stdarg.h>
#  define _va_alist				, ...
#  define _va_dcl
#  define _va_list				va_list
#  define _va_start(pvar, prev) va_start(pvar, prev)
#  define _va_arg(pvar, type)	va_arg(pvar, type)
#  define _va_end(pvar) 		va_end(pvar)
#else
#  include <varargs.h>
#  define _va_alist				, va_alist
#  define _va_dcl				va_dcl
#  define _va_list				va_list
#  define _va_start(pvar, prev) va_start(pvar)
#  define _va_arg(pvar, type)	va_arg(pvar, type)
#  define _va_end(pvar) 		va_end(pvar)
#endif

#define _va_no_args NULL
#define _va_is_no_args(args) ((args) == _va_no_args)

#include <stdio.h>

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

extern char __pname[];					/* Program name         */
extern char __tname[];					/* Terminal name        */
extern char __lname[];					/* List device name     */
extern char *_break;					/* -> Program break location */

char *_petoa PROTO((double *, char *, int, int));
char *_pftoa PROTO((double *, char *, int, int));
char *_pgtoa PROTO((double *, char *, int, int));
VOID _atof PROTO((const char *));

typedef char *(*printfunc) PROTO((char *pobj, char **pbuf, int base, int issigned, char *digs));

char *__prtshort PROTO((char *pobj, char **pbuf, int base, int issigned, char *digs));
char *__prtld PROTO((char *pobj, char **pbuf, int base, int issigned, char *digs));
char *__prtint PROTO((char *pobj, char *pbuf, int base, int issigned, printfunc f));

int __doprint PROTO((FILE *stream, const char *fmt, int mode, va_list args));

long fptoffp PROTO((double f));
double ffptof PROTO((long lf));
long fpftol PROTO((long f));
long fpltof PROTO((long l));
int _doprt PROTO((FILE *sp, const char * fmt, char *pb));

int _creat PROTO((const char *name, mode_t prot, int binary));
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
VOID __fdecls PROTO((NOTHING));
int _flsbuf PROTO((int c, FILE *fp));

#ifdef __OSIF_H__
char *_parsefn PROTO((const char *filename, struct fcbtab *fdb));
FD *_chkc PROTO((unsigned int fd));						/* Converts fd to fp */
size_t _ttyin PROTO((FD *fp, VOIDPTR buff, size_t bytes));					/* Read from tty rtn        */
size_t _rdasc PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read ascii rtn       */
size_t _rdbin PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read binary rtn      */
size_t _wrtasc PROTO((FD *fp, const VOIDPTR buff, size_t bytes));
size_t _wrtbin PROTO((FD *fp, const VOIDPTR buff, size_t bytes));
size_t _wrtchr PROTO((FD *fp, const VOIDPTR buff, size_t bytes));
size_t _pc_readblk PROTO((struct fcbtab *fcb, long offset, VOIDPTR buff, size_t bytes, size_t secsiz));
size_t _pc_writeblk PROTO((struct fcbtab *fcb, long offset, const VOIDPTR buff, size_t bytes, size_t secsiz));
int _ttyout PROTO(( const char *buff ));
int _lstout PROTO(( const char *buff ));
long _blkio PROTO((FD *ccbp, long sector, VOIDPTR buffer, long count, int bdosfunc));
#endif

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

VOID ___atab PROTO((NOTHING));

#define strcasecmp _strcmp


#endif /* __LIBC_H__ */
