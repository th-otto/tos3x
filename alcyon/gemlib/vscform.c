#include "vdilib.h"

VOID vsc_form(P(int16_t) handle, P(int16_t *) cur_form)
PP(int16_t handle;)
PP(int16_t *cur_form;)
{
	i_intin(cur_form);

	contrl[0] = 111;
	contrl[1] = 0;
	contrl[3] = 37;
	contrl[6] = handle;
	vdi();

	i_intin(intin);
}
