#include "aesalt.h"

BOOLEAN Graf_dragbox(GRECT *box, const GRECT *bound)
{
	int *p = _GemParBlk.intin;
	WORD *out;
	WORD ret;
	
	*p++ = box->g_w;
	*p++ = box->g_h;
	*p++ = box->g_x;
	*p++ = box->g_y;
	*p++ = bound->g_x;
	*p++ = bound->g_y;
	*p++ = bound->g_w;
	*p++ = bound->g_h;
	out = _AesCtrl(71);
	ret = out[-1];
	box->g_x = *out++;
	box->g_y = *out;
	return ret;
}
