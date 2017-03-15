#include "vdilib.h"

VOID vsl_ends(P(int16_t) handle, P(int16_t) beg_style, P(int16_t) end_style)
PP(int16_t handle;)
PP(int16_t beg_style;)
PP(int16_t end_style;)
{
	intin[0] = beg_style;
	intin[1] = end_style;

	contrl[0] = 108;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[6] = handle;
	vdi();
}
