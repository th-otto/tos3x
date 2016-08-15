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

#define CREATMODE 1						/* mode to use for 'creat'  */

/* file descriptors */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern char __tname[];					/* Terminal name        */
extern char __pname[];					/* Program name         */
extern char *_break;					/* -> Program break location */

VOID _petoa PROTO((char *, char *, int, int));
VOID _pftoa PROTO((char *, char *, int, int));
VOID _pgtoa PROTO((char *, char *, int, int));
VOID _atof PROTO((const char *));

typedef char *(*printfunc) PROTO((long n, char **pbuf, int base, int issigned, char *digs));

char *__prtshort PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtld PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtint PROTO((char *, char *pbuf, int base, int issigned, printfunc f));

int __doprint PROTO((FILE *stream, const char *fmt, int mode, va_list args));

long fptoffp PROTO((double f));
double ffptof PROTO((long lf));
long fpftol PROTO((long f));
long fpltof PROTO((long l));
int _doprt PROTO((FILE *sp, const char * fmt, char *pb));

int _creat PROTO((const char *name, int prot, int type));
int _open PROTO((const char *name, int mode, int type));

VOID nofloat PROTO((NOTHING));
VOID _optoff PROTO((const char * msg));

VOID _chinit PROTO((NOTHING));
int _main PROTO((char *com, int len));
int __main PROTO((char *com, int len));

int _filbuf PROTO((FILE *));
