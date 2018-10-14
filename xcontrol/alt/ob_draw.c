#include "aesalt.h"

#ifdef __TURBOC__

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

#endif


#ifdef LATTICE

static void ObjcGrect(OBJECT *tree, WORD object, GRECT *r)
{
	objc_offset(tree, object, &r.g_x, &r.g_y);
	r->g_w = tree[object].ob_width;
	r->g_h = tree[object].ob_height;
}

BOOLEAN Objc_draw(OBJECT *tree, WORD object, WORD depth, const GRECT *clip)
{
	GRECT r;
	
	if (clip == NULL)
	{
		clip = &r;
		ObjcGrect(tree, object, &r);
	}
	return objc_draw(tree, object, depth, r.g_x, r.g_y, r.g_w, r.g_h);
}
#endif
