/*      DESKBIND.C              6/16/89 - 9/14/89       Derek Mui       */

/************************************************************************/
/*      New Desktop for Atari ST/TT Computer                            */
/*      Atari Corp                                                      */
/*      Copyright 1989,1990     All Rights Reserved                     */
/************************************************************************/


#include "desktop.h"
#include "aesbind.h"



/* 306de: 00e28052 */
VOID wind_new(NOTHING)
{
	wm_new();
	dsptch();
}


/* 306de: 00e28066 */
int16_t fsel_exinput(P(char *)path, P(char *)selec, P(int16_t *)button, P(const char *)label)
PP(char *path;)
PP(char *selec;)
PP(int16_t *button;)
PP(const char *label;)
{
	int16_t ret;

	ret = fs_input(path, selec, button, label);
	dsptch();
	return ret;
}


/* 306de: 00e28098 */
int16_t rsrc_load(P(const char *) name)
PP(const char *name;)
{
	int16_t ret;

	ret = rs_load((intptr_t) pglobal, name);
	dsptch();
	return ret;
}


#if POPUP_SUPPORT
int16_t rsrc_obfix(P(LPTREE) tree, P(int16_t) obj)
PP(LPTREE tree;)
PP(int16_t obj;)
{
	int16_t ret;

	ret = rs_obfix(tree, obj);
	dsptch();
	return ret;
}


int16_t menu_popup(P(MENU *)menu, P(int16_t) x, P(int16_t) y, P(MENU *)mdata)
PP(MENU *menu;)
PP(int16_t x;)
PP(int16_t y;)
PP(MENU *mdata;)
{
	int16_t ret;

	ret = mn_popup(rlr->p_pid, menu, x, y, mdata);
	dsptch();
	return ret;
}


int16_t menu_istart(P(int16_t) code, P(OBJECT *)mtree, P(int16_t) mmenu, P(int16_t) start)
PP(int16_t code;)
PP(OBJECT *mtree;)
PP(int16_t mmenu;)
PP(int16_t start;)
{
	int16_t ret;

	ret = mn_istart(rlr->p_pid, code, mtree, mmenu, start);
	dsptch();
	return ret;
}
#endif


#if AES3D
VOID objc_gclip(P(LPTREE) tree, P(int16_t) which, P(int16_t *)x, P(int16_t *)y, P(int16_t *)rx, P(int16_t *)ry, P(int16_t *)w, P(int16_t *)h)
PP(LPTREE tree;)
PP(int16_t which;)
PP(int16_t *x;)
PP(int16_t *y;)
PP(int16_t *rx;)
PP(int16_t *ry;)
PP(int16_t *w;)
PP(int16_t *h;)
{
	ob_gclip(tree, which, x, y, rx, ry, w, h);
	dsptch();
}
#endif


/* 306de: 00e280c0 */
VOID graf_mouse(P(int16_t) style, P(MFORM *)grmaddr)
PP(int16_t style;)
PP(MFORM *grmaddr;)
{
	gr_mouse(style, grmaddr);
	dsptch();
}


/*
 * show cursor
 */
/* 306de: 00e280de */
VOID v_show_c(P(BOOLEAN) reset)
PP(int16_t reset;)
{
	intin[0] = reset;
	gsx_ncode(122, 0, 1);
}


/*
 * hide cursor
 */
/* 306de: 00e28100 */
VOID v_hide_c(NOTHING)
{
	gsx_ncode(123, 0, 0);
}


/*
 * enter graphic mode
 */
/* 306de: 00e28118 */
VOID v_exit_cur(NOTHING)
{
	contrl[5] = 2;
	gsx_ncode(5, 0, 0);
}


/*
 * enter alpha mode
 */
/* 306de: 00e28138 */
VOID v_enter_cur(NOTHING)
{
	contrl[5] = 3;
	gsx_ncode(5, 0, 0);
}


/*
 * clipping function
 */
/* 306de: 00e28158 */
VOID vs_clip(P(BOOLEAN) clip_flag, P(const int16_t *) pxy)
PP(BOOLEAN clip_flag;)
PP(int16_t *pxy;)
{
	intin[0] = clip_flag;
	ptsin[0] = pxy[0];
	ptsin[1] = pxy[1];
	ptsin[2] = pxy[2];
	ptsin[3] = pxy[3];
	gsx_ncode(129, 2, 1);
}


#if !BINEXACT
VOID vq_chcells(int16_t *rows, int16_t *cols)
{
	contrl[5] = 1;
	gsx_ncode(5, 0, 0);
	*rows = intout[0];
	*cols = intout[1];
}
#endif
