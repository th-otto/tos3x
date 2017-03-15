#include "vdilib.h"

VOID vsf_udpat(P(int16_t) handle, P(int16_t *) fill_pat, P(int16_t) planes)
PP(int16_t handle;)
PP(int16_t *fill_pat;)
PP(int16_t planes;)
{
	i_intin(fill_pat);

	contrl[0] = 112;
	contrl[1] = 0;
	contrl[3] = 16 * planes;
	contrl[6] = handle;
	vdi();
	i_intin(intin);
}
