
#ifdef __cplusplus
#  define __USE_STDARG 1
#endif
#ifdef __STDC__
#  define __USE_STDARG 1
#endif
#ifdef __USE_STDARG
#  include <stdarg.h>
#  define __c_va_alist				, ...
#  define __c_va_dcl
#  define __c_va_list				va_list
#  define __c_va_start(pvar, prev) 	va_start(pvar, prev)
#  define __c_va_arg(pvar, type)	va_arg(pvar, type)
#  define __c_va_end(pvar) 			va_end(pvar)
#else
#  include <varargs.h>
#  define __c_va_alist				, va_alist
#  define __c_va_dcl				va_dcl
#  define __c_va_list				va_list
#  define __c_va_start(pvar, prev) 	va_start(pvar)
#  define __c_va_arg(pvar, type)	va_arg(pvar, type)
#  define __c_va_end(pvar) 			va_end(pvar)
#endif

#define __c_va_no_args NULL
#define __c_va_is_no_args(args) ((args) == __c_va_no_args)

#include <stdio.h>

#define CREATMODE 1						/* mode to use for 'creat'  */

/* file descriptors */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern char __tname[];					/* Terminal name        */
extern char __pname[];					/* Program name         */
extern char *_break;					/* -> Program break location */

char *petoa PROTO((double fp, char *buf, int prec));
char *pftoa PROTO((double fp, char *buf, int prec));

typedef char *(*printfunc) PROTO((long n, char **pbuf, int base, int issigned, char *digs));

char *__prtshort PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtld PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtint PROTO((long n, char *pbuf, int base, int issigned, printfunc f, int upper));

int __doprint PROTO((FILE *stream, const char *fmt, int mode, va_list args));

long fptoffp PROTO((double f));
double ffptof PROTO((long lf));
long fpftol PROTO((long f));
long fpltof PROTO((long l));

extern long ldivr;

long ldiv PROTO((long al1, long al2));
long lrem PROTO((long al1, long al2));

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

int _creat PROTO((const char *name, int prot, int type));
int _open PROTO((const char *name, int mode, int type));

VOID _chinit PROTO((NOTHING));
int _main PROTO((char *com, int len));
int __main PROTO((char *com, int len));

int _filbuf PROTO((FILE *));

