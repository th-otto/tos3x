#include "aeslib.h"

int16_t menu_popup(P(MENU *) menu, P(int16_t) xpos, P(int16_t) ypos, P(MENU *) mdata)
PP(MENU *menu;)
PP(int16_t xpos;)
PP(int16_t ypos;)
PP(MENU *mdata;)
{
	M_MENU = menu;
	M_XPOS = xpos;
	M_YPOS = ypos;
	M_MDATA = mdata;

	return crys_if(MENU_POPUP);
}
