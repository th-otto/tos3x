#include "vdilib.h"

VOID vq_extnd(P(int16_t) handle, P(int16_t) owflag, P(int16_t *) work_out)
PP(int16_t handle;)
PP(int16_t owflag;)
PP(int16_t *work_out;)
{
	i_intin(intin);						/* must set in 68k land so we can ROM it */
	i_ptsin(ptsin);						/* since bss can't have initialized data */

	i_intout(work_out);
	i_ptsout(work_out + 45);
	intin[0] = owflag;

	contrl[0] = 102;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	i_intout(intout);
	i_ptsout(ptsout);
}
