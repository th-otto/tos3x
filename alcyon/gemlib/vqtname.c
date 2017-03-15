#include "vdilib.h"

int16_t vqt_name(P(int16_t) handle, P(int16_t) element_num, P(char *)name)
PP(int16_t handle;)
PP(int16_t element_num;)
PP(char *name;)
{
	int16_t i;

	intin[0] = element_num;

	contrl[0] = 130;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	for (i = 0; i < 32; i++)
		name[i] = intout[i + 1];
	return intout[0];
}
