#include "vdilib.h"

int16_t vst_effects(P(int16_t) handle, P(int16_t) effect)
PP(int16_t handle;)
PP(int16_t effect;)
{
	intin[0] = effect;

	contrl[0] = 106;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
