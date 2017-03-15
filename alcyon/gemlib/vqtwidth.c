#include "vdilib.h"

int16_t vqt_width(P(int16_t) handle, P(short) character, P(int16_t *) cell_width, P(int16_t *) left_delta, P(int16_t *) right_delta)
PP(int16_t handle;)
PP(short character;)
PP(int16_t *cell_width;)
PP(int16_t *left_delta;)
PP(int16_t *right_delta;)
{
	intin[0] = character;

	contrl[0] = 117;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	*cell_width = ptsout[0];
	*left_delta = ptsout[2];
	*right_delta = ptsout[4];
	return intout[0];
}
