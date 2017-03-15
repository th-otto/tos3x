#include "vdilib.h"

VOID vrq_string(P(int16_t) handle, P(int16_t) length, P(int16_t) echo_mode, P(int16_t *) echo_xy, P(char *) string)
PP(int16_t handle;)
PP(int16_t length;)
PP(int16_t echo_mode;)
PP(int16_t *echo_xy;)
PP(char *string;)
{
	int16_t count;

	intin[0] = length;
	intin[1] = echo_mode;
	i_ptsin(echo_xy);

	contrl[0] = 31;
	contrl[1] = echo_mode;
	contrl[3] = 2;
	contrl[6] = handle;
	vdi();

	for (count = 0; count < contrl[4]; count++)
		*string++ = intout[count];
	*string = 0;
	i_ptsin(ptsin);
}
