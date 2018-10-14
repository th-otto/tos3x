#include "aesalt.h"

void rc_shrink(GRECT *r, WORD xoffset, WORD yoffset)
{
	r->g_w -= xoffset;
	r->g_h -= yoffset;
}

void rc_inset(GRECT *r, WORD xoffset, WORD yoffset)
{
	r->g_x += xoffset;
	r->g_y += yoffset;
}
