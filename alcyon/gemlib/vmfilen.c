#include "vdilib.h"

VOID vm_filename(P(int16_t) handle, P(const char *) filename)
PP(int16_t handle;)
PP(const char *filename;)
{
	int16_t *intstr;

	intstr = intin;
	do
	{
	} while ((*intstr++ = *filename++ & 0xff) != 0);
	
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = ((int) ((intptr_t)intstr - (intptr_t)intin) >> 1) - 1;
	contrl[5] = 100;
	contrl[6] = handle;
	vdi();
}
