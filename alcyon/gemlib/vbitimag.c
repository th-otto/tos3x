#include "vdilib.h"

VOID v_bit_image(P(int16_t) handle, P(const char *) filename, P(int16_t) aspect, P(int16_t) xscale, P(int16_t) yscale, P(int16_t) halign, P(int16_t) valign, P(int16_t *) xy)
PP(int16_t handle;)
PP(const char *filename;)
PP(int16_t aspect;)
PP(int16_t xscale;)
PP(int16_t yscale;)
PP(int16_t halign;)
PP(int16_t valign;)
PP(int16_t *xy;)
{
	int16_t i;

	for (i = 0; i < 4; i++)
		ptsin[i] = xy[i];
	intin[0] = aspect;
	intin[1] = xscale;
	intin[2] = yscale;
	intin[3] = halign;
	intin[4] = valign;
	i = 5;
	do 
		;
	while ((intin[i++] = *filename++ & 0xff) != 0);

	contrl[0] = 5;
	contrl[1] = 2;
	contrl[3] = --i;
	contrl[5] = 23;
	contrl[6] = handle;
	vdi();
}
