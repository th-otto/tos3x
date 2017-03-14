#include "aeslib.h"

short rc_intersect(P(const GRECT *) r1, P(GRECT *) r2)
PP(register const GRECT *r1;)
PP(register GRECT *r2;)
{
	register short tx, ty, tw, th;

	tw = min(r2->g_x + r2->g_w, r1->g_x + r1->g_w);
	th = min(r2->g_y + r2->g_h, r1->g_y + r1->g_h);
	tx = max(r2->g_x, r1->g_x);
	ty = max(r2->g_y, r1->g_y);
	
	r2->g_x = tx;
	r2->g_y = ty;
	r2->g_w = tw - tx;
	r2->g_h = th - ty;
	
	return tw > tx && th > ty;
}
