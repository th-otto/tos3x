#include <stdint.h>
#include <vdibind.h>

#define FALSE 0
#define TRUE  1

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

extern int16_t contrl[];
extern int16_t intin[];
extern int16_t ptsin[];
extern int16_t intout[];
extern int16_t ptsout[];
extern VDIPB pblock;

#define i_ptsin(ptr) pblock.ptsin = ptr
#define i_intin(ptr) pblock.intin = ptr
#define i_intout(ptr) pblock.intout = ptr
#define i_ptsout(ptr) pblock.ptsout = ptr

/*
 * dereferencing a type-casted ptr does not work in Alcyon,
 * you need to assign it to a temporary first.
 */
#define i_ptr(p) t = (VOIDPTR *)&contrl[7]; *t = (VOID *)(p)
#define i_ptr2(p) t = (VOIDPTR *)&contrl[9]; *t = (VOID *)(p)

#define m_lptr2(p) t = (VOIDPTR *)&contrl[9]; *((VOID **)(p)) = *t

VOID vdi PROTO((NOTHING));

short vec_len PROTO((short dx, short dy));
