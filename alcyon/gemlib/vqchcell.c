#include "vdilib.h"

VOID vq_chcells(P(int16_t) handle, P(int16_t *) rows, P(int16_t *) columns)
PP(int16_t handle;)
PP(int16_t *rows;)
PP(int16_t *columns;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 1;
	contrl[6] = handle;
	vdi();

	*rows = intout[0];
	*columns = intout[1];
}
