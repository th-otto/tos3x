#include "vdilib.h"

VOID vrq_choice(P(int16_t) handle, P(int16_t) in_choice, P(int16_t *) out_choice)
PP(int16_t handle;)
PP(int16_t in_choice;)
PP(int16_t *out_choice;)
{
	intin[0] = in_choice;

	contrl[0] = 30;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	*out_choice = intout[0];
}
