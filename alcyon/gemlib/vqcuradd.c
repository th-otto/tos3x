#include "vdilib.h"

VOID vq_curaddress(P(int16_t) handle, P(int16_t *) row, P(int16_t *) column)
PP(int16_t handle;)
PP(int16_t *row;)
PP(int16_t *column;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 15;
	contrl[6] = handle;
	vdi();

	*row = intout[0];
	*column = intout[1];
}
