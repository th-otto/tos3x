/*	CRYSIF	LIBRARY		04/05/85	Derek Mui	*/
#include <portab.h>
#include <aes.h>

#include  "menubind.h"

WORD _AesCtrl(WORD code);


#define EXTERN extern

#define _aes() ((WORD (*)(void))((char *)_AesCtrl + 38))()

/* Menu Manager         */
WORD menu_popup(MENU *Menu, WORD xpos, WORD ypos, MENU *MData)
{
	_GemParBlk.addrin[0] = Menu;
	_GemParBlk.intin[0] = xpos;
	_GemParBlk.intin[1] = ypos;
	_GemParBlk.addrin[1] = MData;

	_GemParBlk.contrl[0] = 36;
	_GemParBlk.contrl[1] = 2;
	_GemParBlk.contrl[2] = 1;
	_GemParBlk.contrl[3] = 2;
	return _aes();
}



WORD menu_attach(WORD flag, OBJECT *tree, WORD item, MENU *Menu)
{
	_GemParBlk.intin[0] = flag;
	_GemParBlk.addrin[0] = tree;
	_GemParBlk.intin[1] = item;
	_GemParBlk.addrin[0] = Menu;

	_GemParBlk.contrl[0] = 37;
	_GemParBlk.contrl[1] = 2;
	_GemParBlk.contrl[2] = 1;
	_GemParBlk.contrl[3] = 2;
	return _aes();
}



WORD menu_istart(WORD flag, OBJECT *tree, WORD menu, WORD item)
{
	_GemParBlk.intin[0] = flag;
	_GemParBlk.addrin[0] = tree;
	_GemParBlk.intin[1] = menu;
	_GemParBlk.intin[2] = item;

	_GemParBlk.contrl[0] = 38;
	_GemParBlk.contrl[1] = 3;
	_GemParBlk.contrl[2] = 1;
	_GemParBlk.contrl[3] = 1;
	return _aes();
}



WORD menu_settings(WORD flag, MN_SET *Values)
{
	_GemParBlk.intin[0] = flag;
	_GemParBlk.addrin[0] = Values;

	_GemParBlk.contrl[0] = 39;
	_GemParBlk.contrl[1] = 1;
	_GemParBlk.contrl[2] = 1;
	_GemParBlk.contrl[3] = 1;
	return _aes();
}
