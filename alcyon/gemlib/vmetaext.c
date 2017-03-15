#include "vdilib.h"

VOID v_meta_extents(P(int16_t) handle, P(int16_t) min_x, P(int16_t) min_y, P(int16_t) max_x, P(int16_t) max_y)
PP(int16_t handle;)
PP(int16_t min_x;)
PP(int16_t min_y;)
PP(int16_t max_x;)
PP(int16_t max_y;)
{
	ptsin[0] = min_x;
	ptsin[1] = min_y;
	ptsin[2] = max_x;
	ptsin[3] = max_y;

	contrl[0] = 5;
	contrl[1] = 2;
	contrl[3] = 0;
	contrl[5] = 98;
	contrl[6] = handle;
	vdi();
}
