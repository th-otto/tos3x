#include "vdilib.h"

VOID vqf_attributes(P(int16_t) handle, P(int16_t *) attributes)
PP(int16_t handle;)
PP(int16_t *attributes;)
{
	i_intout(attributes);

	contrl[0] = 37;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	i_intout(intout);
}
