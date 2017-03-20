#include "aeslib.h"

static CBLK c = {
	control,
	global,
	int_in,
	int_out,
	addr_in,
	addr_out
};

VOID crystal(NOTHING)
{
#ifdef __ALCYON__
	asm("move.l #L1,d1");
	asm("move.w #200,d0");
	asm("trap #2");
#endif
#ifdef __GNUC__
    __asm__ volatile
    (
        "move.l  %0,d1\n\t"
        "move.w  #200,d0\n\t"
        "trap    #2"
    :
    : "g"(&c)
    : "d0", "d1", "d2", "a0", "a1", "a2", "memory", "cc"
    );
#endif
}
