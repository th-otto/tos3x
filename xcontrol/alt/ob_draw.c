#include "aesalt.h"

WORD _AesCtrl(long code);

BOOLEAN Objc_draw(OBJECT *tree, WORD object, WORD depth, const GRECT *clip)
{
	GRECT r;
	int *p = _GemParBlk.intin;
	
	if (clip == NULL)
	{
		objc_offset(tree, object, &r.g_x, &r.g_y);
		r.g_w = tree[object].ob_width;
		r.g_h = tree[object].ob_height;
	} else
	{
		r = *clip;
	}
	*p++ = object;
	*p++ = depth;
	*((GRECT *)p)++ = r;
	_GemParBlk.addrin[0] = tree;
	return _AesCtrl(42);
}
