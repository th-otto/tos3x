#include "vdilib.h"

VOID vqt_extent(P(int16_t) handle, P(const char *) string, P(int16_t *) extent)
PP(int16_t handle;)
PP(const char *string;)
PP(int16_t *extent;)
{
	int16_t *intstr;

	intstr = intin;
	do
		;
	while ((*intstr++ = *string++ & 0xff) != 0);

	i_ptsout(extent);

	contrl[0] = 116;
	contrl[1] = 0;
	contrl[3] = ((int) ((intptr_t)intstr - (intptr_t)intin) >> 1) - 1;
	contrl[6] = handle;
	vdi();

	i_ptsout(ptsout);
}
