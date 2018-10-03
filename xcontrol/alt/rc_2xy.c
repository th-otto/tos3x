#include "aesalt.h"

void rc_2xy(const GRECT *r, WORD *pxy)
{
	*pxy++ = r->g_x;
	*pxy++ = r->g_y;
	*pxy++ = r->g_x + r->g_w - 1;
	*pxy++ = r->g_y + r->g_h - 1;
}
