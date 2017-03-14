#include "aeslib.h"

int16_t appl_init(NOTHING)
{
	c.cb_pcontrol = &control[0];
	c.cb_pglobal = &global[0];
	c.cb_pintin = &int_in[0];
	c.cb_pintout = &int_out[0];
	c.cb_padrin = &addr_in[0];
	c.cb_padrout = &addr_out[0];

	ad_c = &c;
	crys_if(APPL_INIT);
	gl_apid = RET_CODE;
	return TRUE;
}
