#include "vdilib.h"

int16_t vst_point(P(int16_t) handle, P(int16_t) point, P(int16_t *) char_width, P(int16_t *) char_height, P(int16_t *) cell_width, P(int16_t *) cell_height)
PP(int16_t handle;)
PP(int16_t point;)
PP(int16_t *char_width;)
PP(int16_t *char_height;)
PP(int16_t *cell_width;)
PP(int16_t *cell_height;)
{
	intin[0] = point;

	contrl[0] = 107;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	*char_width = ptsout[0];
	*char_height = ptsout[1];
	*cell_width = ptsout[2];
	*cell_height = ptsout[3];
	return intout[0];
}
