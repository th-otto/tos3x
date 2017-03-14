#include "aeslib.h"

int16_t objc_change(
	P(OBJECT *) tree, P(int16_t) drawob, P(int16_t) depth,
	P(int16_t) xc, P(int16_t) yc, P(int16_t) wc, P(int16_t) hc,
	P(int16_t) newstate, P(int16_t) redraw)
PP(OBJECT *tree;)

PP(int16_t drawob;)
PP(int16_t depth;)

PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t wc;)
PP(int16_t hc;)

PP(int16_t newstate;)
PP(int16_t redraw;)
{
	OB_TREE = tree;
	OB_DRAWOB = drawob;
	OB_DEPTH = depth;
	OB_XCLIP = xc;
	OB_YCLIP = yc;
	OB_WCLIP = wc;
	OB_HCLIP = hc;
	OB_NEWSTATE = newstate;
	OB_REDRAW = redraw;
	return crys_if(OBJC_CHANGE);
}
