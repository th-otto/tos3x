#include "aesalt.h"

void xy_2rect(const WORD *pxy, GRECT *r)
{
	WORD x, y;
	
	x = *pxy++;
	y = *pxy++;
	r->g_x = x;
	r->g_y = y;
	r->g_h = *pxy++ - x + 1; /* BUG */
	r->g_w = *pxy - y + 1; /* BUG */
}
