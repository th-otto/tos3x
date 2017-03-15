#include "vdilib.h"

int16_t vqp_films(P(int16_t) handle, P(char *)name)
PP(int16_t handle;)
PP(char *name;)
{
	int16_t i;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 91;
	contrl[6] = handle;
	intin[0] = 0;
	vdi();

	for (i = 0; i < contrl[4]; i++)
		name[i] = intout[i];
	name[i] = '\0';
	return contrl[4];
}
