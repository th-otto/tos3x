#include "vdilib.h"

VOID v_justified(P(int16_t) handle, P(int16_t) x, P(int16_t) y, P(const char *) string, P(int16_t) length, P(int16_t) word_space, P(int16_t) char_space)
PP(int16_t handle;)
PP(int16_t x;)
PP(int16_t y;)
PP(const char *string;)
PP(int16_t length;)
PP(int16_t word_space;)
PP(int16_t char_space;)
{
	int16_t *intstr;

	ptsin[0] = x;
	ptsin[1] = y;
	ptsin[2] = length;
	ptsin[3] = 0;
	intin[0] = word_space;
	intin[1] = char_space;
	intstr = &intin[2];
	do 
		;
	while ((*intstr++ = *string++ & 0xff) != 0);

	contrl[0] = 11;
	contrl[1] = 2;
	contrl[3] = ((int) ((intptr_t)intstr - (intptr_t)intin) >> 1) - 1;
	contrl[5] = 10;
	contrl[6] = handle;
	vdi();
}
