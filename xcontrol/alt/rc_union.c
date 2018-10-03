#include "aesalt.h"

/*
 *  rc_union(): find the union of two rectangles
 */
void rc_union(const GRECT *p1, GRECT *p2)
{
	short tw, th, tx, ty;

	tw = max(p1->g_x + p1->g_w, p2->g_x + p2->g_w);
	th = max(p1->g_y + p1->g_h, p2->g_y + p2->g_h);
	tx = min(p1->g_x, p2->g_x);
	ty = min(p1->g_y, p2->g_y);

	p2->g_x = tx;
	p2->g_y = ty;
	p2->g_w = tw - tx;
	p2->g_h = th - ty;
}
