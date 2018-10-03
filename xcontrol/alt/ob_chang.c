#include "aesalt.h"

WORD _AesCtrl(long code);

BOOLEAN Objc_change(OBJECT *tree, WORD object, const GRECT *clip,
						WORD state, BOOLEAN redraw )
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
	*p++ = 0;
	*((GRECT *)p)++ = r;
	*p++ = state;
	*p = redraw;
	_GemParBlk.addrin[0] = tree;
	return _AesCtrl(47);
}
