#include "vdilib.h"

VOID vro_cpyfm(P(int16_t) handle, P(int16_t) wr_mode, P(int16_t *) xy, P(MFDB *) srcMFDB, P(MFDB *) desMFDB)
PP(int16_t handle;)
PP(int16_t wr_mode;)
PP(int16_t *xy;)
PP(MFDB *srcMFDB;)
PP(MFDB *desMFDB;)
{
	VOIDPTR *t;
	
	intin[0] = wr_mode;
	i_ptr(srcMFDB);
	i_ptr2(desMFDB);
	i_ptsin(xy);

	contrl[0] = 109;
	contrl[1] = 4;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
