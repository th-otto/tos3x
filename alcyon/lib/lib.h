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
char *pgtoa PROTO((double fp, char *buf, int prec));

typedef char *(*printfunc) PROTO((long n, char **pbuf, int base, int issigned, char *digs));

char *__prtshort PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtld PROTO((long n, char **pbuf, int base, int issigned, char *digs));
char *__prtint PROTO((long n, char *pbuf, int base, int issigned, printfunc f, int upper));

int __doprint PROTO((FILE *stream, const char *fmt, int mode, va_list args));

long fptoffp PROTO((double f));
double ffptof PROTO((long lf));
long fpftol PROTO((long f));
long fpltof PROTO((long l));
int _doprt PROTO((FILE *sp, const char * fmt, char *pb));

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
long _rdasc PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read ascii rtn       */
long _rdbin PROTO((FD *fp, VOIDPTR buff, long bytes));					/* Read binary rtn      */
long _wrtasc PROTO((FD *fp, const VOIDPTR buff, long bytes));
long _wrtbin PROTO((FD *fp, const VOIDPTR buff, long bytes));
long _pc_readblk PROTO((struct fcbtab *fcb, long offset, VOIDPTR buff, long bytes));
long _pc_writeblk PROTO((struct fcbtab *fcb, long offset, const VOIDPTR buff, long bytes));
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
