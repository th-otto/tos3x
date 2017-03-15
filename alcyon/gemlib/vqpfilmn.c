#include "vdilib.h"

int16_t vqp_filmname(P(int16_t) handle, P(int16_t) index, P(char *) name)
PP(int16_t handle;)
PP(int16_t index;)
PP(char *name;)
{
	int16_t i;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 91;
	contrl[6] = handle;
	intin[0] = index;
	vdi();

	for (i = 0; i < contrl[4]; i++)
		name[i] = intout[i];
	name[i] = '\0';
	return contrl[4];
}
