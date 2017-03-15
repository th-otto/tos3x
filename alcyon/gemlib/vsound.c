#include "vdilib.h"

VOID v_sound(P(int16_t) handle, P(int16_t) frequency, P(int16_t) duration)
PP(int16_t handle;)
PP(int16_t frequency;)
PP(int16_t duration;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 61;
	contrl[6] = handle;

	intin[0] = frequency;
	intin[1] = duration;
	vdi();
}
