#include "vdilib.h"

VOID v_opnvwk(P(int16_t *) work_in, P(int16_t *) handle, P(int16_t *) work_out)
PP(int16_t *work_in;)
PP(int16_t *handle;)
PP(int16_t *work_out;)
{
	i_intin(work_in);
	i_intout(work_out);
	i_ptsout(work_out + 45);

	contrl[0] = 100;
	contrl[1] = 0;
	contrl[3] = 11;
	contrl[6] = *handle;
	vdi();

	*handle = contrl[6];
	i_intin(intin);
	i_intout(intout);
	i_ptsout(ptsout);
	i_ptsin(ptsin);						/* must set in 68k land so we can ROM it */
}
