#include "vdilib.h"

VOID v_curtext(P(int16_t) handle, P(const char *) string)
PP(int16_t handle;)
PP(const char *string;)
{
	int16_t *intstr;

	intstr = intin;
	do
		;
	while ((*intstr++ = *string++ & 0xff) != 0);

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = ((int) ((intptr_t)intstr - (intptr_t)intin) >> 1) - 1;
	contrl[5] = 12;
	contrl[6] = handle;
	vdi();
}
