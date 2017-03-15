#include "aeslib.h"


/*
 * rc_union	Returns the union of two rectangles in r2.
 */

VOID rc_union(P(const GRECT *) r1, P(GRECT *) r2)
PP(register const GRECT *r1;)
PP(register GRECT *r2;)
{
	register short tx, ty, tw, th;
	
	tw = max(r1->g_x + r1->g_w, r2->g_x + r2->g_w);
	th = max(r1->g_y + r1->g_h, r2->g_y + r2->g_h);
	tx = min(r1->g_x, r2->g_x);
	ty = min(r1->g_y, r2->g_y);
	r2->g_x = tx;
	r2->g_y = ty;
	r2->g_w = tw - tx;
	r2->g_h = th - ty;
}
