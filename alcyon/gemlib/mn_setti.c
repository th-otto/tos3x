#include "aeslib.h"

int16_t menu_settings(P(int16_t) flag, P(MN_SET *) values)
PP(int16_t flag;)
PP(MN_SET *values;)
{
	M_FLAG = flag;
	M_MENU = values;

	return crys_if(MENU_SETTINGS);
}
