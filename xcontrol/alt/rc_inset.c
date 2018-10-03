#include "aesalt.h"

void rc_inset(GRECT *r, WORD xoffset, WORD yoffset)
{
	r->g_x += xoffset;
	r->g_y += yoffset;
	r->g_w -= xoffset / 2;
	r->g_h -= yoffset / 2;
}
