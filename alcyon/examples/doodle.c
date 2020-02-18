/************************************************************************/
/*      File:   doodle.c                                                */
/************************************************************************/
/*                                                                      */
/*                   GGGGG        EEEEEEEE     MM      MM               */
/*                 GG             EE           MMMM  MMMM               */
/*                 GG   GGG       EEEEE        MM  MM  MM               */
/*                 GG   GG        EE           MM      MM               */
/*                   GGGGG        EEEEEEEE     MM      MM               */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/*                        +--------------------------+                  */
/*                        | Digital Research, Inc.   |                  */
/*                        | 60 Garden Court          |                  */
/*                        | Monterey, CA.     93940  |                  */
/*                        +--------------------------+                  */
/*                                                                      */
/*   The  source code  contained in  this listing is a non-copyrighted  */
/*   work which  can be  freely used.  In applications of  this source  */
/*   code you  are requested to  acknowledge Digital Research, Inc. as  */
/*   the originator of this code.                                       */
/*                                                                      */
/*   Author:    Tom Rolander, Tim Oren                                  */
/*   PRODUCT:   GEM Sample Application                                  */
/*   Module:    DOODLE                                                  */
/*   Modified:  Version 2.1, Mitch Smith,    April 25, 1986             */
/*   Modified:  Version 1.2, September 17, 1985                         */
/*   Original:  Version 1.1, March 22, 1985                             */
/*                                                                      */
/*   Revised: Mike Fulton, Atari Corp.  Nov. 1, 1991 (to fix problems   */
/*              with revised compiler header files)                     */
/*                                                                      */
/************************************************************************/

/*------------------------------*/
/*      includes                */
/*------------------------------*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <obdefs.h>						/* object definitions   */
#include <gemdefs.h>					/* gem binding structs  */
#include <gembind.h>					/* gem binding structs  */
#include <vdibind.h>
#include <osbind.h>
#include <fcntl.h>
#include "doodle.h"						/* doodle apl resource  */

/*------------------------------*/
/*      defines                 */
/*------------------------------*/

#define PEN_INK         BLACK
#define PEN_ERASER      WHITE

#define PEN_FINE        1
#define PEN_MEDIUM      5
#define PEN_BROAD       9

#define X_FWD           0x0100			/* extended object types */
#define X_BAK           0x0200			/* used with scrolling   */
#define X_SEL           0x0300			/* selectors             */
#define N_COLORS        15L
#define YSCALE(x) umul_div(x, scrn_xsize, scrn_ysize)

typedef int BOOLEAN;
#define FALSE 0
#define TRUE  1

/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Data Structures                          ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/

/*------------------------------*/
/*      Local Data Structures   */
/*------------------------------*/

short gl_wchar;							/* character width              */
short gl_hchar;							/* character height             */
short gl_wbox;							/* box (cell) width             */
short gl_hbox;							/* box (cell) height            */
short gl_hspace;						/* height of space between lines */
short gem_handle;						/* GEM vdi handle               */
short vdi_handle;						/* doodle vdi handle            */
short work_out[57];						/* open virt workstation values */
GRECT scrn_area;						/* whole screen area            */
GRECT work_area;						/* drawing area of main window  */
GRECT undo_area;						/* area equal to work_area      */
GRECT save_area;						/* save area for full/unfulling */
short gl_rmsg[8];						/* message buffer               */
OBJECT *gl_menu;						/* menu tree address            */
short gl_xfull;							/* full window 'x'              */
short gl_yfull;							/* full window 'y'              */
short gl_wfull;							/* full window 'w'              */
short gl_hfull;							/* full window 'h'              */
short scrn_width;						/* screen width in pixels       */
short scrn_height;						/* screen height in pixels      */
short scrn_planes;						/* number of color planes       */
short scrn_xsize;						/* width of one pixel           */
short scrn_ysize;						/* height of one pixel          */
unsigned short m_out = FALSE;			/* mouse in/out of window flag  */
MFDB undo_mfdb;							/* undo buffer mmry frm def blk */
MFDB scrn_mfdb;							/* screen memory form defn blk  */
long buff_size;							/* buffer size req'd for screen */
char *buff_location;					/* screen buffer pointer        */
short dood_whndl;						/* doodle window handle         */
BOOLEAN dood_fill = FALSE;				/* fill or draw flag            */
short dood_shade = PEN_INK;				/* doodle current pen shade     */
short pen_ink = PEN_INK;				/* current ink shade            */
short bkgr_color = WHITE;				/* current background shade     */
short bkgr_next = WHITE;				/* next background shade        */
short dood_pen = 1;						/* doodle current pen width     */
short dood_height = 13;					/* doodle current char height   */
short char_fine;						/* character height for fine    */
short char_medium;						/* character height for medium  */
short char_broad;						/* character height for broad   */
short monumber = 5;						/* mouse form number            */
VOIDPTR mofaddr = NULL;					/* mouse form address           */
short file_handle;						/* file handle -> pict ld/sv    */
char file_name[64] = "";				/* current pict file name       */
BOOLEAN key_input;						/* key inputting state          */
short key_xbeg;							/* x position for line beginning */
short key_ybeg;							/* y position for line beginning */
short key_xcurr;						/* current x position           */
short key_ycurr;						/* current y position           */

										/* doodle window title          */
char *wdw_title = " Dr Doodle Window ";

short usercolor[2] = { 1, 0 };

MFDB userbrush_mfdb;					/* MFDB for Prog def objects  */

USERBLK brushub[6];						/* 6 Programmer defined objects  */

#ifdef GEM3
X_BUF_V2 gl_xbuf;
#endif

long color_sel[N_COLORS + 1] = {		/* data for scrolling color selector */
	N_COLORS,
/*
      ++----------------------------- Letter
      ||++--------------------------- Border width
      ||||+-------------------------- Border colour
      |||||+------------------------- Background
      ||||||+------------------------ Background shade (bits 4-6)
      |||||||+----------------------- Foreground
      ||||||||
*/	0x31FF1071L,
	0x32FF1072L,
	0x33FF1073L,
	0x34FF1074L,
	0x35FF1075L,
	0x36FF1076L,
	0x37FF1077L,
	0x38FF1078L,
	0x39FF1079L,
	0x41FF107AL,
	0x42FF107BL,
	0x43FF107CL,
	0x44FF107DL,
	0x45FF107EL,
	0x46FF107FL
};

/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Local Procedures                         ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      string_addr             */
/*------------------------------*/
/* returns a tedinfo string addr  */
static char *string_addr(P(short) which)
PP(short which;)
{
	char *where;

	rsrc_gaddr(R_STRING, which, &where);
	return where;
}


/*------------------------------*/
/*      bit_addr                */
/*------------------------------*/
/* returns a free image addr  */
static BITBLK *bit_addr(P(short) which)
PP(short which;)
{
	BITBLK **bit;
	
	rsrc_gaddr(R_FRIMG, which, &bit);
	return *bit;
}

static MFORM *mform_addr(P(short) which)
PP(short which;)
{
	BITBLK *bit;
	
	bit = bit_addr(which);
	return (MFORM *)(bit->bi_pdata);
}


/*------------------------------*/
/*      inside                  */
/*------------------------------*/
/* determine if x,y is in rectangle     */
static BOOLEAN inside(P(short) x, P(short) y, P(const GRECT *) pt)
PP(short x;)
PP(short y;)
PP(const GRECT *pt;)
{
	if ((x >= pt->g_x) && (y >= pt->g_y) && (x < pt->g_x + pt->g_w) && (y < pt->g_y + pt->g_h))
		return TRUE;
	return FALSE;
}

/*------------------------------*/
/*      grect_to_array          */
/*------------------------------*/
/* convert x,y,w,h to upper left x,y and lower right x,y for raster ops and for clipping */
static VOID grect_to_array(P(const GRECT *) area, P(short *) array)
PP(GRECT *area;)
PP(short *array;)
{
	*array++ = area->g_x;
	*array++ = area->g_y;
	*array++ = area->g_x + area->g_w - 1;
	*array = area->g_y + area->g_h - 1;
}


/*------------------------------*/
/*      rast_op                 */
/*------------------------------*/
/* bit block level trns */
static VOID rast_op(P(short) mode, P(GRECT *) s_area, P(MFDB *) s_mfdb, P(GRECT *) d_area, P(MFDB *) d_mfdb)
PP(short mode;)
PP(GRECT *s_area;)
PP(MFDB *s_mfdb;)
PP(GRECT *d_area;)
PP(MFDB *d_mfdb;)
{
	short pxy[8];

	grect_to_array(s_area, pxy);
	grect_to_array(d_area, &pxy[4]);
	vro_cpyfm(vdi_handle, mode, pxy, s_mfdb, d_mfdb);
}

/*------------------------------*/
/*      vdi_fix                 */
/*------------------------------*/
/* set up MFDB for transform     */
static VOID vdi_fix(P(MFDB *) pfd, P(VOIDPTR) theaddr, P(short) wb, P(short) h)
PP(MFDB *pfd;)
PP(VOIDPTR theaddr;)
PP(short wb;)
PP(short h;)
{
	pfd->fd_wdwidth = wb >> 1;			/* # of bytes to words     */
	pfd->fd_w = wb << 3;				/* # of bytes to to pixels    */
	pfd->fd_h = h;						/* height in scan lines    */
	pfd->fd_nplanes = 1;				/* number of planes     */
	pfd->fd_addr = theaddr;				/* memory pointer    */
}

/*------------------------------*/
/*      vdi_trans               */
/*------------------------------*/
static VOID vdi_trans(P(VOIDPTR) saddr, P(short) swb, P(VOIDPTR) daddr, P(short) dwb, P(short) h)
PP(VOIDPTR saddr;)
PP(short swb;)
PP(VOIDPTR daddr;)
PP(short dwb;)
PP(short h;)
{
	MFDB src, dst;

	vdi_fix(&src, saddr, swb, h);
	src.fd_stand = TRUE;

	vdi_fix(&dst, daddr, dwb, h);
	dst.fd_stand = FALSE;
	/* transform to device specific format     */
	vr_trnfm(vdi_handle, &src, &dst);
}

/*------------------------------*/
/*      trans_gimage            */
/*------------------------------*/
static VOID trans_gimage(P(OBJECT *) tree, P(short) obj)
PP(OBJECT *tree;)
PP(short obj;)
{
	VOIDPTR taddr;
	BITBLK *obspec;
	short wb, hl;

	obspec = (BITBLK *)tree[obj].ob_spec;
	taddr = obspec->bi_pdata;
	wb = obspec->bi_wb;
	hl = obspec->bi_hl;
	vdi_trans(taddr, wb, taddr, wb, hl);
}

/*------------------------------*/
/*      do_open                 */
/*------------------------------*/
/* grow and open specified wdw  */
static VOID do_open(P(short) wh, P(short) x, P(short) y, P(short) w, P(short) h)
PP(short wh;)
PP(short x;)
PP(short y;)
PP(short w;)
PP(short h;)
{
	graf_growbox(gl_xfull + gl_wfull / 2, gl_yfull + gl_hfull / 2, 21, 21, x, y, w, h);
	wind_open(wh, x, y, w, h);
}

/*------------------------------*/
/*      do_close                */
/*------------------------------*/
/* close and shrink specified window */
static VOID do_close(P(short) wh)
PP(short wh;)
{
	short x, y, w, h;

	wind_get(wh, WF_CURRXYWH, &x, &y, &w, &h);
	wind_close(wh);
	graf_shrinkbox(gl_xfull + gl_wfull / 2, gl_yfull + gl_hfull / 2, 21, 21, x, y, w, h);
}

/*------------------------------*/
/*      set_clip                */
/*------------------------------*/
static VOID set_clip(P(short) clip_flag, P(GRECT *)s_area)		/* set clip to specified area   */
PP(short clip_flag;)
PP(GRECT *s_area;)
{
	short pxy[4];

	grect_to_array(s_area, pxy);
	vs_clip(vdi_handle, clip_flag, pxy);
}

/*------------------------------*/
/*      draw_rect               */
/*------------------------------*/
/* used by dr_code() to draw a rectangle around pen/eraser */
static VOID draw_rect(P(GRECT *) area)
PP(GRECT *area;)
{
	short pxy[10];

	pxy[0] = area->g_x;
	pxy[1] = area->g_y;
	pxy[2] = area->g_x + area->g_w - 1;
	pxy[3] = area->g_y + area->g_h - 1;
	pxy[4] = pxy[2];
	pxy[5] = pxy[3];
	pxy[3] = pxy[1];
	pxy[6] = pxy[0];
	pxy[7] = pxy[5];
	pxy[8] = pxy[0];
	pxy[9] = pxy[1];
	v_pline(vdi_handle, 5, pxy);
}

/*------------------------------*/
/*      align_x                 */
/*------------------------------*/
/* forces word alignment for column position rounding to nearest word */
static short align_x(P(short) x)
PP(short x;)
{
#if 0
	return ((x & 0xfff0) + ((x & 0x000c) ? 0x0010 : 0));
#else
	return x;
#endif
}

/*------------------------------*/
/* do_top                       */
/*------------------------------*/
/* top the window if not already active */
static VOID do_top(P(short) wdw_hndl)
PP(short wdw_hndl;)
{
	short active, dummy;

	wind_get(wdw_hndl, WF_TOP, &active, &dummy, &dummy, &dummy);
	if (wdw_hndl != active)
		wind_set(wdw_hndl, WF_TOP, 0, 0, 0, 0);
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Object Tree Manipulation                 ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      do_obj                  */
/*------------------------------*/
/* set specified bit in object state    */
static VOID do_obj(P(OBJECT *) tree, P(short) which, P(short) bit)
PP(OBJECT *tree;)
PP(short which;)
PP(short bit;)
{
	tree[which].ob_state |= bit;
}


/*------------------------------*/
/*      undo_obj                */
/*------------------------------*/
/* clear specified bit in object state  */
static VOID undo_obj(P(OBJECT *) tree, P(short) which, P(short) bit)
PP(OBJECT *tree;)
PP(short which;)
PP(short bit;)
{
	tree[which].ob_state &= ~bit;
}

/*------------------------------*/
/*      sel_obj                 */
/*------------------------------*/
/* turn on selected bit of spcfd object */
static VOID sel_obj(P(OBJECT *) tree, P(short) which)
PP(OBJECT *tree;)
PP(short which;)
{
	do_obj(tree, which, SELECTED);
}

/*------------------------------*/
/*      desel_obj               */
/*------------------------------*/
/* turn off selected bit of spcfd object */
static VOID desel_obj(P(OBJECT *) tree, P(short) which)
PP(OBJECT *tree;)
PP(short which;)
{
	undo_obj(tree, which, SELECTED);
}

/*------------------------------*/
/*      enab_menu               */
/*------------------------------*/
/* enable specified menu item   */
static VOID enab_menu(P(short) which)
PP(short which;)
{
	undo_obj(gl_menu, which, DISABLED);
}

/*------------------------------*/
/*      disab_menu              */
/*------------------------------*/
/* disable specified menu item      */
static VOID disab_menu(P(short) which)
PP(short which;)
{
	do_obj(gl_menu, which, DISABLED);
}

/*------------------------------*/
/*      unflag_obj              */
/*------------------------------*/
static VOID unflag_obj(P(OBJECT *) tree, P(short) which, P(short) bit)
PP(OBJECT *tree;)
PP(short which;)
PP(short bit;)
{
	tree[which].ob_flags &= ~bit;
}

/*------------------------------*/
/*      flag_obj                */
/*------------------------------*/
static VOID flag_obj(P(OBJECT *) tree, P(short) which, P(short) bit)
PP(OBJECT *tree;)
PP(short which;)
PP(short bit;)
{
	tree[which].ob_flags |= bit;
}

/*------------------------------*/
/*      indir_obj               */
/*------------------------------*/
static VOID indir_obj(P(OBJECT *) tree, P(short) which)
PP(OBJECT *tree;)
PP(short which;)
{
	flag_obj(tree, which, INDIRECT);
}

/*------------------------------*/
/*      dir_obj                 */
/*------------------------------*/
static VOID dir_obj(P(OBJECT *) tree, P(short) which)
PP(OBJECT *tree;)
PP(short which;)
{
	unflag_obj(tree, which, INDIRECT);
}

/*------------------------------*/
/*      get_parent              */
/*------------------------------*/
/*
 *       Routine that will find the parent of a given object.  The
 *       idea is to walk to the end of our siblings and return
 *       our parent.  If object is the root then return NIL as parent.
 */
static short get_parent(P(OBJECT *) tree, P(short) obj)
PP(OBJECT *tree;)
PP(short obj;)
{
	short pobj;

	if (obj == NIL)
		return NIL;
	pobj = tree[obj].ob_next;
	if (pobj != NIL)
	{
		while (tree[pobj].ob_tail != obj)
		{
			obj = pobj;
			pobj = tree[obj].ob_next;
		}
	}
	return pobj;
}

/*------------------------------*/
/*      objc_xywh               */
/*------------------------------*/
/* get x,y,w,h for specified object     */
static VOID objc_xywh(P(OBJECT *) tree, P(short) obj, P(GRECT *) p)
PP(OBJECT *tree;)
PP(short obj;)
PP(GRECT *p;)
{
	objc_offset(tree, obj, &p->g_x, &p->g_y);
	p->g_w = tree[obj].ob_width;
	p->g_h = tree[obj].ob_height;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                   Advanced Dialog Handling                     ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/

struct xbind {
	long orig_obspec;
	long *arry;
};
	
/*------------------------------*/
/*      set_select              */
/*------------------------------*/
static VOID set_select(P(OBJECT *) tree, P(short) obj, P(short) sel, P(short) init_no, P(struct xbind *) bind, P(long *) arry)
PP(OBJECT *tree;)
PP(short obj;)
PP(short sel;)
PP(short init_no;)
PP(struct xbind *bind;)
PP(long *arry;)
{
	short n, cobj, count;

	indir_obj(tree, obj);
	bind->orig_obspec = tree[obj].ob_spec;
	tree[obj].ob_spec = (long)bind;
	bind->arry = arry;

	n = (short) arry[0];
	count = 0;
	for (cobj = tree[obj].ob_head; cobj != obj; cobj = tree[cobj].ob_next)
	{									/* loop to init color selector with colors 1 to 4  */
		indir_obj(tree, cobj);
		tree[cobj].ob_spec = (long)&arry[count + 1];
		count = (count + 1) % n;
	}
	indir_obj(tree, sel);
	tree[sel].ob_spec = (long)&arry[1 + init_no % n];
}


/*------------------------------*/
/*      get_select              */
/*------------------------------*/
/* Get the Current pen color selection. Used after Pen/Eraser Dialog interaction */
static short get_select(P(OBJECT *) tree, P(short) obj, P(short) sel)
PP(OBJECT *tree;)
PP(short obj;)
PP(short sel;)
{
	short cobj;
	struct xbind *bind;
	long *arry;
	long *temp;

	bind = (struct xbind *)tree[obj].ob_spec;
	dir_obj(tree, obj);
	tree[obj].ob_spec = bind->orig_obspec;
	arry = bind->arry;

	for (cobj = tree[obj].ob_head; cobj != obj; cobj = tree[cobj].ob_next)
	{
		dir_obj(tree, cobj);
		temp = (long *)tree[cobj].ob_spec;
		tree[cobj].ob_spec = *temp;
	}

	dir_obj(tree, sel);
	temp = (long *)tree[sel].ob_spec;
	tree[sel].ob_spec = *temp;

	return (short) ((intptr_t)temp - (intptr_t)arry) / sizeof(long) - 1;
}

/*------------------------------*/
/*      redraw_do               */
/*------------------------------*/
/* Routine to draw a specified object  */
static VOID redraw_do(P(OBJECT *) tree, P(short) obj)
PP(OBJECT *tree;)
PP(short obj;)
{
	GRECT o;

	objc_xywh(tree, obj, &o);
	o.g_x -= 3;
	o.g_y -= 3;
	o.g_w += 6;
	o.g_h += 6;
	objc_draw(tree, ROOT, MAX_DEPTH, o.g_x, o.g_y, o.g_w, o.g_h);
}

/*------------------------------*/
/*      move_do                 */
/*------------------------------*/
/* routine to scroll the color selector in the Pen/Eraser Dialog  */
static VOID move_do(P(OBJECT *) tree, P(short) obj, P(short) inc)
PP(OBJECT *tree;)
PP(short obj;)
PP(short inc;)
{
	short cobj;
	long n;
	struct xbind *bind;
	long *arry;
	long *limit;
	long *obspec;

	obj = get_parent(tree, obj);
	obj = tree[obj].ob_next;
	bind = (struct xbind *)tree[obj].ob_spec;
	arry = bind->arry;
	n = arry[0];
	limit = arry + n;

	for (cobj = tree[obj].ob_head; cobj != obj; cobj = tree[cobj].ob_next)
	{
		obspec = (long *)tree[cobj].ob_spec;
		obspec += inc;
		while (obspec <= arry || obspec > limit)
			obspec += obspec > limit ? -n : n;
		tree[cobj].ob_spec = (long)obspec;
	}

	redraw_do(tree, obj);
}

/*------------------------------*/
/*      xtend_do                */
/*------------------------------*/
/* called by hndl_dial() if extended type object is the exit object   */
static BOOLEAN xtend_do(P(OBJECT *) tree, P(short) obj, P(short) xtype)
PP(OBJECT *tree;)
PP(short obj;)
PP(short xtype;)
{
	long obspec;

	switch (xtype)
	{
	case X_SEL:						/* selected color */
		obspec = tree[obj].ob_spec;
		obj = get_parent(tree, obj);
		obj = tree[obj].ob_next;
		tree[obj].ob_spec = obspec;
		redraw_do(tree, obj);
		break;
	case X_FWD:						/* forward arrow  */
		move_do(tree, obj, 1);
		redraw_do(tree, obj);
		break;
	case X_BAK:						/* backward arrow */
		move_do(tree, obj, -1);
		redraw_do(tree, obj);
		break;
	default:
		break;
	}
	return FALSE;
}

/*------------------------------*/
/*      hndl_dial               */
/*------------------------------*/
/* general purpose dialog handler. Provides for extended object type  */
static short hndl_dial(P(OBJECT *) tree, P(short) def, P(short) x, P(short) y, P(short) w, P(short) h)
PP(OBJECT *tree;)
PP(short def;)
PP(short x;)
PP(short y;)
PP(short w;)
PP(short h;)
{
	short xdial, ydial, wdial, hdial;
	short exitobj;
	short xtype;

	form_center(tree, &xdial, &ydial, &wdial, &hdial);
	form_dial(0, x, y, w, h, xdial, ydial, wdial, hdial);
	form_dial(1, x, y, w, h, xdial, ydial, wdial, hdial);
	objc_draw(tree, ROOT, MAX_DEPTH, xdial, ydial, wdial, hdial);

	for (;;)
	{
		exitobj = form_do(tree, def) & 0x7FFF;
		xtype = tree[exitobj].ob_type & 0xFF00;
		if (!xtype)
			break;
		if (xtend_do(tree, exitobj, xtype))
			break;
	}

	form_dial(2, x, y, w, h, xdial, ydial, wdial, hdial);
	form_dial(3, x, y, w, h, xdial, ydial, wdial, hdial);
	return exitobj;
}

/*------------------------------*/
/*      dr_code                 */
/*------------------------------*/
static __CDECL short dr_code(P(PARMBLK *)ppb)
PP(PARMBLK *ppb;)
{
	PARMBLK pb;
	short pxy[10], hl, wb;
	BITBLK *taddr;

	pb = *ppb;
	set_clip(TRUE, (GRECT *) & pb.pb_xc);

	taddr = (BITBLK *)pb.pb_parm;					/* original obspec         */
	/* point to data  */
	userbrush_mfdb.fd_addr = taddr->bi_pdata;
	/* height in scan lines */
	hl = taddr->bi_hl;
	/* width in bytes */
	wb = taddr->bi_wb;

	userbrush_mfdb.fd_w = wb << 3;
	userbrush_mfdb.fd_wdwidth = wb >> 1;
	userbrush_mfdb.fd_h = hl;
	userbrush_mfdb.fd_nplanes = 1;
	userbrush_mfdb.fd_stand = 0;

	pxy[0] = pxy[1] = 0;
	pxy[2] = (wb << 3) - 1;
	pxy[3] = hl - 1;
	pxy[4] = pb.pb_x;
	pxy[5] = pb.pb_y;
	pxy[6] = pxy[4] + pb.pb_w - 1;
	pxy[7] = pxy[5] + pb.pb_h - 1;

	/* Copy Raster Transparent */
	vrt_cpyfm(vdi_handle, 2, pxy, &userbrush_mfdb, &scrn_mfdb, usercolor);

	if ((pb.pb_currstate != pb.pb_prevstate) || (pb.pb_currstate & SELECTED))
	{
		if (pb.pb_currstate & SELECTED)
			vsl_color(vdi_handle, BLACK);
		else
			vsl_color(vdi_handle, WHITE);
		vsl_width(vdi_handle, 1);
		vsl_type(vdi_handle, FIS_SOLID);

		pb.pb_x--;
		pb.pb_y--;
		pb.pb_w++;
		pb.pb_h++;
		draw_rect((GRECT *) & pb.pb_x);
	}

	set_clip(TRUE, &work_area);

	return 0;							/*  always return 0 from prog def drawing code */
}

/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Work Area Management                     ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      set_work                */
/*------------------------------*/
/* update undo area, clamping to page edges, and updt sliders if req'd */
static VOID set_work(P(BOOLEAN) slider_update)
PP(BOOLEAN slider_update;)
{
	short i;

	wind_get(dood_whndl, WF_WORKXYWH, &work_area.g_x, &work_area.g_y, &work_area.g_w, &work_area.g_h);

	undo_area.g_w = work_area.g_w;
	undo_area.g_h = work_area.g_h;
	/* clamp work area to page edges */
	undo_area.g_x = align_x(undo_area.g_x);
	if ((i = undo_mfdb.fd_w - (undo_area.g_x + undo_area.g_w)) < 0)
		undo_area.g_x += i;
	if ((i = undo_mfdb.fd_h - (undo_area.g_y + undo_area.g_h)) < 0)
		undo_area.g_y += i;

	if (slider_update)
	{
		wind_set(dood_whndl, WF_HSLIDE, umul_div(undo_area.g_x, 1000, undo_mfdb.fd_w - undo_area.g_w), 0, 0, 0);
		wind_set(dood_whndl, WF_VSLIDE, umul_div(undo_area.g_y, 1000, undo_mfdb.fd_h - undo_area.g_h), 0, 0, 0);
		wind_set(dood_whndl, WF_HSLSIZE, umul_div(work_area.g_w, 1000, undo_mfdb.fd_w), 0, 0, 0);
		wind_set(dood_whndl, WF_VSLSIZE, umul_div(work_area.g_h, 1000, undo_mfdb.fd_h), 0, 0, 0);
	}

	/* only use portion of work_area on screen */
	rc_intersect(&scrn_area, &work_area);
	undo_area.g_w = work_area.g_w;
	undo_area.g_h = work_area.g_h;
}


/*------------------------------*/
/*      save_work               */
/*------------------------------*/
/* copy work_area to undo_area buffer  */
static VOID save_work(NOTHING)
{
	GRECT tmp_area;

	rc_copy(&work_area, &tmp_area);
	if (rc_intersect(&scrn_area, &tmp_area))
	{
		graf_mouse(M_OFF, NULL);
		rast_op(S_ONLY, &tmp_area, &scrn_mfdb, &undo_area, &undo_mfdb);
		graf_mouse(M_ON, NULL);
	}
}


/*------------------------------*/
/*      restore_work            */
/*------------------------------*/
/* restore work_area from undo_area */
static VOID restore_work(NOTHING)
{
	GRECT tmp_area;

	rc_copy(&work_area, &tmp_area);
	if (rc_intersect(&scrn_area, &tmp_area))
	{
		graf_mouse(M_OFF, NULL);
		rast_op(S_ONLY, &undo_area, &undo_mfdb, &tmp_area, &scrn_mfdb);
		graf_mouse(M_ON, NULL);
	}
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       File Path Name Functions                 ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      dial_name               */
/*------------------------------*/
/* dialogue box input filename  */
static short dial_name(P(char *) name)
PP(char *name;)
{
	OBJECT *tree;
	TEDINFO *ted_addr;
	char c;
	short i, j;
	GRECT box;

	objc_xywh(gl_menu, DOODFILE, &box);
	rsrc_gaddr(R_TREE, DOODSVAD, &tree);
	ted_addr = (TEDINFO *)tree[DOODNAME].ob_spec;
	ted_addr->te_ptext = name;
	ted_addr->te_txtlen = 9;			/* 1 more than null to clear edit field and position cursor */
	name[0] = '\0';

	if (hndl_dial(tree, DOODNAME, box.g_x, box.g_y, box.g_w, box.g_h) == DOODSOK)
	{
		i = j = 0;
		while (TRUE)
		{
			c = name[i++];
			if (!c)
				break;
			if ((c != ' ') && (c != '_'))
				name[j++] = c;
		}
		desel_obj(tree, DOODSOK);
		if (*name == 0)
			return FALSE;
		strcpy(&name[j], ".DOO");
		return TRUE;
	} else
	{
		desel_obj(tree, DOODSCNL);
		return FALSE;
	}
}


/*------------------------------*/
/*      get_path                */
/*------------------------------*/
/* get directory path name      */
static VOID get_path(P(char *) tmp_path, P(char *) spec)
PP(char *tmp_path;)
PP(char *spec;)
{
	short cur_drv;

	cur_drv = Dgetdrv();
	tmp_path[0] = cur_drv + 'A';
	tmp_path[1] = ':';
	tmp_path[2] = '\\';
	Dgetpath(&tmp_path[2], cur_drv + 1);
	if (strlen(tmp_path) > 3)
		strcat(tmp_path, "\\");
	else
		tmp_path[2] = '\0';
	strcat(tmp_path, spec);
}


/*------------------------------*/
/*      add_file_name           */
/*------------------------------*/
/* replace name at end of input file spec */
static VOID add_file_name(P(char *) dname, P(char *) fname)
PP(char *dname;)
PP(char *fname;)
{
	char c;
	short ii;

	ii = strlen(dname);
	while (ii && (((c = dname[ii - 1]) != '\\') && (c != ':')))
		ii--;
	dname[ii] = '\0';
	strcat(dname, fname);
}


/*------------------------------*/
/*      get_file                */
/*------------------------------*/
/* use file selector to get input file */
static BOOLEAN get_file(P(BOOLEAN) loop)
PP(BOOLEAN loop;)
{
	short fs_iexbutton;
	char fs_iinsel[13];

	for (;;)
	{
		get_path(file_name, "*.DOO");
		fs_iinsel[0] = '\0';

#ifdef GEM3
		/* Recent AES supports fsel_exinput() */
		if (gl_xbuf.arch)
		{
			fsel_exinput(file_name, fs_iinsel, &fs_iexbutton, " Select file to load ");
		} else
#endif
		{
			fsel_input(file_name, fs_iinsel, &fs_iexbutton);
		}
		if (fs_iexbutton)
		{
			add_file_name(file_name, fs_iinsel);
			file_handle = Fopen(file_name, O_RDWR);
			if (!loop || (loop && file_handle > 0))
				return TRUE;
		} else
		{
			disab_menu(DOODSAVE);
			disab_menu(DOODABAN);
			return FALSE;
		}
	}
	return FALSE;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Soft Cursor Support                      ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      cursor                  */
/*------------------------------*/
/* turn cursor on,  color = pen_ink or cursor off, color = WHITE       */
static VOID cursor(P(short) color)
PP(short color;)
{
	short pxy[4];

	pxy[0] = key_xcurr + 1;
	pxy[1] = key_ycurr + gl_hspace;
	pxy[2] = key_xcurr + 1;
	pxy[3] = key_ycurr - gl_hbox;

	vsl_color(vdi_handle, color);
	vswr_mode(vdi_handle, MD_REPLACE);
	vsl_type(vdi_handle, FIS_SOLID);
	vsl_width(vdi_handle, PEN_FINE);
	graf_mouse(M_OFF, NULL);
	set_clip(TRUE, &work_area);
	v_pline(vdi_handle, 2, pxy);
	set_clip(FALSE, &work_area);
	graf_mouse(M_ON, NULL);
}


/*------------------------------*/
/*      curs_on                 */
/*------------------------------*/
/* turn 'soft' cursor 'on'      */
static VOID curs_on(NOTHING)
{
	cursor(pen_ink);
}

/*------------------------------*/
/*      curs_off                */
/*------------------------------*/
/* turn 'soft' cursor 'off'     */
static VOID curs_off(NOTHING)
{
	cursor(bkgr_color);
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Menu Handling                            ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      do_about                */
/*------------------------------*/
/* display Doodle Info...       */
static VOID do_about(NOTHING)
{
	OBJECT *tree;
	GRECT box;

	objc_xywh(gl_menu, DOODDESK, &box);	/* DESK menu title xywh */
	rsrc_gaddr(R_TREE, DOODINFD, &tree);	/* address of DOODINFD  */

#ifdef GEM3
	{
		TEDINFO *lpted;
		lpted = (TEDINFO *) (tree[AESID].ob_spec);
		lpted->te_ptext = gl_xbuf.info;
		lpted->te_txtlen = 1 + LSTRLEN(gl_xbuf.info);
	}
#endif

	hndl_dial(tree, 0, box.g_x, box.g_y, box.g_w, box.g_h);
	desel_obj(tree, DOODOK);			/* deselect OK button   */
}


/*------------------------------*/
/*      do_load                 */
/*------------------------------*/
/* load doodle picture file     */
static VOID do_load(P(BOOLEAN) need_name)
PP(BOOLEAN need_name;)
{
	if (!need_name || get_file(TRUE))
	{
		if (file_handle > 0)
		{
			Fread(file_handle, buff_size, buff_location);
			Fclose(file_handle);
			file_handle = -1;
			enab_menu(DOODSAVE);
			enab_menu(DOODABAN);
			restore_work();
		}
	}
}


/*------------------------------*/
/*      do_save                 */
/*------------------------------*/
/* save current named doodle picture    */
static BOOLEAN do_save(NOTHING)
{
	if (*file_name)
	{
		file_handle = Fopen(file_name, O_RDWR);
		if (file_handle > 0)
		{								/* File already exists  */
			if (form_alert(1, string_addr(DOODOVWR)) == 2)
			{							/* Cancel - dont't overwrite  */
				Fclose(file_handle);
				return FALSE;
			}
		}

		if (file_handle < 0)
			file_handle = Fcreate(file_name, 0);
		if (file_handle < 0)
		{								/* disable Save and Abandon   */
			disab_menu(DOODSAVE);
			disab_menu(DOODABAN);
			return FALSE;
		}

		Fwrite(file_handle, buff_size, buff_location);
		enab_menu(DOODSAVE);
		enab_menu(DOODABAN);
		Fclose(file_handle);
		file_handle = -1;
		return TRUE;
	}
	return FALSE;
}


/*------------------------------*/
/*      do_clnormal             */
/*------------------------------*/
/*      set all color checks to NORMAL in color menu    */
static VOID do_clnormal(NOTHING)
{
#ifdef DOODCOLR
	static short const ind[16] = {
		COLOR0, COLOR1, COLOR2, COLOR3, COLOR4, COLOR5, COLOR6, COLOR7,
		COLOR8, COLOR9, COLOR10, COLOR11, COLOR12, COLOR13, COLOR14, COLOR15
	};
	short i;
	
	for (i = 0; i < 16; i++)
		menu_icheck(gl_menu, ind[i], FALSE);
	menu_icheck(gl_menu, ind[pen_ink], TRUE);
#endif
}

/*------------------------------*/
/*      do_save_as              */
/*------------------------------*/
/* save doodle picture as named */
static VOID do_svas(NOTHING)
{
	char name[13];

	if (dial_name(name))
	{
		add_file_name(file_name, name);
		do_save();
	}
}

/*------------------------------*/
/*      set_pen                 */
/*------------------------------*/
/* set pen width and height      */
static VOID set_pen(P(short) pen, P(short) height)
PP(short pen;)
PP(short height;)
{
	dood_pen = pen;
	monumber = 5;						/* thin cross hair */
	mofaddr = NULL;
}

/*------------------------------*/
/*      set_eraser              */
/*------------------------------*/
/* set mouse form to eraser */
static VOID set_eraser(P(short) pen, P(short) height, P(MFORM *) eraser)
PP(short pen;)
PP(short height;)
PP(MFORM *eraser;)
{
	dood_pen = pen;
	dood_shade = PEN_ERASER;
	monumber = 255;
	mofaddr = eraser;
}

/*------------------------------*/
/*      set_color               */
/*------------------------------*/
/*  Set Pen Color Selection   */
static VOID set_color(P(OBJECT *) tree, P(short) obj, P(short) sel, P(short) color_num, P(struct xbind *) bind)
PP(OBJECT *tree;)
PP(short obj;)
PP(short sel;)
PP(short color_num;)
PP(struct xbind *bind;)
{
	/* "-1" because WHITE is not in the list of colors */
	set_select(tree, obj, sel, color_num - 1, bind, color_sel);
}

/*------------------------------*/
/*      get_color               */
/*------------------------------*/
/* Get Pen Color Selection */
static short get_color(P(OBJECT *) tree, P(short) obj, P(short) sel)
PP(OBJECT *tree;)
PP(short obj;)
PP(short sel;)
{
	/* "+1" because WHITE is not in the list of colors */
	return get_select(tree, obj, sel) + 1;
}

/*------------------------------*/
/*      do_penselect            */
/*------------------------------*/
/* use dialogue box to input selection of specified pen/eraser */
static VOID do_penselect(NOTHING)
{
	short exit_obj;
	short psel_obj;
	short color;
	OBJECT *tree;
	struct xbind bind;
	GRECT box;

	objc_xywh(gl_menu, DOODPENS, &box);
	rsrc_gaddr(R_TREE, DOODPEND, &tree);
	/* first setup current selection state */
	switch (dood_pen)
	{
	case PEN_FINE:
		sel_obj(tree, (dood_shade != PEN_ERASER) ? DOODPFIN : DOODEFIN);
		break;
	case PEN_MEDIUM:
		sel_obj(tree, (dood_shade != PEN_ERASER) ? DOODPMED : DOODEMED);
		break;
	case PEN_BROAD:
		sel_obj(tree, (dood_shade != PEN_ERASER) ? DOODPBRD : DOODEBRD);
		break;
	}

	set_color(tree, DOODPCLR, DOODPSEL, pen_ink, &bind);

	/* get dialogue box input */
	exit_obj = hndl_dial(tree, 0, box.g_x, box.g_y, box.g_w, box.g_h);
	for (psel_obj = DOODPFIN; psel_obj <= DOODEBRD; psel_obj++)
		if (tree[psel_obj].ob_state & SELECTED)
		{
			desel_obj(tree, psel_obj);
			break;
		}
	color = get_color(tree, DOODPCLR, DOODPSEL);

	if (exit_obj == DOODPSOK)
	{
		switch (psel_obj)
		{
		case DOODPFIN:
			set_pen(PEN_FINE, char_fine);
			dood_shade = color;
			break;
		case DOODPMED:
			set_pen(PEN_MEDIUM, char_medium);
			dood_shade = color;
			break;
		case DOODPBRD:
			set_pen(PEN_BROAD, char_broad);
			dood_shade = color;
			break;
		case DOODEFIN:
			set_eraser(PEN_FINE, char_fine, mform_addr(ERASEFIN));
			break;
		case DOODEMED:
			set_eraser(PEN_MEDIUM, char_medium, mform_addr(ERASEMED));
			break;
		case DOODEBRD:
			set_eraser(PEN_BROAD, char_broad, mform_addr(ERASEBRD));
			break;
		}
		pen_ink = color;
		desel_obj(tree, DOODPSOK);
	} else
	{
		desel_obj(tree, DOODCNCL);
	}
}


/*------------------------------*/
/*      do_erase                */
/*------------------------------*/
/* clear the screen and the undo buffer */
static VOID do_erase(NOTHING)
{
	short fo_aexbttn;

	fo_aexbttn = form_alert(0, string_addr(ERASEWRN));

	if (fo_aexbttn == 1)
	{
		rast_op(ALL_WHITE, &scrn_area, &scrn_mfdb, &scrn_area, &undo_mfdb);
		restore_work();
	}
}


/*------------------------------*/
/*      hndl_menu               */
/*------------------------------*/
static BOOLEAN hndl_menu(P(short) title, P(short) item)
PP(short title;)
PP(short item;)
{
	BOOLEAN done;

	graf_mouse(ARROW, NULL);
	done = FALSE;

	switch (title)
	{
	case DOODDESK:						/* Desk menu */
		if (item == DOODINFO)			/* 'About Demo' menu item  */
			do_about();
		break;

	case DOODFILE:						/* File menu */
		switch (item)
		{
		case DOODLOAD:					/* Load File    */
			do_load(TRUE);
			break;
		case DOODSAVE:					/* Save File    */
			do_save();
			break;
		case DOODSVAS:					/* Save File As */
			do_svas();
			break;
		case DOODABAN:					/* Abandon File */
			file_handle = Fopen(file_name, O_RDWR);
			do_load(FALSE);
			break;
		case DOODQUIT:					/* Quit - Exit back to Desktop   */
			done = TRUE;
			break;
		}

	case DOODOPTS:						/* Options menu   */
		switch (item)
		{
		case DOODPENS:					/* Pen/Eraser Selection    */
			do_penselect();
			break;
		case DOODERAP:					/* Erase Picture  */
			do_top(dood_whndl);
			do_erase();
			break;
		}
		break;

#ifdef DOODCOLR
	case DOODCOLR:
		switch (item)
		{
		case COLOR0:
		case COLORB0:
			pen_ink = 0;
			break;
		case COLOR1:
		case COLORB1:
			pen_ink = 1;
			break;
		case COLOR2:
		case COLORB2:
			pen_ink = 2;
			break;
		case COLOR3:
		case COLORB3:
			pen_ink = 3;
			break;
		case COLOR4:
		case COLORB4:
			pen_ink = 4;
			break;
		case COLOR5:
		case COLORB5:
			pen_ink = 5;
			break;
		case COLOR6:
		case COLORB6:
			pen_ink = 6;
			break;
		case COLOR7:
		case COLORB7:
			pen_ink = 7;
			break;
		case COLOR8:
		case COLORB8:
			pen_ink = 8;
			break;
		case COLOR9:
		case COLORB9:
			pen_ink = 9;
			break;
		case COLOR10:
		case COLORB10:
			pen_ink = 10;
			break;
		case COLOR11:
		case COLORB11:
			pen_ink = 11;
			break;
		case COLOR12:
		case COLORB12:
			pen_ink = 12;
			break;
		case COLOR13:
		case COLORB13:
			pen_ink = 13;
			break;
		case COLOR14:
		case COLORB14:
			pen_ink = 14;
			break;
		case COLOR15:
		case COLORB15:
			pen_ink = 15;
			break;
		}

		dood_shade = pen_ink;
		do_clnormal();
		break;
#endif
	}
	menu_tnormal(gl_menu, title, TRUE);
	graf_mouse(monumber, mofaddr);
	return done;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Keyboard Handling                        ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      hdl_keyboard            */
/*------------------------------*/
static BOOLEAN hdl_keyboard(P(unsigned short) kreturn)
PP(unsigned short kreturn;)
{
	short i;
	char str[2];
	GRECT lttr, test;

	if ((str[0] = kreturn & 0xff) == 0x03)	/* Ctrl C */
		return TRUE;

	graf_mouse(M_OFF, NULL);
	if (!key_input)
	{
		vswr_mode(vdi_handle, MD_REPLACE);
		vst_color(vdi_handle, pen_ink);

		/* set text height, then calculate space between lines   */
		vst_height(vdi_handle, dood_height, &gl_wchar, &gl_hchar, &gl_wbox, &gl_hbox);
		gl_hspace = gl_hbox - gl_hchar;

		/* set text alignment to left justification, bottom   */
		vst_alignment(vdi_handle, 0, 3, &i, &i);
		/* get current mouse location, button and keybd state */
		graf_mkstate(&key_xbeg, &key_ybeg, &i, &i);

		key_xcurr = ++key_xbeg;
		key_ycurr = --key_ybeg;
	} else
	{
		curs_off();
	}

	str[1] = '\0';
	if (str[0] == 0x1A)					/* Ctrl Z  */
	{
		save_work();					/* update undo area from work area  */
		graf_mouse(M_ON, NULL);
		key_input = FALSE;
		return FALSE;
	} else if (str[0] == 0x0D)			/* carriage return  */
	{
		/* adjust x,y */
		key_ycurr += gl_hbox + gl_hspace;
		key_xcurr = key_xbeg;
	} else if (str[0] == 0x08)			/* backspace  */
	{
		if (key_input && (key_xcurr != key_xbeg))
		{								/* 'back up' */
			for (i = 0; i < gl_wbox; i++)
			{
				key_xcurr--;
				curs_off();
			}
		}
	} else if ((str[0] >= ' ') && (str[0] <= 'z'))
	{									/* output character so long as it fits in the work area */
		lttr.g_x = key_xcurr;
		lttr.g_y = key_ycurr - gl_hbox;
		lttr.g_w = gl_wbox;
		lttr.g_h = gl_hbox;

		rc_copy(&lttr, &test);
		rc_intersect(&work_area, &test);
		if (!rc_equal(&lttr, &test))
		{
			graf_mouse(M_ON, NULL);
			return FALSE;
		}
		set_clip(TRUE, &work_area);
		vswr_mode(vdi_handle, MD_TRANS);
		vst_color(vdi_handle, pen_ink);
		v_gtext(vdi_handle, key_xcurr, key_ycurr, str);
		set_clip(FALSE, &work_area);

		/* update x position */
		key_xcurr += gl_wbox;
	}
	if (!key_input)
	{
		key_input = TRUE;
	}
	curs_on();
	graf_mouse(M_ON, NULL);
	return FALSE;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Message Handling                         ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      do_redraw               */
/*------------------------------*/
/* redraw specified area from undo bfr */
static VOID do_redraw(P(short) wh, P(GRECT *) area)
PP(short wh;)
PP(GRECT *area;)
{
	GRECT box;
	GRECT dirty_source, dirty_dest;

	graf_mouse(M_OFF, NULL);

	/* get the coordinates of the first rectangle in the window's rectangle list */
	wind_get(wh, WF_FIRSTXYWH, &box.g_x, &box.g_y, &box.g_w, &box.g_h);
	while (box.g_w && box.g_h)
	{
		if (rc_intersect(area, &box))
		{
			if (wh == dood_whndl)
			{
				/* copy rectangle list x,y,w,h to dirty_dest   */
				rc_copy(&box, &dirty_dest);
				if (rc_intersect(&work_area, &dirty_dest))
				{
					/*  calculate dirty_source x and y  */
					dirty_source.g_x = (dirty_dest.g_x - work_area.g_x) + undo_area.g_x;
					dirty_source.g_y = (dirty_dest.g_y - work_area.g_y) + undo_area.g_y;

					/* window rectangle w and h to dirty_source  */
					dirty_source.g_w = dirty_dest.g_w;
					dirty_source.g_h = dirty_dest.g_h;

					/* pixel for pixel source to dest copy */
					rast_op(S_ONLY, &dirty_source, &undo_mfdb, &dirty_dest, &scrn_mfdb);
				}
			}
		}
		/* get next rectangle in window's rectangle list     */
		wind_get(wh, WF_NEXTXYWH, &box.g_x, &box.g_y, &box.g_w, &box.g_h);
	}
	/* done walking the rectangle list - turn mouse back on     */
	graf_mouse(M_ON, NULL);
}

/*------------------------------*/
/*      do_full                 */
/*------------------------------*/
/*
 * depending on current window state, either make window
 * full size -or- return to previous shrunken size
 */
static VOID do_full(P(short) wh)
PP(short wh;)
{
	GRECT prev;
	GRECT curr;
	GRECT full;

	graf_mouse(M_OFF, NULL);
	wind_get(wh, WF_CURRXYWH, &curr.g_x, &curr.g_y, &curr.g_w, &curr.g_h);
	wind_get(wh, WF_PREVXYWH, &prev.g_x, &prev.g_y, &prev.g_w, &prev.g_h);
	wind_get(wh, WF_FULLXYWH, &full.g_x, &full.g_y, &full.g_w, &full.g_h);
	if (rc_equal(&curr, &full))
	{
		/* is full now so change to previous  */
		graf_shrinkbox(prev.g_x, prev.g_y, prev.g_w, prev.g_h, full.g_x, full.g_y, full.g_w, full.g_h);
		wind_set(wh, WF_CURRXYWH, prev.g_x, prev.g_y, prev.g_w, prev.g_h);
		rc_copy(&save_area, &undo_area);
		set_work(TRUE);
		if ((prev.g_x == full.g_x) && (prev.g_y == full.g_y))
			do_redraw(wh, &work_area);
	} else
	{
		/* is not full so make it full */
		rc_copy(&undo_area, &save_area);
		graf_growbox(curr.g_x, curr.g_y, curr.g_w, curr.g_h, full.g_x, full.g_y, full.g_w, full.g_h);
		wind_set(wh, WF_CURRXYWH, full.g_x, full.g_y, full.g_w, full.g_h);
		set_work(TRUE);
	}
	graf_mouse(M_ON, NULL);
}


/*------------------------------*/
/*      hdl_msg                 */
/*------------------------------*/
static BOOLEAN hdl_msg(NOTHING)
{
	BOOLEAN done;
	short wdw_hndl;
	GRECT work;

	done = FALSE;
	wdw_hndl = gl_rmsg[3];
	switch (gl_rmsg[0])
	{
	case MN_SELECTED:
		done = hndl_menu(wdw_hndl, gl_rmsg[4]);	/* Title, Item */
		break;

	case WM_REDRAW:
		do_redraw(wdw_hndl, (GRECT *) & gl_rmsg[4]);	/* x,y,w,h    */
		break;

	case WM_TOPPED:
		if (wdw_hndl == dood_whndl)		/* make sure it's my window   */
			wind_set(wdw_hndl, WF_TOP, 0, 0, 0, 0);
		break;
	case WM_CLOSED:
		done = TRUE;					/* terminate, exit back to DESKTOP.APP */
		break;
	case WM_FULLED:
		do_full(wdw_hndl);				/* toggle between full and previous   */
		break;

	case WM_ARROWED:					/* calculate new undo_area x,y      */
		switch (gl_rmsg[4])				/* requested action */
		{
		case WA_UPPAGE:					/* page up  */
			undo_area.g_y = max(undo_area.g_y - undo_area.g_h, 0);
			break;
		case WA_DNPAGE:					/* page down   */
			undo_area.g_y += undo_area.g_h;
			break;
		case WA_UPLINE:					/* row up   */
			undo_area.g_y = max(undo_area.g_y - YSCALE(16), 0);
			break;
		case WA_DNLINE:					/* row down */
			undo_area.g_y += YSCALE(16);
			break;
		case WA_LFPAGE:					/* page left   */
			undo_area.g_x = max(undo_area.g_x - undo_area.g_w, 0);
			break;
		case WA_RTPAGE:					/* page right  */
			undo_area.g_x += undo_area.g_w;
			break;
		case WA_LFLINE:					/* column left */
			undo_area.g_x = max(undo_area.g_x - 16, 0);
			break;
		case WA_RTLINE:					/* column right   */
			undo_area.g_x += 16;
			break;
		}
		set_work(TRUE);					/* update slider positions    */
		restore_work();					/* update screen from undo_area    */
		break;

	case WM_HSLID:						/* horizontal slider */
		undo_area.g_x = align_x(umul_div(undo_mfdb.fd_w - undo_area.g_w, gl_rmsg[4], 1000));
		set_work(TRUE);
		restore_work();
		break;
	case WM_VSLID:						/* vertical slider   */
		undo_area.g_y = umul_div(undo_mfdb.fd_h - undo_area.g_h, gl_rmsg[4], 1000);
		set_work(TRUE);
		restore_work();
		break;

	case WM_SIZED:						/* new window size requested  */
		/* get work area x,y,w,h      */
		wind_calc(1, 0x0fef, gl_rmsg[4], gl_rmsg[5], gl_rmsg[6],
				  gl_rmsg[7], &work.g_x, &work.g_y, &work.g_w, &work.g_h);

		work.g_x = align_x(work.g_x);
		work.g_w = align_x(work.g_w);

		/* get border area x, y, w, h */
		wind_calc(0, 0x0fef, work.g_x, work.g_y, work.g_w, work.g_h,
				  &gl_rmsg[4], &gl_rmsg[5], &gl_rmsg[6], &gl_rmsg[7]);

		/* set current x,y,w,h - borders, title bar, (info)     */
		wind_set(wdw_hndl, WF_CURRXYWH, gl_rmsg[4], gl_rmsg[5], gl_rmsg[6], gl_rmsg[7]);
		set_work(TRUE);					/* update slider positions */
		break;
	case WM_MOVED:						/* user moved the window         */
		gl_rmsg[4] = align_x(gl_rmsg[4]);
		wind_set(wdw_hndl, WF_CURRXYWH, align_x(gl_rmsg[4]) - 1, gl_rmsg[5], gl_rmsg[6], gl_rmsg[7]);
		set_work(FALSE);				/* NO slider update   */
		break;
	}
	return done;
}

/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Mouse Handling                           ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      hdl_mouse               */
/*------------------------------*/
/* change mouse form depending on whether it's in or out of window    */
static BOOLEAN hdl_mouse(NOTHING)
{
	if (m_out)
		graf_mouse(ARROW, NULL);
	else
		graf_mouse(monumber, mofaddr);

	m_out = !m_out;						/* change MU_M1 entry/exit flag  */
	return FALSE;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Button Handling                          ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*----------------------------------------------*/
/*      do_fill         HK 6/30/85              */
/*----------------------------------------------*/
static VOID do_fill(P(short) x, P(short) y)
PP(short x;)
PP(short y;)
{
	short idx;

	vsf_interior(vdi_handle, IP_1PATT);
	vsf_color(vdi_handle, pen_ink);
	idx = -1;							/* for complete fill */

	graf_mouse(M_OFF, NULL);
	v_contourfill(vdi_handle, x, y, idx);
	save_work();
	graf_mouse(M_ON, NULL);
}


/*------------------------------*/
/*      eraser                  */
/*------------------------------*/
/* erase rectangle of eraser size at x,y */
static VOID eraser(P(short) x, P(short) y)
PP(short x;)
PP(short y;)
{
	short erase_xy[4];

	if (dood_pen == PEN_FINE)			/* 5 x 3 */
	{
		erase_xy[0] = x - 2;
		erase_xy[1] = y - 1;
		erase_xy[2] = x + 2;
		erase_xy[3] = y + 1;
	} else if (dood_pen == PEN_MEDIUM)	/* 9 x 5 */
	{
		erase_xy[0] = x - 4;
		erase_xy[1] = y - 2;
		erase_xy[2] = x + 4;
		erase_xy[3] = y + 2;
	} else								/* 13 x 7 */
	{
		erase_xy[0] = x - 6;
		erase_xy[1] = y - 3;
		erase_xy[2] = x + 6;
		erase_xy[3] = y + 3;
	}
	vswr_mode(vdi_handle, MD_REPLACE);
	vr_recfl(vdi_handle, erase_xy);
}


/*------------------------------*/
/*      draw_pencil             */
/*------------------------------*/
static VOID draw_pencil(P(short) x, P(short) y)
PP(short x;)
PP(short y;)
{
	short pxy[4];
	BOOLEAN done;
	unsigned short mflags;
	unsigned short locount, hicount;
	short ev_which, bbutton, kstate, kreturn, breturn;

	set_clip(TRUE, &work_area);
	pxy[0] = x;
	pxy[1] = y;

	vsl_color(vdi_handle, dood_shade);	/* set line color */
	vswr_mode(vdi_handle, MD_REPLACE);	/* replace writing mode */
	vsl_type(vdi_handle, FIS_SOLID);	/* solid line type   */

	if (dood_shade != PEN_ERASER)
	{
		vsl_width(vdi_handle, dood_pen);	/* set line width   */
		vsl_ends(vdi_handle, 2, 2);		/* rounded end style  */
		hicount = 0;					/* MU_TIMER high   */
		locount = 125;					/*  and low count  */
		mflags = MU_BUTTON | MU_M1 | MU_TIMER;
		graf_mouse(M_OFF, NULL);		/* turn mouse 'off'   */
	} else
	{
		vsf_interior(vdi_handle, 1);	/* solid interior fill  */
		vsf_color(vdi_handle, bkgr_color);	/* fill color     */
		mflags = MU_BUTTON | MU_M1;
	}

	done = FALSE;
	while (!done)
	{
		ev_which = evnt_multi(mflags, 0x01, 0x01, 0x00,	/* 1 click, 1 button, button up */
							  1, pxy[0], pxy[1], 1, 1,
							  0, 0, 0, 0, 0,
							  gl_rmsg, locount, hicount, &pxy[2], &pxy[3], &bbutton, &kstate, &kreturn, &breturn);

		if (ev_which & MU_BUTTON)
		{
			if (!(mflags & MU_TIMER))
				graf_mouse(M_OFF, NULL);
			if (dood_shade != PEN_ERASER)
				v_pline(vdi_handle, 2, pxy);
			else
				eraser(pxy[2], pxy[3]);
			graf_mouse(M_ON, NULL);
			done = TRUE;
		} else if (ev_which & MU_TIMER)
		{
			graf_mouse(M_ON, NULL);
			mflags = MU_BUTTON | MU_M1;
		} else
		{
			if (!(mflags & MU_TIMER))
				graf_mouse(M_OFF, NULL);
			if (dood_shade != PEN_ERASER)
			{
				v_pline(vdi_handle, 2, pxy);
				mflags = MU_BUTTON | MU_M1 | MU_TIMER;
			} else
			{
				eraser(pxy[2], pxy[3]);
				graf_mouse(M_ON, NULL);
			}
			pxy[0] = pxy[2];
			pxy[1] = pxy[3];
		}
	}

	set_clip(FALSE, &work_area);
	save_work();
}


/*------------------------------*/
/*      hdl_button              */
/*------------------------------*/
static BOOLEAN hdl_button(P(short) mousex, P(short) mousey)
PP(short mousex;)
PP(short mousey;)
{
	BOOLEAN done;

	done = FALSE;
	if (inside(mousex, mousey, &work_area))
	{
		if (dood_fill)
			do_fill(mousex, mousey);
		else
			draw_pencil(mousex, mousey);
	}
	return done;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Doodle Event Handler                     ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/

/*------------------------------*/
/*      doodle                  */
/*------------------------------*/
static VOID doodle(NOTHING)
{
	BOOLEAN done;
	short ev_which;
	short mousex, mousey;			/* mouse x,y position           */
	short bstate, bclicks;			/* button state, & # of clicks  */
	short kstate, kreturn;			/* key state and keyboard char  */
	
	key_input = FALSE;
	done = FALSE;
	while (!done)						/* loop handling user input until done  */
	{
		ev_which = evnt_multi(MU_BUTTON | MU_MESAG | MU_M1 | MU_KEYBD,
			0x02, 0x01, 0x01,	/* 2 clicks, 1 button, button down */
			m_out,	work_area.g_x, work_area.g_y, work_area.g_w, work_area.g_h,
			0, 0, 0, 0, 0,
			gl_rmsg, 0, 0,
			&mousex, &mousey, &bstate, &kstate, &kreturn, &bclicks);

		wind_update(BEG_UPDATE);

		if (!(ev_which & MU_KEYBD))		/* not KEYBD event */
		{
			if (key_input)				/* key_input TRUE? */
			{
				curs_off();				/* turn cursor off */
				key_input = FALSE;
				save_work();
			}
		}

		if (ev_which & MU_BUTTON)
			done |= hdl_button(mousex, mousey);

		if (ev_which & MU_M1)
			done |= hdl_mouse();

		if (ev_which & MU_MESAG)
			done |= hdl_msg();

		if (ev_which & MU_KEYBD)
			done |= hdl_keyboard(kreturn);

		wind_update(END_UPDATE);
	}
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Termination                              ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/


/*------------------------------*/
/*      dood_term               */
/*------------------------------*/
static VOID dood_term(P(short) term_type)
PP(short term_type;)
{
	wind_update(BEG_UPDATE);
	switch (term_type)					/* NOTE: all cases fall through         */
	{
	case (0 /* normal termination */ ):
#ifdef GEM3
		if (gl_xbuf.abilities & ABLE_PROP)
		{
			char psbuf[10];

			sprintf(psbuf, "%d", dood_pen);
			prop_put("PTK.DOOD", "Pen.size", psbuf, 0);
			sprintf(psbuf, "%d", dood_shade);
			prop_put("PTK.DOOD", "Pen.shade", psbuf, 0);
			sprintf(psbuf, "%d", pen_ink);
			prop_put("PTK.DOOD", "Pen.colour", psbuf, 0);
		}
#endif
		do_close(dood_whndl);
		wind_delete(dood_whndl);
	case (3 /* no window available */ ):
		menu_bar(0x0L, FALSE);
		Mfree(buff_location);
	case (2 /* not enough memory */ ):
		graf_mouse(ARROW, NULL);
		v_clsvwk(vdi_handle);
	case (1 /* no .RSC file or no virt wksta */ ):
		wind_update(END_UPDATE);
		appl_exit();
	case (4 /* appl_init() failed */ ):
		break;
	}
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Initialization                           ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/
/*------------------------------*/
/*      pict_init               */
/*------------------------------*/
/* transform IMAGES and ICONS */
/*  set up Programmer Defined objects */
static VOID pict_init(NOTHING)
{
	OBJECT *tree;
	short tr_obj, nobj;

	rsrc_gaddr(R_TREE, DOODINFD, &tree);
	trans_gimage(tree, DOODIMG);
	rsrc_gaddr(R_TREE, DOODPEND, &tree);
	for (tr_obj = DOODPFIN; tr_obj <= DOODEBRD; tr_obj++)
	{
		/* loop through Pen/Eraser  Dialog objects transforming  */
		/* them, setting type to G_PROGDEF, establishing the  */
		/* address of the drawing code and setting the obspec    */
		/* pointer for each              */

		trans_gimage(tree, tr_obj);
		tree[tr_obj].ob_type = G_USERDEF;
		nobj = tr_obj - DOODPFIN;
		brushub[nobj].ub_code = (long)dr_code;
		brushub[nobj].ub_parm = tree[tr_obj].ob_spec;
		tree[tr_obj].ob_spec = (long)&brushub[nobj];
	}
}

/*------------------------------*/
/*      dood_init               */
/*------------------------------*/
static int dood_init(NOTHING)
{
	short work_in[11];
	short i;

#ifdef GEM3
	char psbuf[10];

	memset(&gl_xbuf, 0, sizeof(gl_xbuf));
	gl_xbuf.buf_len = sizeof(gl_xbuf);
	gl_apid = appl_init(&gl_xbuf);		/* init application  */
#else
	gl_apid = appl_init();				/* initialize libraries */
#endif

	if (gl_apid == -1)
		return 4;

#ifdef GEM3
	/* If GEM does not contain a version string, fake one. */
	if (!gl_xbuf.arch)
	{
		gl_xbuf.info = "Digital Research GEM";
	}
#endif

	wind_update(BEG_UPDATE);
	graf_mouse(HOURGLASS, NULL);
	if (!rsrc_load("doodle.rsc"))
	{
		/* No Resource File  */
		graf_mouse(ARROW, NULL);
		form_alert(1, "[3][Fatal Error !|DOODLE.RSC|File Not Found][ Abort ]");
		return 1;
	}
	/* open virtual workstation */
	for (i = 0; i < 10; i++)
	{
		work_in[i] = 1;
	}
	work_in[10] = 2;

	/* Get the VDI handle for GEM AES screen workstation */
	gem_handle = graf_handle(&gl_wchar, &gl_hchar, &gl_wbox, &gl_hbox);
	vdi_handle = gem_handle;
	v_opnvwk(work_in, &vdi_handle, work_out);

	if (vdi_handle == 0)
		return 1;

	scrn_width = work_out[0] + 1;		/* # of pixels wide  */
	scrn_height = work_out[1] + 1;		/* # of pixels high  */
	scrn_xsize = work_out[3];			/* pixel width (microns) */
	scrn_ysize = work_out[4];			/* pixel height(microns) */

	char_fine = work_out[46];			/* minimum char height  */
	char_medium = work_out[48];			/* maximum char height  */
	char_broad = char_medium * 2;

	vq_extnd(vdi_handle, 1, work_out);	/* extended inquire  */
	scrn_planes = work_out[4];			/* # of planes    */

	/* allocate undo buffer */
	undo_mfdb.fd_w = scrn_width;		/* width in pixels   */
	undo_mfdb.fd_wdwidth = undo_mfdb.fd_w >> 4;	/* width in words */
	undo_mfdb.fd_h = scrn_height;		/* form height    */
	undo_mfdb.fd_nplanes = scrn_planes;		/* # of planes    */
	undo_mfdb.fd_stand = 0;

	buff_size = (long) (undo_mfdb.fd_w >> 3) *	/* # of bytes     */
		(long) undo_mfdb.fd_h *			/* form height    */
		(long) undo_mfdb.fd_nplanes;			/* # of planes    */
	buff_location = undo_mfdb.fd_addr = (VOIDPTR)Malloc(buff_size);

	if (buff_location == NULL)
		return 2;						/* not enough memory */

	scrn_area.g_x = 0;
	scrn_area.g_y = 0;
	scrn_area.g_w = scrn_width;
	scrn_area.g_h = scrn_height;
	scrn_mfdb.fd_addr = 0;

	rc_copy(&scrn_area, &undo_area);
	rast_op(ALL_WHITE, &undo_area, &scrn_mfdb, &undo_area, &undo_mfdb);

	pict_init();					/* transforms & user defined objects */

	/* Get Desktop work area   */
	wind_get(DESK, WF_WORKXYWH, &gl_xfull, &gl_yfull, &gl_wfull, &gl_hfull);
	/* initialize menu    */
	rsrc_gaddr(R_TREE, DOODMENU, &gl_menu);
	do_clnormal();

	/* show menu         */
	menu_bar(gl_menu, TRUE);

	/* create window with all components except info line */
	dood_whndl = wind_create(0x0fef, gl_xfull, gl_yfull, gl_wfull, gl_hfull);
	if (dood_whndl == -1)
	{
		/* No Window Available        */
		form_alert(1, string_addr(DOODNWDW));
		return 3;
	}

	graf_mouse(HOURGLASS, NULL);
#ifndef __ALCYON__
	wind_set_str(dood_whndl, WF_NAME, wdw_title);
#else
	wind_set(dood_whndl, WF_NAME, wdw_title, 0, 0);
#endif

	do_open(dood_whndl, gl_xfull, gl_yfull, gl_wfull, gl_hfull);

	/* get work area of window */
	wind_get(dood_whndl, WF_WORKXYWH, &work_area.g_x, &work_area.g_y, &work_area.g_w, &work_area.g_h);

	set_work(TRUE);						/* initial slider positions */
	rc_copy(&undo_area, &save_area);	/* save_area used by do_full() */

#ifdef GEM3
	/* [JCE 25-7-1999] If the AES supports it, read settings
	 *
	 * Care is taken to guard against bizarre values.
	 *
	 */

	if (gl_xbuf.abilities & ABLE_PROP)
	{
		if (!prop_get("PTK.DOOD", "Pen.size", psbuf, 10, 0))
		{
			dood_pen = atoi(psbuf);
			if (dood_pen != PEN_FINE && dood_pen != PEN_MEDIUM && dood_pen != PEN_BROAD)
				dood_pen = PEN_FINE;
		}
		if (!prop_get("PTK.DOOD", "Pen.shade", psbuf, 10, 0))
		{
			dood_shade = atoi(psbuf);
		}
		if (!prop_get("PTK.DOOD", "Pen.colour", psbuf, 10, 0))
		{
			pen_ink = atoi(psbuf);
		}
	}
#endif

	graf_mouse(ARROW, NULL);
	wind_update(END_UPDATE);
	return 0;
}


/************************************************************************/
/************************************************************************/
/****                                                                ****/
/****                       Main Program                             ****/
/****                                                                ****/
/************************************************************************/
/************************************************************************/

/*------------------------------*/
/*      GEMAIN                  */
/*------------------------------*/
int main(NOTHING)
{
	int term_type;

#ifdef __ALCYON__
	/* symbols etoa and ftoa are unresolved */
	asm("xdef _etoa");
	asm("_etoa equ 0");
	asm("xdef _ftoa");
	asm("_ftoa equ 0");
#endif

	if (!(term_type = dood_init()))
		doodle();
	dood_term(term_type);
	return 0;
}
