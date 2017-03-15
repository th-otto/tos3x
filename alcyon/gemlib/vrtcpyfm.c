#include "vdilib.h"

VOID vrt_cpyfm(P(int16_t) handle, P(int16_t) wr_mode, P(int16_t *)xy, P(MFDB *) srcMFDB, P(MFDB *) desMFDB, P(int16_t *) index)
PP(int16_t handle;)
PP(int16_t wr_mode;)
PP(int16_t *xy;)
PP(MFDB *srcMFDB;)
PP(MFDB *desMFDB;)
PP(int16_t *index;)
{
	VOIDPTR *t;
	
	intin[0] = wr_mode;
	intin[1] = *index++;
	intin[2] = *index;
	i_ptr(srcMFDB);
	i_ptr2(desMFDB);
	i_ptsin(xy);

	contrl[0] = 121;
	contrl[1] = 4;
	contrl[3] = 3;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
