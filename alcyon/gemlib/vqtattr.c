#include "vdilib.h"

VOID vqt_attributes(P(int16_t) handle, P(int16_t *) attributes)
PP(int16_t handle;)
PP(int16_t *attributes;)
{
	i_intout(attributes);
	i_ptsout(attributes + 6);

	contrl[0] = 38;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	i_intout(intout);
	i_ptsout(ptsout);
}
