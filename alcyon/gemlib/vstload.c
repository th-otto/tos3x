#include "vdilib.h"

int16_t vst_load_fonts(P(int16_t) handle, P(int16_t) select)
PP(int16_t handle;)
PP(int16_t select;)
{
	contrl[0] = 119;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	intin[0] = select;
	vdi();
	return intout[0];
}
