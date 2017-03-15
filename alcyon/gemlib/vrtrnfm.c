#include "vdilib.h"

VOID vr_trnfm(P(int16_t) handle, P(MFDB *) srcMFDB, P(MFDB *) desMFDB)
PP(int16_t handle;)
PP(MFDB *srcMFDB;)
PP(MFDB *desMFDB;)
{
	VOIDPTR *t;
	
	i_ptr(srcMFDB);
	i_ptr2(desMFDB);

	contrl[0] = 110;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();
}
