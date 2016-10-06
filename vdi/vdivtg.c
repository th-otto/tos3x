#include "vdi.h"
#include "alone.h"


VOID VsetRGB(P(int16_t) colorNum, P(int16_t) count, P(const int32_t *) palettePtr)
{
}


int16_t setscreen(P(int32_t) laddr, P(int32_t) paddr, P(int16_t) mode, P(int16_t) opt)
PP(int32_t laddr;)
PP(int32_t paddr;)
{
	register volatile uint8_t *p;
	
	UNUSED(opt);
	if (laddr >= 0)
	{
		*((intptr_t *)0x44e) = laddr;
	}
	if (paddr >= 0)
	{
		p = (volatile uint8_t *)0xFFFF8201;
		*p = paddr >> 16;
		p = (volatile uint8_t *)0xFFFF8203;
		*p = paddr >> 8;
		p = (volatile uint8_t *)0xFFFF820d;
		*p = paddr;
	}
	return mode;
}
