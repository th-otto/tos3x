#include "vdilib.h"

int16_t vsm_type(P(int16_t) handle, P(int16_t) symbol)
PP(int16_t handle;)
PP(int16_t symbol;)
{
	intin[0] = symbol;

	contrl[0] = 18;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
