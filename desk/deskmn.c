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


VOID mv_desk(NOTHING)
{
}


VOID mins_app(NOTHING)
{
}


VOID mdesk_pref(NOTHING)
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


VOID DoPopup(P(OBJECT *)tree, P(int16_t) button, P(int16_t) title, P(OBJECT *)Mtree, P(int16_t) Mmenu, P(int16_t) Mfirst, P(int16_t *) Mstart, P(int16_t) Mscroll, P(int16_t) FirstMenu, P(int16_t) FirstText, P(int16_t) Skip)
PP(OBJECT *tree;)
PP(int16_t button;)
PP(int16_t title;)
PP(OBJECT *Mtree;)
PP(int16_t Mmenu;)
PP(int16_t Mfirst;)
PP(int16_t *Mstart;)
PP(int16_t Mscroll;)
PP(int16_t FirstMenu;)
PP(int16_t FirstText;)
PP(int16_t Skip;)
{
}


VOID init_vtree(NOTHING)
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
