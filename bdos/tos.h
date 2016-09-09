#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

#ifndef __GNUC__
#define __builtin_unreachable()
#endif

typedef int16_t BOOLEAN;
#define FALSE 0
#define TRUE 1


#define    ERR      -1L    /* basic, fundamental error     0xffffffff      */

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

typedef uint16_t CLNO;             /* cluster number */
typedef uint16_t RECNO;            /* record number  */
typedef uint32_t LRECNO;           /* record number  */

VOID _osinit PROTO((NOTHING));
VOID osinit PROTO((NOTHING));
VOID gouser PROTO((NOTHING));

VOID kprintf PROTO((const char *fmt, ...));

typedef int32_t xjmp_buf[3];

extern xjmp_buf errbuf;		/*  in sup.c			*/

int32_t xsetjmp PROTO((xjmp_buf buf));
VOID xlongjmp PROTO((xjmp_buf buf, int32_t rc));


extern const char *bdosts;
