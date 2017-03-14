#include "aeslib.h"

int16_t objc_draw(P(OBJECT *) tree, P(int16_t) drawob, P(int16_t) depth, P(int16_t) xc, P(int16_t) yc, P(int16_t) wc, P(int16_t) hc)
PP(OBJECT *tree;)
PP(int16_t drawob;)
PP(int16_t depth;)
PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t wc;)
PP(int16_t hc;)
{
	OB_TREE = tree;
	OB_DRAWOB = drawob;
	OB_DEPTH = depth;
	OB_XCLIP = xc;
	OB_YCLIP = yc;
	OB_WCLIP = wc;
	OB_HCLIP = hc;
	return crys_if(OBJC_DRAW);
}
