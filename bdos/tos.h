#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
#endif

#define TOSVERSION 0x0306

/* major.minor */
#if TOSVERSION >= 0x0100
#undef GEMDOS
#define GEMDOS 0x0013
#endif
#if TOSVERSION >= 0x0103
#undef GEMDOS
#define GEMDOS 0x0014
#endif
#if TOSVERSION >= 0x0104
#undef GEMDOS
#define GEMDOS 0x0015
#endif
#if TOSVERSION >= 0x0162
#undef GEMDOS
#define GEMDOS 0x0017
#endif
#if TOSVERSION >= 0x0201
#undef GEMDOS
#define GEMDOS 0x0019
#endif
#if TOSVERSION >= 0x0306
#undef GEMDOS
#define GEMDOS 0x0020
#endif


#ifdef __GNUC__
#define unreachable() __builtin_unreachable()
#else
#define unreachable()
#endif

typedef int16_t BOOLEAN;
#define FALSE 0
#define TRUE 1

/* ERROR - error return code */
typedef	int32_t ERROR;
 
#define    ERR      -1L    /* basic, fundamental error     0xffffffff      */

#ifdef __ALCYON__
#  define UNUSED(x)
#else
#  define UNUSED(x) ((void)(x))
#endif

typedef int16_t CLNO;              /* cluster number */ /* BUG: should be unsigned */
typedef int16_t RECNO;             /* record number  */ /* BUG: should be unsigned */
typedef int32_t LRECNO;            /* record number  */ /* BUG: should be unsigned, but Alcyon does not support unsigned long */

VOID _osinit PROTO((NOTHING));
VOID osinit PROTO((NOTHING));
VOID gouser PROTO((NOTHING));

VOID kprintf PROTO((const char *fmt, ...));

typedef int32_t xjmp_buf[3];

extern xjmp_buf errbuf;		/*  in sup.c			*/

int32_t xsetjmp PROTO((xjmp_buf buf));
VOID xlongjmp PROTO((xjmp_buf buf, int32_t rc));
long lmul PROTO((long l1, long l2));