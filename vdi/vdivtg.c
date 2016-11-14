#include "vdi.h"
#include "alone.h"
#include "falcon.h"

int16_t falcon_videomode;

#define Srealloc(size) vtrap1(21, size)
#define VsetMode(mode) mode

#define VcheckMode mode_validate


int16_t VcheckMode(P(int16_t) mode)
PP(int16_t mode;)
{
	return mode;
}


int16_t VgetMonitor(NOTHING)
{
	register volatile char * p = (volatile char *)0xffff8006;
	return (*p >> 6) & 0x03;
}


VOID VsetRGB(P(int16_t) colorNum, P(int16_t) count, P(const int32_t *) palettePtr)
{
}


int16_t mode_vwidth(P(int16_t) mode)
PP(int16_t mode;)
{
}


int16_t mode_yres(P(int16_t) mode)
PP(int16_t mode;)
{
}


int32_t VgetSize(P(int16_t) mode)
PP(int16_t mode;)
{
	int16_t w;
	
	w = mode_vwidth(mode);
	return ((int32_t)mode_yres(mode) * (int32_t)w) << 1;
}


int16_t setscreen(P(int32_t) laddr, P(int32_t) paddr, P(int16_t) rez, P(int16_t) mode)
PP(register int32_t laddr;)
PP(register int32_t paddr;)
PP(register int16_t rez;)
PP(int16_t mode;)
{
	register volatile uint8_t *p;
	
	if (rez == 3 && mode != -1)
	{
		VcheckMode(mode);
		if (laddr == 0 && paddr == 0)
		{
			laddr = paddr = Srealloc(VgetSize(mode));
			if (laddr == 0)
				return -1;
		}
	}
	if (laddr > 0)
	{
		*((intptr_t *)0x44e) = laddr;
	}
	if (paddr > 0)
	{
		p = (volatile uint8_t *)0xFFFF8201;
		*p = paddr >> 16;
		p = (volatile uint8_t *)0xFFFF8203;
		*p = paddr >> 8;
		p = (volatile uint8_t *)0xFFFF820d;
		*p = paddr;
	}
	switch (rez)
	{
	case -1:
		mode = -1;
		break;
	case 0:
		mode = (falcon_videomode & VIDEL_PAL) | (VIDEL_COMPAT | VIDEL_4BPP);
		goto setmode;
	case 1:
		mode = (falcon_videomode & VIDEL_PAL) | (VIDEL_COMPAT | VIDEL_2BPP | VIDEL_80COL);
		goto setmode;
	case 2:
		mode = (falcon_videomode & VIDEL_PAL) | (VIDEL_COMPAT | VIDEL_1BPP | VIDEL_80COL);
	setmode:
		mode = VcheckMode(mode);
		break;
	case 3:
		break;
	default:
		return -1;
	}
	
	if (rez >= 0)
		*((char *)0x44c) = rez;
	
	mode = VsetMode(mode);
	
	if (rez >= 0)
	{
		
	}
	
	if (mode == (VIDEL_OVERSCAN | VIDEL_VGA | VIDEL_80COL))
	{
	}
	
	return mode;
}
