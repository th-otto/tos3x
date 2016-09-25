#ifndef __COMPILER_H__
#include <compiler.h>
#endif
#ifndef __STDINT_H__
#include <stdint.h>
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

typedef int16_t RECNO;             /* record number  */ /* BUG: should be unsigned */
typedef int32_t LRECNO;            /* record number  */ /* BUG: should be unsigned, but Alcyon does not support unsigned long */



/*
 * blkdev.c
 */
VOID bhdv_init PROTO((NOTHING));
ERROR bhdv_getbpb PROTO((int16_t dev));
ERROR bhdv_mediach PROTO((int16_t dev));
ERROR bhdv_rwabs PROTO((int16_t rw, char *buf, int16_t cnt, RECNO recnr, int16_t dev, LRECNO lrecnr));
ERROR bhdv_boot PROTO((NOTHING));


/*
 * chardev.c
 */
int32_t bcon1stat PROTO((NOTHING));
int32_t bcon2stat PROTO((NOTHING));
int32_t bcon3stat PROTO((NOTHING));
int32_t bcon0in PROTO((NOTHING));
int32_t bcon1in PROTO((NOTHING));
int32_t bcon2in PROTO((NOTHING));
int32_t bcon3in PROTO((NOTHING));
int32_t bco0stat PROTO((NOTHING));
int32_t bco1stat PROTO((NOTHING));
int32_t bco2stat PROTO((NOTHING));
int32_t bco3stat PROTO((NOTHING));
int32_t bco4stat PROTO((NOTHING));
int32_t bcon0out PROTO((int16_t dev, int16_t c));
int32_t bcon1out PROTO((int16_t dev, int16_t c));
int32_t bcon2out PROTO((int16_t dev, int16_t c));
int32_t bcon3out PROTO((int16_t dev, int16_t c));
int32_t bcon4out PROTO((int16_t dev, int16_t c));
int32_t bcon5out PROTO((int16_t dev, int16_t c));
VOID midiws PROTO((uint16_t cnt, const VOIDPTR ptr));
VOID ikbdws PROTO((uint16_t cnt, const VOIDPTR ptr));
ERROR dbmsg PROTO((int16_t rsrvd, int16_t msg_num, int32_t msg_arg));
