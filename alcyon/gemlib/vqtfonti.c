#include "vdilib.h"

VOID vqt_font_info(P(int16_t) handle, P(int16_t *) minADE, P(int16_t *) maxADE, P(int16_t *) distances, P(int16_t *) maxwidth, P(int16_t *) effects)
PP(int16_t handle;)
PP(int16_t *minADE;)
PP(int16_t *maxADE;)
PP(int16_t *distances;)
PP(int16_t *maxwidth;)
PP(int16_t *effects;)
{
	contrl[0] = 131;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*minADE = intout[0];
	*maxADE = intout[1];
	*maxwidth = ptsout[0];
	distances[0] = ptsout[1];
	distances[1] = ptsout[3];
	distances[2] = ptsout[5];
	distances[3] = ptsout[7];
	distances[4] = ptsout[9];
	effects[0] = ptsout[2];
	effects[1] = ptsout[4];
	effects[2] = ptsout[6];
}
