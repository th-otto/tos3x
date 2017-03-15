#include "vdilib.h"

VOID vex_butv(P(int16_t) handle, P(VOIDPTR) usercode, P(VOIDPTR *) savecode)
PP(int16_t handle;)
PP(VOIDPTR usercode;)
PP(VOIDPTR *savecode;)
{
	i_ptr(usercode);

	contrl[0] = 125;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	m_lptr2(savecode);
}
