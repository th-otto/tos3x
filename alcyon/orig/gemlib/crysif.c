#include "aeslib.h"

CBLK c;
int16_t control[C_SIZE];
int16_t global[G_SIZE];
int16_t int_in[I_SIZE];
int16_t int_out[O_SIZE];
VOIDPTR addr_in[AI_SIZE];
VOIDPTR addr_out[AO_SIZE];
int16_t gl_apid;
CBLK *ad_c;

int16_t crys_if(P(int16_t) opcode)
PP(int16_t opcode;)
{
	int i;
	const char *pctrl;
	control[0] = opcode;

	pctrl = &ctrl_cnts[(opcode - 10) * 3];
	for (i = 1; i < 4; i++)
		control[i] = *pctrl++;

	crystal(ad_c);
	return RET_CODE;
}
