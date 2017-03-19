#include "aeslib.h"

int crystal(P(CBLK *)pb)
PP(CBLK *pb;)
{
#ifdef __ALCYON__
	asm("move.l 4(a7),d1");
	asm("move.w #200,d0");
	asm("trap #2");
#endif
#ifdef __GNUC__
    __asm__ volatile
    (
        "move.l  %0,d1\n\t"
        "movew   #200,d0\n\t"
        "trap    #2"
    :
    : "g"(pb)
    : "d0", "d1", "d2", "a0", "a1", "a2", "memory", "cc"
    );
#endif
}
