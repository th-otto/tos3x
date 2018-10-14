#include "..\alt\aesalt.h"

BOOLEAN Objc_change(OBJECT *tree, WORD object, const GRECT *clip, WORD state, BOOLEAN redraw)
{
	GRECT r;
	WORD *p = _AESintin;
	
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
	*((GRECT *)p) = r;
	p += 4;
	*p++ = state;
	*p = redraw;
	_AESaddrin[0] = tree;
	return _AESif(148);
}
