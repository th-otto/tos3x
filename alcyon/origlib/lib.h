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

#ifndef __attribute__
#  ifndef __GNUC__
#    define __attribute__(x)
#  endif
#endif


#define CREATMODE 1						/* mode to use for 'creat'  */

/* file descriptors */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern char __tname[];					/* Terminal name        */
extern char __pname[];					/* Program name         */
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

int _creat PROTO((const char *name, int prot, int type));
int _open PROTO((const char *name, int mode, int type));
int __open PROTO((int fd, const char *name, int search));

VOID _optoff PROTO((const char * msg)) __attribute__((noreturn));

VOID _chinit PROTO((NOTHING));
int _main PROTO((char *com, int len));
int __main PROTO((char *com, int len));

int _filbuf PROTO((FILE *));
long _filesz PROTO((int fd));
char *_salloc PROTO((size_t size));					/* Stack allocation routine */

#ifdef __OSIF_H__
FD *_chkc PROTO((int fd));						/* Converts fd to fp */
size_t _ttyin PROTO((FD *fp, VOIDPTR buff, size_t bytes));					/* Read from tty rtn        */
size_t _rdasc PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read ascii rtn       */
size_t _rdbin PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read binary rtn      */
size_t _wrtasc PROTO((FD *fp, const VOIDPTR buff, long bytes));
size_t _wrtbin PROTO((FD *fp, const VOIDPTR buff, long bytes));
size_t _pc_readblk PROTO((struct fcbtab *fcb, long offset, VOIDPTR buff, size_t bytes, size_t secsiz));
size_t _pc_writeblk PROTO((struct fcbtab *fcb, long offset, const VOIDPTR buff, size_t bytes, size_t secsiz));
#endif

VOID nottyin PROTO((NOTHING));
VOID nostart PROTO((NOTHING));
VOID nodisk PROTO((NOTHING));
VOID nolong PROTO((NOTHING));
VOID noascii PROTO((NOTHING));
VOID nobinary PROTO((NOTHING));
VOID nofilesz PROTO((NOTHING));
VOID nofloat PROTO((NOTHING));
VOID nowildcards PROTO((NOTHING));

int main PROTO((int argc, char **argv, char **envp));
