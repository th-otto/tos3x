#include "vdilib.h"

VOID v_gtext(P(int16_t) handle, P(int16_t) x, P(int16_t) y, P(const char *) string)
PP(int16_t handle;)
PP(int16_t x;)
PP(int16_t y;)
PP(const char *string;)
{
	int16_t i;

	ptsin[0] = x;
	ptsin[1] = y;
	i = 0;
	do
	{
	} while ((intin[i++] = *string++ & 0xff) != 0);

	contrl[0] = 8;
	contrl[1] = 1;
	contrl[3] = --i;
	contrl[6] = handle;
	vdi();
}
