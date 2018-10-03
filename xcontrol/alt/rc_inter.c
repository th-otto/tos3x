#include "aesalt.h"

BOOLEAN	rc_intersect(const GRECT *r1, GRECT *r2)
{
	WORD tw, th, tx, ty;
	BOOLEAN ret;

	tw = min (r2->g_x + r2->g_w, r1->g_x + r1->g_w);
	th = min (r2->g_y + r2->g_h, r1->g_y + r1->g_h);
	tx = max (r2->g_x, r1->g_x);
	ty = max (r2->g_y, r1->g_y);
	
	ret = (tw -= tx) > 0;
	if (ret)
	{
		ret = (th -= ty) > 0;
		if (ret)
		{
			r2->g_x = tx;
			r2->g_y = ty;
			r2->g_w = tw;
			r2->g_h = th;
		}
	}
	
	return ret;
}
