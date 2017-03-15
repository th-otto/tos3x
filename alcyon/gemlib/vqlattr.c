#include "vdilib.h"

VOID vql_attributes(P(int16_t) handle, P(int16_t *) attributes)
PP(int16_t handle;)
PP(int16_t *attributes;)
{
	i_intout(attributes);

	contrl[0] = 35;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	i_intout(intout);
	attributes[3] = ptsout[0];
}
