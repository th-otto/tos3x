#include "aesalt.h"

BOOLEAN	rc_inside(WORD x, WORD y, GRECT *r)
{
	WORD x2, y2;
	BOOLEAN ret;
	
	x2 = r->g_x + r->g_w;
	y2 = r->g_y + r->g_h;
	ret = x >= r->g_x && y >= r->g_y && x < x2 && y < y2;
	return ret;
}
