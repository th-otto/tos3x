#include "vdilib.h"

short vst_height(P(int16_t) handle, P(int16_t) height, P(int16_t *) char_width, P(int16_t *) char_height, P(int16_t *) cell_width, P(int16_t *) cell_height)
PP(int16_t handle;)
PP(int16_t height;)
PP(int16_t *char_width;)
PP(int16_t *char_height;)
PP(int16_t *cell_width;)
PP(int16_t *cell_height;)
{
	ptsin[0] = 0;
	ptsin[1] = height;

	contrl[0] = 12;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*char_width = ptsout[0];
	*char_height = ptsout[1];
	*cell_width = ptsout[2];
	*cell_height = ptsout[3];
	return intout[0];
}
