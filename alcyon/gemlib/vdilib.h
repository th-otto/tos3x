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

#define i_ptsin(ptr) pioff = ptr
#define i_intin(ptr) iioff = ptr
#define i_intout(ptr) iooff = ptr
#define i_ptsout(ptr) pooff = ptr

extern int16_t *pioff, *iioff, *pooff, *iooff;

VOID i_ptr PROTO((VOIDPTR));
VOID i_ptr2 PROTO((VOIDPTR));
VOID m_lptr2 PROTO((VOIDPTR *));

VOID vdi PROTO((NOTHING));

short vec_len PROTO((short dx, short dy));
