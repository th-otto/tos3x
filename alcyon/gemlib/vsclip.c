#include "vdilib.h"

VOID vs_clip(P(int16_t) handle, P(int16_t) clip_flag, P(int16_t *) xy)
PP(int16_t handle;)
PP(int16_t clip_flag;)
PP(int16_t *xy;)
{
	i_ptsin(xy);
	intin[0] = clip_flag;

	contrl[0] = 129;
	contrl[1] = 2;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
