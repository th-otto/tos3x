#include "vdilib.h"

int16_t vsm_choice(P(int16_t) handle, P(int16_t *) choice)
PP(int16_t handle;)
PP(int16_t *choice;)
{
	contrl[0] = 30;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*choice = intout[0];
	return contrl[4];
}
