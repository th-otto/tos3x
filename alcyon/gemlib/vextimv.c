#include "vdilib.h"

VOID vex_timv(P(int16_t) handle, P(VOIDPTR) usercode, P(VOIDPTR *) savecode, P(int16_t *) scale)
PP(int16_t handle;)
PP(VOIDPTR usercode;)
PP(VOIDPTR *savecode;)
PP(int16_t *scale;)
{
	VOIDPTR *t;
	
	i_ptr(usercode);

	contrl[0] = 118;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	m_lptr2(savecode);
	*scale = intout[0];
}
