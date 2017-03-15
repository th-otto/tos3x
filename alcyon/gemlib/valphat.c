#include "vdilib.h"

VOID v_alpha_text(P(int16_t) handle, P(const char *) string)
PP(int16_t handle;)
PP(const char *string;)
{
	int16_t i;

	i = 0;
	while ((intin[i++] = *string++ & 0xff) != 0)
		;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = --i;
	contrl[5] = 25;
	contrl[6] = handle;
	vdi();
}
