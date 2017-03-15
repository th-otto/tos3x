#include "aeslib.h"

short appl_getinfo(P(short) type, P(short *) out1, P(short *) out2, P(short *) out3, P(short *) out4)
PP(short type;)
PP(short *out1;)
PP(short *out2;)
PP(short *out3;)
PP(short *out4;)
{
	int_in[0] = type;
	crys_if(APPL_GINFO);
	*out1 = int_out[1];
	*out2 = int_out[2];
	*out3 = int_out[3];
	*out4 = int_out[4];
	return RET_CODE;
}
