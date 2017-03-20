#include "aeslib.h"

int16_t control[C_SIZE];
int16_t global[G_SIZE];
int16_t int_in[I_SIZE];
int16_t int_out[O_SIZE];
VOIDPTR addr_in[AI_SIZE];
VOIDPTR addr_out[AO_SIZE];
int16_t gl_apid;

int16_t crys_if(P(int16_t) opcode)
PP(int16_t opcode;)
{
	register const char *pctrl;
	register int16_t *pc = control;
	
	*pc++ = opcode;

	pctrl = &ctrl_cnts[(opcode - 10) * 3];
	*pc++ = *pctrl++;
	*pc++ = *pctrl++;
	*pc = *pctrl;

	crystal();
	return RET_CODE;
}
