#include "..\alt\aesalt.h"

static void ObjcGrect(OBJECT *tree, WORD object, GRECT *r)
{
	objc_offset(tree, object, &r->g_x, &r->g_y);
	r->g_w = tree[object].ob_width;
	r->g_h = tree[object].ob_height;
}


static void ObjcXGrect(OBJECT *tree, WORD object, GRECT *r)
{
	WORD framesize;
	WORD offset;
	void **ob_spec;
	
	ObjcGrect(tree, object, r);
	tree += object;
	if (tree->ob_flags & INDIRECT)
	{
		ob_spec = tree->ob_spec;
	} else
	{
		ob_spec = &tree->ob_spec;
	}
	if (tree->ob_type == G_TEXT || tree->ob_type == G_BOXTEXT || tree->ob_type == G_FTEXT || tree->ob_type == G_FBOXTEXT)
	{
		framesize = ((TEDINFO *)(*ob_spec))->te_thickness;
	} else
	{
		framesize = ((ObInfo *)(ob_spec))->border;
	}
	if (framesize >= 0)
	{
		offset = 0;
	} else
	{
		offset = framesize;
		framesize = -framesize;
	}
	if ((tree->ob_state & OUTLINED) && offset > -3)
	{
		offset = -3;
	}
	if (offset != 0)
	{
		Rc_center(r, offset, offset, r);
	}
	if ((tree->ob_state & SHADOWED) && framesize != 0)
	{
		r->g_w += framesize + framesize;
		r->g_h += framesize + framesize;
	}
}


BOOLEAN ObjcDraw(OBJECT *tree, WORD object, WORD depth, const GRECT *clip)
{
	GRECT r;
	GRECT *grin;
	
	_AESintin[0] = object;
	grin = (GRECT *)(_AESintin + 2);
	_AESintin[1] = depth;
	if (clip == NULL)
	{
		clip = &r;
		ObjcXGrect(tree, object, &r);
	}
	*grin = *clip;
	_AESaddrin[0] = tree;
	return _AESif(128);
}
