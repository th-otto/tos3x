#include "vdilib.h"

VDIPB pblock = {
	contrl,
	intin,
	ptsin,
	intout,
	ptsout
};


short	contrl[12];
short	intin[128];
short	ptsin[128];
short	intout[128];
short	ptsout[128];

VOID vdi(NOTHING)
{
#ifdef __ALCYON__
	asm("move.l #_pblock,d1");
	asm("moveq.l #115,d0");
	asm("trap #2");
#endif
#ifdef __GNUC__
    __asm__ volatile
    (
        "move.l  %0,d1\n\t"
        "moveq   #115,d0\n\t"
        "trap    #2"
    :
    : "g"(&pblock)
    : "d0", "d1", "d2", "a0", "a1", "a2", "memory", "cc"
    );
#endif
}
