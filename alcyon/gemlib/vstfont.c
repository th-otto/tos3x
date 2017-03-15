#include "vdilib.h"

int16_t vst_font(P(int16_t) handle, P(int16_t) font)
PP(int16_t handle;)
PP(int16_t font;)
{
	intin[0] = font;

	contrl[0] = 21;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
