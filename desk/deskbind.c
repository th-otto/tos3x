/*	DESKBIND.C		6/16/89	- 9/14/89	Derek Mui	*/

/************************************************************************/
/*	New Desktop for Atari ST/TT Computer				*/
/*	Atari Corp							*/
/*	Copyright 1989,1990 	All Rights Reserved			*/
/************************************************************************/


#include <portab.h>
#include <struct88.h>

extern int16_t gl_wchar;

extern int16_t gl_hchar;

extern int16_t gl_wbox;

extern int16_t gl_hbox;

extern int16_t gl_handle;

extern int16_t pglobal[];

extern int16_t contrl[];

extern int16_t intin[];

extern int16_t ptsin[];

extern PD *rlr;


wind_new()
{
	wm_new();
	dsptch();
}


int16_t fsel_exinput(path, selec, button, label)
char *path,
*selec,
*label;

int16_t *button;
{
	int16_t ret;

	ret = fs_input(path, selec, button, label);
	dsptch();
	return (ret);
}


int16_t rsrc_load(name)
char *name;
{
	int16_t ret;

	ret = rs_load(pglobal, name);
	dsptch();
	return (ret);
}

int16_t rsrc_obfix(tree, obj)
int32_t tree;

int16_t obj;
{
	int16_t ret;

	ret = rs_obfix(tree, obj);
	dsptch();
	return (ret);
}


int16_t menu_popup(menu, x, y, mdata)
int32_t menu,
	mdata;

int16_t x,
	y;
{
	int16_t ret;

	ret = mn_popup(rlr->p_pid, menu, x, y, mdata);
	dsptch();
	return (ret);
}


int16_t menu_istart(code, mtree, mmenu, start)
int16_t code,
	mmenu,
	start;

int32_t mtree;
{
	int16_t ret;

	ret = mn_istart(rlr->p_pid, code, mtree, mmenu, start);
	dsptch();
	return (ret);
}


objc_gclip(tree, which, x, y, rx, ry, w, h)
int32_t tree;

int16_t which;

int16_t *x,
*y,
*rx,
*ry,
*w,
*h;
{
	ob_gclip(tree, which, x, y, rx, ry, w, h);
	dsptch();
}


graf_mouse(style, pointer)
int16_t style;

char *pointer;
{
	gr_mouse(style, pointer);
	dsptch();
}

/*	show cursor	*/

v_show_c(reset)
int16_t reset;
{
	intin[0] = reset;
	gsx_ncode(122, 0, 1);
}

/*	hide cursor	*/

v_hide_c()
{
	gsx_ncode(123, 0, 0);
}


/*	enter graphic mode	*/

v_exit_cur()
{
	contrl[5] = 2;
	gsx_ncode(5, 0, 0);
}

/*	enter alpha mode	*/

v_enter_cur()
{
	contrl[5] = 3;
	gsx_ncode(5, 0, 0);
}

/*	clipping function	*/

vs_clip(clip_flag, pxyarray)
int16_t clip_flag;

int16_t pxyarray[];
{
	intin[0] = clip_flag;
	ptsin[0] = pxyarray[0];
	ptsin[1] = pxyarray[1];
	ptsin[2] = pxyarray[2];
	ptsin[3] = pxyarray[3];
	gsx_ncode(129, 2, 1);
}
