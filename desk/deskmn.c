#include "desktop.h"

#if !POPUP_SUPPORT /* whole file */

/* Mouse Rectangle Structure...*/
typedef struct _mrets
{
	int16_t x;
	int16_t y;
	int16_t buttons;
	int16_t kstate;
} MRETS;


VOID desk_pref(NOTHING)
{
}


BOOLEAN set_video(NOTHING)
{
	return FALSE;
}


VOID XSelect(P(OBJECT *)tree, P(int16_t) obj)
PP(OBJECT *tree;)
PP(int16_t obj;)
{
}


VOID XDeselect(P(OBJECT *) tree, P(int16_t) obj)
PP(OBJECT *tree;)
PP(int16_t obj;)
{
}


VOID wait_up(NOTHING)
{
	MRETS mk;

	do
	{
		graf_mkstate(&mk.x, &mk.y, &mk.buttons, &mk.kstate);
	} while (mk.buttons);
}


#endif /* POPUP_SUPPORT */
