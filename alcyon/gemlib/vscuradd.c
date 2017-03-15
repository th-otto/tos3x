#include "vdilib.h"

VOID vs_curaddress(P(int16_t) handle, P(int16_t) row, P(int16_t) column)
PP(int16_t handle;)
PP(int16_t row;)
PP(int16_t column;)
{
	intin[0] = row;
	intin[1] = column;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 11;
	contrl[6] = handle;
	vdi();
}
