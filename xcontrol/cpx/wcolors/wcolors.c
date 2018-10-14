/* wcolors.c
 * CPX to set default top and background windows
 * 09/01/92 cjg - Recreated and Rewritten under Lattice C
 *		- Added Background window parts
 * 09/22/92 cjg - Fixed RAM-Resident Mode
 * 01/14/93 cjg - Handle new 3D format
 */

/* INCLUDE FILES
 * ================================================================
 */
#define MYTEDINFOS 1
#define PROGRAM 0
#define TESTXLATE  0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tos.h>
#ifdef LATTICE
#include <osbind.h>
#endif

#include "gemskel.h"
#include "gemerror.h"
#undef K_INSERT
#include "vdikeys.h"
#include "country.h"
#include "wcolors.h"
#include "wcolors.rsh"
#include "cpxdata.h"

#ifndef OF_FL3DIND
#define OF_FL3DIND 0x200
#define OF_FL3DBAK 0x400
#endif

#ifndef CT_KEY
#define CT_KEY 53
#endif

#ifndef COUNTRY_US
#define COUNTRY_US	 0	/* en_US USA */
#define COUNTRY_DE	 1	/* de_DE Germany */
#define COUNTRY_FR	 2	/* fr_FR France */
#define COUNTRY_UK	 3	/* en_GB United Kingdom */
#define COUNTRY_ES	 4	/* es_ES Spain */
#define COUNTRY_IT	 5	/* it_IT Italy */
#define COUNTRY_SE	 6	/* sv_SE Sweden */
#endif

#define TedBorder(obj)   ( tree[(obj)].ob_spec.tedinfo->te_thickness )

#ifndef OBSPEC_GET_CHARACTER
#define OBSPEC_GET_CHARACTER(obspec)   ((unsigned char) ( (((obspec).index) >> 24) & 0xff ))
#define OBSPEC_GET_FRAMESIZE(obspec)   ((signed char)   ( (((obspec).index) >> 16) & 0xff ))
#define OBSPEC_GET_FRAMECOL(obspec)    ((unsigned char) ( (((obspec).index) >> 12) & 0x0f ))
#define OBSPEC_GET_TEXTCOL(obspec)     ((unsigned char) ( (((obspec).index) >>  8) & 0x0f ))
#define OBSPEC_GET_TEXTMODE(obspec)    ((unsigned char) ( (((obspec).index) >>  7) & 0x01 ))
#define OBSPEC_GET_FILLPATTERN(obspec) ((unsigned char) ( (((obspec).index) >>  4) & 0x07 ))
#define OBSPEC_GET_INTERIORCOL(obspec) ((unsigned char) ( (((obspec).index)      ) & 0x0f ))

#define OBSPEC_SET_CHARACTER(obspec, ch)            (obspec).index = ( (((obspec).index) & 0x00ffffffl) | ((((unsigned long)((ch)          & 0xff)) << 24)) )
#define OBSPEC_SET_FRAMESIZE(obspec, framesize)     (obspec).index = ( (((obspec).index) & 0xff00ffffl) | ((((unsigned long)((framesize)   & 0xff)) << 16)) )
#define OBSPEC_SET_FRAMECOL(obspec, framecol)       (obspec).index = ( (((obspec).index) & 0xffff0fffl) | ((((unsigned long)((framecol)    & 0x0f)) << 12)) )
#define OBSPEC_SET_TEXTCOL(obspec, textcol)         (obspec).index = ( (((obspec).index) & 0xfffff0ffl) | ((((unsigned long)((textcol)     & 0x0f)) <<  8)) )
#define OBSPEC_SET_TEXTMODE(obspec, textmode)       (obspec).index = ( (((obspec).index) & 0xffffff7fl) | ((((unsigned long)((textmode)    & 0x01)) <<  7)) )
#define OBSPEC_SET_FILLPATTERN(obspec, fillpattern) (obspec).index = ( (((obspec).index) & 0xffffff8fl) | ((((unsigned long)((fillpattern) & 0x07)) <<  4)) )
#define OBSPEC_SET_INTERIORCOL(obspec, interiorcol) (obspec).index = ( (((obspec).index) & 0xfffffff0l) | ((((unsigned long)((interiorcol) & 0x0f))      )) )

#define COLSPEC_GET_FRAMECOL(color)    (((color) >> 12) & 0x0f)
#define COLSPEC_GET_TEXTCOL(color)     (((color) >>  8) & 0x0f)
#define COLSPEC_GET_TEXTMODE(color)    (((color) >>  7) & 0x01)
#define COLSPEC_GET_FILLPATTERN(color) (((color) >>  4) & 0x07)
#define COLSPEC_GET_INTERIORCOL(color) (((color)      ) & 0x0f)

#define COLSPEC_SET_FRAMECOL(color, framecol)       color = ( ((color) & 0x0fff) | (((framecol)    & 0x0f) << 12) )
#define COLSPEC_SET_TEXTCOL(color, textcol)         color = ( ((color) & 0xf0ff) | (((textcol)     & 0x0f) <<  8) )
#define COLSPEC_SET_TEXTMODE(color, textmode)       color = ( ((color) & 0xff7f) | (((textmode)    & 0x01) <<  7) )
#define COLSPEC_SET_FILLPATTERN(color, fillpattern) color = ( ((color) & 0xff8f) | (((fillpattern) & 0x07) <<  4) )
#define COLSPEC_SET_INTERIORCOL(color, interiorcol) color = ( ((color) & 0xfff0) | (((interiorcol) & 0x0f)      ) )

#define ICOLSPEC_GET_DATACOL(color)   ( ((color) >> 12) & 0x0f )
#define ICOLSPEC_GET_MASKCOL(color)   ( ((color) >>  8) & 0x0f )
#define ICOLSPEC_GET_CHARACTER(color) ( ((color)      ) & 0xff )

#define ICOLSPEC_SET_DATACOL(color, datacol) color = ( ((color) & 0x0fff) | (((datacol) & 0x0f) << 12) )
#define ICOLSPEC_SET_MASKCOL(color, maskcol) color = ( ((color) & 0xf0ff) | (((maskcol) & 0x0f) <<  8) )
#define ICOLSPEC_SET_CHARACTER(color, ch)    color = ( ((color) & 0xff00) | (((ch)      & 0xff)      ) )
#endif


#define COLOR_MIN 0
#define COLOR_MAX 15
#define COLOR_RANGE (COLOR_MAX - COLOR_MIN + 1)
#define WSHADOW 3

#undef ObColor
#define ObColor(obj)  ((WORD *)(&tree[(obj)].ob_spec.index))[1]

#undef NULLFUNC
#define NULLFUNC ((void (*)(OBJECT *))0)


typedef struct
{
	WORD name[2];
	WORD info[2];
	WORD sizer[2];
	WORD closer[2];
	WORD fuller[2];
	WORD vbar;
	WORD uparrow[2];
	WORD dnarrow[2];
	WORD vslid[2];
	WORD velev[2];
	WORD hbar;
	WORD lfarrow[2];
	WORD rtarrow[2];
	WORD hslid[2];
	WORD helev[2];
	WORD c19;
	WORD c20;
	WORD c21;
	WORD c22;
	WORD c23;
	WORD c24;
} WColors;


/* EXTERNALS
 * ================================================================
 */
extern WColors savecolors[4];
extern WColors defcolors;
extern WColors altcolors[10];

/* GLOBALS
 * ================================================================
 */
int AES_Version;

static XCPB *xcpb;
CPXINFO cpxinfo;

static GRECT norect;
static int curobj;
static int curcborder;
static int curctext;
static int curcfill;
static int curcolor;
static BOOLEAN curIsTed;
static BOOLEAN canSetColors;
static GRECT formrect;
static WORD ncolors;
static WORD saveindex;
static LONG const dcrates[5] = { 450L, 330L, 275L, 220L, 165L };

static LONG click_delay;

static WColors *wcolors;

#define NUM_OBJECTS  19

static int const xobj[] = {
	TSIZER, TCLOSER, TFULLER, TUP, TDN,
	TVELEV, TLF, TRT, THELEV,
	BGSIZER, BFULLER, BUP, BDN,
	BVELEV, BLF, BRT, BHELEV, TNAME, BGNAME
};



static void ObjcGrect(OBJECT *tree, WORD object, GRECT *r)
{
	objc_offset(tree, object, &r->g_x, &r->g_y);
	r->g_w = tree[object].ob_width;
	r->g_h = tree[object].ob_height;
}


static void ObjcXGrect(OBJECT *tree, WORD object, GRECT *r)
{
	WORD framesize;
	WORD offset;
	OBSPEC ob_spec;
	
	ObjcGrect(tree, object, r);
	tree += object;
	if (tree->ob_flags & INDIRECT)
	{
		ob_spec = *tree->ob_spec.indirect;
	} else
	{
		ob_spec = tree->ob_spec;
	}
	if (tree->ob_type == G_TEXT || tree->ob_type == G_BOXTEXT || tree->ob_type == G_FTEXT || tree->ob_type == G_FBOXTEXT)
	{
		framesize = ob_spec.tedinfo->te_thickness;
	} else
	{
		framesize = ob_spec.obspec.framesize;
	}
	if (framesize >= 0)
	{
		offset = 0;
	} else
	{
		offset = framesize;
		framesize = -framesize;
	}
	if ((tree->ob_state & OUTLINED) && offset > -3)
	{
		offset = -3;
	}
	if (offset != 0)
	{
		Rc_center(r, r, offset, offset);
	}
	if ((tree->ob_state & SHADOWED) && framesize != 0)
	{
		r->g_w += framesize + framesize;
		r->g_h += framesize + framesize;
	}
}


WORD _AesCtrl(long code);

BOOLEAN ObjcDraw(OBJECT *tree, WORD object, WORD depth, const GRECT *clip)
{
	GRECT r;
	int *p = _GemParBlk.intin;
	
	if (clip == NULL)
	{
		clip = &r;
		ObjcXGrect(tree, object, &r);
	}
	*p++ = object;
	*p++ = depth;
	*((GRECT *)p)++ = r;
	_GemParBlk.addrin[0] = tree;
	return _AesCtrl(42);
}


static void init_tree(OBJECT *tree)
{
	int i;

	if (AES_Version < 0x0330)
	{
		HideObj(BFULLER);
		HideObj(BUP);
		HideObj(BDN);
		HideObj(BVELEV);
		HideObj(BVSLID);
		HideObj(BLF);
		HideObj(BRT);
		HideObj(BHELEV);
		HideObj(BHSLID);

		ObX(BGNAME) = ObX(BGINFO);
		ObW(BGNAME) = ObW(BGINFO);
	} else
	{
		/* Turn off the activator first */
		ObFlags(SAVE) &= ~OF_FL3DIND;
		ObFlags(SAVE) &= ~OF_FL3DBAK;

		ObFlags(OK) &= ~OF_FL3DIND;
		ObFlags(OK) &= ~OF_FL3DBAK;

		ObFlags(CANCEL) &= ~OF_FL3DIND;
		ObFlags(CANCEL) &= ~OF_FL3DBAK;

		if (ncolors > LWHITE)
		{
			/* Activators */
			ObFlags(SAVE) |= OF_FL3DIND;
			ObFlags(SAVE) |= OF_FL3DBAK;

			ObFlags(OK) |= OF_FL3DIND;
			ObFlags(OK) |= OF_FL3DBAK;

			ObFlags(CANCEL) |= OF_FL3DIND;
			ObFlags(CANCEL) |= OF_FL3DBAK;

			ObX(SAVE) += 2;
			ObY(SAVE) += 2;
			ObW(SAVE) -= 4;
			ObH(SAVE) -= 4;

			ObX(OK) += 2;
			ObY(OK) += 2;
			ObW(OK) -= 4;
			ObH(OK) -= 4;

			ObX(CANCEL) += 2;
			ObY(CANCEL) += 2;
			ObW(CANCEL) -= 4;
			ObH(CANCEL) -= 4;

			for (i = 0; i < NUM_OBJECTS; i++)
			{
				ObFlags(xobj[i]) |= OF_FL3DIND;	/* Indicator */
				ObFlags(xobj[i]) &= ~OF_FL3DBAK;

				ObX(xobj[i]) += 2;
				ObY(xobj[i]) += 2;
				ObW(xobj[i]) -= 4;
				ObH(xobj[i]) -= 4;
			}
		}
	}
}


static void wait_bup(void)
{
	MRETS m;

	do
	{
		Graf_mkstate(&m);
	} while (m.buttons);
}


static void itoa2(int n, char *s)
{
	*s = n / 10 + '0';
	if (*s != '0')
		s++;
	*s++ = n % 10 + '0';
	*s = 0;
}


static void getrect(OBJECT *tree, int obj, GRECT *d)
{
	*d = ObRect(obj);
	objc_offset(tree, obj, &d->g_x, &d->g_y);
}


/*
 * Fix up X position of slider based on value
 */
static void sl_x(OBJECT *tree, int slid, int elev, int value, int min, int max, void (*foo) (OBJECT *tree))
{
	GRECT slidr, elevr;

#define TRUEVALUE  ( (long)(value - min) )
#define PIXELRANGE ( (long)(slidr.g_w - elevr.g_w) * 1000L )
#define VALUERANGE ( (long)(max - min + 1) * 1000L )

	getrect(tree, slid, &slidr);
	getrect(tree, elev, &elevr);

	if (value == max)
		ObX(elev) = slidr.g_w - elevr.g_w;
	else
		ObX(elev) = (int) ((TRUEVALUE * PIXELRANGE) / VALUERANGE);

	if (foo)
		foo(tree);
}


static void draw_slider(OBJECT *tree, int slid, int elev, int value, int min, int max, void (*foo) (OBJECT *tree))
{
	GRECT newr, oldr, slidr;
	int offset;
	BOOLEAN moveleft;

	/* get old position */
	getrect(tree, elev, &oldr);
	offset = 0;
	if (TedBorder(elev) < 0)
		offset = TedBorder(elev);
	Rc_center(&oldr, &oldr, offset, offset);
	/* set new position */
	sl_x(tree, slid, elev, value, min, max, foo);

	/* draw new */
	getrect(tree, elev, &newr);
	Rc_center(&newr, &newr, offset, offset);
	ObjcDraw(tree, elev, MAX_DEPTH, &newr);
	/* undraw old */
	moveleft = newr.g_x < oldr.g_x;
	if (rc_intersect(&oldr, &newr))
	{
		newr.g_w -= 1;
		oldr.g_w -= newr.g_w;
		if (moveleft)
			oldr.g_x += newr.g_w;
	}
	getrect(tree, slid, &slidr);
	rc_intersect(&oldr, &slidr);
	ObjcDraw(tree, slid, MAX_DEPTH, &slidr);
	oldr.g_h = 1;
	ObjcDraw(tree, TBOX, MAX_DEPTH, &oldr);
	oldr.g_y += slidr.g_h + 1;
	ObjcDraw(tree, TBOX, MAX_DEPTH, &oldr);

	/* if setting info border color, or
	 * at top of any slider setting up, velevator or vslider colors,
	 * redraw FLOATER
	 */
	if ((curobj == TINFO && slid == BORDSLID) ||
		((value == COLOR_MAX || (value == COLOR_MAX - 1)) &&
		 (curobj == TUP || curobj == TVELEV || curobj == TVSLID)))
		ObjcDraw(tree, FLOATER, MAX_DEPTH, NULL);

}


static void sl_dragx(OBJECT *tree, int slid, int elev, int min, int max, int *numvar, void (*foo) (OBJECT *tree))
{
	int newvalue;
	int xoffset;
	int elevx;
	int lastvalue;
	MRETS m;
	GRECT slidr, elevr;

	lastvalue = *numvar;

	COLSPEC_SET_FRAMECOL(ObColor(elev), WHITE);
	draw_slider(tree, slid, elev, lastvalue, min, max, NULLFUNC);

	/* get extents */
	getrect(tree, slid, &slidr);
	getrect(tree, elev, &elevr);

	/* find mouse offset into elev, and adjust box for clip */
	Graf_mkstate(&m);
	xoffset = m.x - elevr.g_x - 1;

	/* While a button's down,
	 *   get the current elevator coordinate,
	 *   translate it to a value,
	 *   draw it if it changed
	 */
	while (m.buttons)
	{
		elevx = m.x - xoffset;
		if (elevx <= slidr.g_x)
			newvalue = min;
		else if (elevx >= slidr.g_x + slidr.g_w - elevr.g_w)
			newvalue = max;
		else
#define PIXELOFFSET ( (long)(elevx - slidr.g_x) )
			newvalue = min + (int)((VALUERANGE * PIXELOFFSET) / PIXELRANGE);

		if (newvalue != lastvalue)
		{
			lastvalue = newvalue;
			*numvar = newvalue;
			draw_slider(tree, slid, elev, lastvalue, min, max, foo);
		}
		Graf_mkstate(&m);
	}
	COLSPEC_SET_FRAMECOL(ObColor(elev), BLACK);
	draw_slider(tree, slid, elev, lastvalue, min, max, NULLFUNC);
}


static void sl_pagex(OBJECT *tree, int slid, int elev, int inc, int min, int max, int *numvar, void (*foo) (OBJECT *tree))
{
	MRETS m;
	GRECT slidr, elevr;
	int newval, dir;

	getrect(tree, slid, &slidr);
	getrect(tree, elev, &elevr);

	Graf_mkstate(&m);
	do
	{
		dir = (m.x < elevr.g_x + elevr.g_w / 2) ? -1 : 1;
		newval = *numvar + inc * dir;
		if (newval >= min && newval <= max)
		{
			*numvar = newval;
			draw_slider(tree, slid, elev, newval, min, max, foo);
			getrect(tree, elev, &elevr);
#ifdef __PUREC__
			evnt_timer(click_delay);
#else
			evnt_timer(click_delay, 0);
#endif
		}

		Graf_mkstate(&m);				/* check _after_ delay */
		if ((m.x >= elevr.g_x + elevr.g_w / 4) && (m.x < elevr.g_x + (3 * elevr.g_w) / 4))
		{
			sl_dragx(tree, slid, elev, min, max, numvar, foo);
			m.buttons = 0;
		}

		while (m.buttons && !xy_inrect(m.x, m.y, &slidr))
			Graf_mkstate(&m);

	} while (m.buttons);
}


static void draw_fills(OBJECT *tree)
{
	int i;

	for (i = FILL0; i <= FILL7; i++)
	{
		if (COLSPEC_GET_FRAMECOL(ObColor(i)) == WHITE)
		{
			COLSPEC_SET_FRAMECOL(ObColor(i), BLACK);
			ObjcDraw(tree, i, MAX_DEPTH, NULL);
		}
	}
	i = FILL0 + COLSPEC_GET_FILLPATTERN(curcolor);
	OBSPEC_SET_FRAMECOL(ObSpec(i), WHITE);
	ObjcDraw(tree, i, MAX_DEPTH, NULL);
}


static void draw_curborders(OBJECT *tree, GRECT *r)
{
	int borderobj = NIL;

	switch (curobj)
	{
	case BHSLID:
	case BLF:
		borderobj = BHELEV;
		break;

	case BRT:
		ObjcDraw(tree, BHSLID, MAX_DEPTH, r);
		borderobj = BGSIZER;
		break;

	case BHELEV:
	case BVELEV:
		break;

	case BFULLER:
	case BGNAME:
		borderobj = BGINFO;
		break;


	case BVSLID:
	case BUP:
		borderobj = BVELEV;
		break;

	case BDN:
		ObjcDraw(tree, BVSLID, MAX_DEPTH, r);
		borderobj = BGSIZER;
		break;


	case BGINFO:
		borderobj = BGVBAR;
		break;

	case BGVBAR:
	case BGHBAR:
		borderobj = BGSIZER;
		break;

	case BGSIZER:
		break;

	case TCLOSER:
	case TFULLER:
	case TNAME:
		borderobj = TINFO;
		break;

	case TINFO:
		borderobj = TUP;
		break;

	case TVSLID:
	case TUP:
		borderobj = TVELEV;
		break;

	case TDN:
		ObjcDraw(tree, TVSLID, MAX_DEPTH, r);
		borderobj = TSIZER;
		break;

	case TVELEV:
	case THELEV:
		break;

	case THSLID:
	case TLF:
		borderobj = THELEV;
		break;

	case TRT:
		ObjcDraw(tree, THSLID, MAX_DEPTH, r);
		borderobj = TSIZER;
		break;

	case TSIZER:
		break;
	}
	if (borderobj != NIL)
		ObjcDraw(tree, borderobj, MAX_DEPTH, r);

	if (ObNext(BGBOX) == FLOATER &&
		(curobj == TUP ||
		 curobj == THELEV ||
		 curobj == THSLID ||
		 curobj == TRT ||
		 curobj == TSIZER ||
		 curobj == TFULLER ||
		 curobj == TINFO))
		return;

	ObjcDraw(tree, FLOATER, MAX_DEPTH, r);
}


static void draw_curobj(OBJECT *tree)
{
	GRECT r, r1, r2, tr;
	int offset;

	/* fix and draw current object */
	if (curIsTed)
	{
		TedColor(curobj) = curcolor;
		if (TedBorder(curobj) < 0)
			offset = TedBorder(curobj);
	} else
	{
		ObColor(curobj) = curcolor;

		if (OBSPEC_GET_FRAMESIZE(ObSpec(curobj)) < 0)
			offset = OBSPEC_GET_FRAMESIZE(ObSpec(curobj));
	}

	getrect(tree, curobj, &r);
	Rc_center(&r, &r, offset, offset);

	if (AES_Version >= 0x0330 && ncolors > LWHITE)
	{
		r.g_x -= 2;
		r.g_y -= 2;
		r.g_w += 4;
		r.g_h += 4;
	}

	if (curobj == BGHBAR || curobj == BLF || curobj == BHELEV || curobj == BHSLID)
	{
		r1 = r2 = r;
		getrect(tree, TBOX, &tr);
		rc_intersect(&tr, &r2);
		r1.g_y += r2.g_h + WSHADOW;		/* TBOX shadow compensation */
		r1.g_h -= r2.g_h + WSHADOW;
		r.g_x += r2.g_w + WSHADOW;
		r.g_w -= r2.g_w + WSHADOW;
		r.g_h -= r1.g_h;
		ObjcDraw(tree, curobj, MAX_DEPTH, &r1);
		draw_curborders(tree, &r1);
		return;
	} else if ((curobj == BGNAME) || (curobj == BGINFO))
	{
		r2 = r;
		getrect(tree, TBOX, &tr);
		rc_intersect(&tr, &r2);
		r.g_x += r2.g_w + WSHADOW;		/* TBOX shadow compensation */
		r.g_w -= r2.g_w + WSHADOW;
	} else if (curobj == BRT)
	{
		/* Below TBOX */
		r1 = r2 = r;
		getrect(tree, TBOX, &tr);
		r1.g_y = tr.g_y + tr.g_h + WSHADOW;
		ObjcDraw(tree, curobj, MAX_DEPTH, &r1);
		draw_curborders(tree, &r1);

		/* Right of TBOX */
		r1 = r2 = r;
		getrect(tree, TBOX, &tr);
		r1.g_x = tr.g_x + tr.g_w + WSHADOW;
		ObjcDraw(tree, curobj, MAX_DEPTH, &r1);
		draw_curborders(tree, &r1);

		ObjcDraw(tree, TBOX, MAX_DEPTH, &r);
		return;
	}

	ObjcDraw(tree, curobj, MAX_DEPTH, &r);
	draw_curborders(tree, &r);
}


/*
 * fix elevator contents
 */
static void fix_elev(OBJECT *tree, int elev, int value)
{
	itoa2(value, TedText(elev));
	if (value == 0)
		COLSPEC_SET_TEXTCOL(TedColor(elev), BLACK);
	else
		COLSPEC_SET_TEXTCOL(TedColor(elev), WHITE);
	COLSPEC_SET_INTERIORCOL(TedColor(elev), value);
}


static void do_bordslid(OBJECT *tree)
{
	COLSPEC_SET_FRAMECOL(curcolor, curcborder);
	fix_elev(tree, BORDELEV, curcborder);
	draw_curobj(tree);
}


static void do_textslid(OBJECT *tree)
{
	COLSPEC_SET_TEXTCOL(curcolor, curctext);
	fix_elev(tree, TEXTELEV, curctext);
	draw_curobj(tree);
}


static void do_fillslid(OBJECT *tree)
{
	COLSPEC_SET_INTERIORCOL(curcolor, curcfill);
	fix_elev(tree, FILLELEV, curcfill);
	draw_curobj(tree);
}


static void set_info(OBJECT *tree)
{
	if (curIsTed)
	{
		curcolor = TedColor(curobj);
	} else
	{
		curcolor = ObColor(curobj);
	}
	draw_fills(tree);
	COLSPEC_SET_TEXTMODE(TedColor(MODE), COLSPEC_GET_TEXTMODE(curcolor));
	ObjcDraw(tree, MODE, MAX_DEPTH, NULL);

	curcborder = COLSPEC_GET_FRAMECOL(curcolor);
	fix_elev(tree, BORDELEV, curcborder);
	draw_slider(tree, BORDSLID, BORDELEV, curcborder, COLOR_MIN, COLOR_MAX, NULLFUNC);
	curctext = COLSPEC_GET_TEXTCOL(curcolor);
	fix_elev(tree, TEXTELEV, curctext);
	draw_slider(tree, TEXTSLID, TEXTELEV, curctext, COLOR_MIN, COLOR_MAX, NULLFUNC);
	curcfill = COLSPEC_GET_INTERIORCOL(curcolor);
	fix_elev(tree, FILLELEV, curcfill);
	draw_slider(tree, FILLSLID, FILLELEV, curcfill, COLOR_MIN, COLOR_MAX, NULLFUNC);
}


/* check for incompatible text/fill color based on available palette */
static int check_tcolor(int c)
{
	int cw;

	cw = c;

	if ((COLSPEC_GET_INTERIORCOL(cw) >= ncolors && COLSPEC_GET_TEXTCOL(cw) == BLACK) ||
		(COLSPEC_GET_TEXTCOL(cw) >= ncolors && COLSPEC_GET_INTERIORCOL(cw) == BLACK) ||
		(COLSPEC_GET_TEXTCOL(cw) >= ncolors && COLSPEC_GET_INTERIORCOL(cw) >= ncolors))
		COLSPEC_SET_TEXTCOL(cw, WHITE);

	return cw;
}


static int check_elcolor(int sl, int el)
{
	WORD slcw, elcw;

	slcw = sl;
	elcw = el;

	if (COLSPEC_GET_INTERIORCOL(elcw) != WHITE &&
		((COLSPEC_GET_INTERIORCOL(elcw) >= ncolors && COLSPEC_GET_INTERIORCOL(slcw) == BLACK) ||
		 (COLSPEC_GET_INTERIORCOL(slcw) >= ncolors && COLSPEC_GET_INTERIORCOL(elcw) == BLACK) ||
		 (COLSPEC_GET_INTERIORCOL(elcw) >= ncolors && COLSPEC_GET_INTERIORCOL(slcw) >= ncolors)))
		COLSPEC_SET_INTERIORCOL(elcw, WHITE);

	return elcw;
}


static void set_windows(void)
{
#define WDColor(obj) wind_set( 0, WF_DCOLOR, (obj), t, b, 0 )
	int t, b;

#ifdef TAKESYNC
	wind_update(BEG_UPDATE);
#endif

	t = check_tcolor(wcolors->name[0]);
	b = check_tcolor(wcolors->name[1]);
	WDColor(W_NAME);
	t = check_tcolor(wcolors->info[0]);
	b = check_tcolor(wcolors->info[1]);
	WDColor(W_INFO);
#if 0
	WDColor(W_BOX);
#endif
	t = check_tcolor(wcolors->sizer[0]);
	b = check_tcolor(wcolors->sizer[1]);
	WDColor(W_SIZER);

	t = -1;
	b = wcolors->vbar;
	WDColor(W_VBAR);
	b = wcolors->hbar;
	WDColor(W_HBAR);

	b = -1;

	wcolors->closer[1] = wcolors->fuller[1];	/* BackGrnd is same */
	t = check_tcolor(wcolors->closer[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->closer[1]);
	WDColor(W_CLOSER);

	t = check_tcolor(wcolors->fuller[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->fuller[1]);
	WDColor(W_FULLER);

	t = check_tcolor(wcolors->uparrow[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->uparrow[1]);
	WDColor(W_UPARROW);

	t = check_tcolor(wcolors->dnarrow[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->dnarrow[1]);
	WDColor(W_DNARROW);

	t = check_tcolor(wcolors->lfarrow[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->lfarrow[1]);
	WDColor(W_LFARROW);

	t = check_tcolor(wcolors->rtarrow[0]);
	if (AES_Version >= 0x0330)
		b = check_tcolor(wcolors->rtarrow[1]);
	WDColor(W_RTARROW);

	t = wcolors->vslid[0];
	if (AES_Version >= 0x0330)
		b = wcolors->vslid[1];
	WDColor(W_VSLIDE);

	t = check_elcolor(t, wcolors->velev[0]);
	if (AES_Version >= 0x0330)
		b = check_elcolor(b, wcolors->velev[1]);
	WDColor(W_VELEV);

	t = wcolors->hslid[0];
	if (AES_Version >= 0x0330)
		b = wcolors->hslid[1];
	WDColor(W_HSLIDE);

	t = check_elcolor(t, wcolors->helev[0]);
	if (AES_Version >= 0x0330)
		b = check_elcolor(t, wcolors->helev[1]);
	WDColor(W_HELEV);

#ifdef TAKESYNC
	wind_update(END_UPDATE);
#endif

}


static void set_wobjects(OBJECT *tree, WColors *wc)
{
	if (AES_Version >= 0x0330)
	{
#if 0
		ObColor(BCLOSER) = wc->closer[1];
#endif
		ObColor(BFULLER) = wc->fuller[1];
		ObColor(BUP) = wc->uparrow[1];
		ObColor(BDN) = wc->dnarrow[1];
		ObColor(BVSLID) = wc->vslid[1];
		ObColor(BVELEV) = wc->velev[1];
		ObColor(BLF) = wc->lfarrow[1];
		ObColor(BRT) = wc->rtarrow[1];
		ObColor(BHSLID) = wc->hslid[1];
		ObColor(BHELEV) = wc->helev[1];
	}

	TedColor(BGNAME) = wc->name[1];
	TedColor(BGINFO) = wc->info[1];
	ObColor(BGSIZER) = wc->sizer[1];

	ObColor(BGVBAR) = wc->vbar;
	ObColor(BGHBAR) = wc->hbar;

	ObColor(TCLOSER) = wc->closer[0];
	ObColor(TFULLER) = wc->fuller[0];

	TedColor(TNAME) = wc->name[0];
	TedColor(TINFO) = wc->info[0];

	ObColor(TUP) = wc->uparrow[0];
	ObColor(TDN) = wc->dnarrow[0];
	ObColor(TVSLID) = wc->vslid[0];
	ObColor(TVELEV) = wc->velev[0];
	ObColor(TLF) = wc->lfarrow[0];
	ObColor(TRT) = wc->rtarrow[0];
	ObColor(THSLID) = wc->hslid[0];
	ObColor(THELEV) = wc->helev[0];
	ObColor(TSIZER) = wc->sizer[0];

	ObjcDraw(tree, ROOT, MAX_DEPTH, NULL);
	set_info(tree);
}


static void set_wcolors(OBJECT *tree)
{
	if (AES_Version >= 0x0330)
	{
#if 0
		wcolors->closer[1] = ObColor(BCLOSER);
#endif
		wcolors->fuller[1] = ObColor(BFULLER);
		wcolors->uparrow[1] = ObColor(BUP);
		wcolors->dnarrow[1] = ObColor(BDN);
		wcolors->vslid[1] = ObColor(BVSLID);
		wcolors->velev[1] = ObColor(BVELEV);
		wcolors->lfarrow[1] = ObColor(BLF);
		wcolors->rtarrow[1] = ObColor(BRT);
		wcolors->hslid[1] = ObColor(BHSLID);
		wcolors->helev[1] = ObColor(BHELEV);
	}

	wcolors->name[1] = TedColor(BGNAME);
	wcolors->info[1] = TedColor(BGINFO);

	wcolors->vbar = ObColor(BGVBAR);
	wcolors->hbar = ObColor(BGHBAR);
	wcolors->sizer[1] = ObColor(BGSIZER);

	wcolors->closer[0] = ObColor(TCLOSER);
	wcolors->fuller[0] = ObColor(TFULLER);

	wcolors->name[0] = TedColor(TNAME);
	wcolors->info[0] = TedColor(TINFO);

	wcolors->uparrow[0] = ObColor(TUP);
	wcolors->dnarrow[0] = ObColor(TDN);
	wcolors->vslid[0] = ObColor(TVSLID);
	wcolors->velev[0] = ObColor(TVELEV);
	wcolors->lfarrow[0] = ObColor(TLF);
	wcolors->rtarrow[0] = ObColor(TRT);
	wcolors->hslid[0] = ObColor(THSLID);
	wcolors->helev[0] = ObColor(THELEV);
	wcolors->sizer[0] = ObColor(TSIZER);
}


static void drag_curobj(OBJECT *tree)
{
	MRETS m;
	int overobj, savecur;
	BOOLEAN saveIsTed;
	WORD savecol, lastcol;

	graf_mouse(FLAT_HAND, NULL);
	savecur = overobj = curobj;
	savecol = lastcol = curcolor;

	saveIsTed = curIsTed;
	do
	{
		if (overobj != curobj)
		{
			switch (overobj)
			{
			case BGNAME:
			case BGINFO:
			case BGVBAR:
			case BGHBAR:
			case BGSIZER:
			case TCLOSER:
			case TFULLER:
			case BFULLER:
			case TNAME:
			case TINFO:
			case TUP:
			case BUP:
			case TDN:
			case BDN:
			case TVSLID:
			case BVSLID:
			case TVELEV:
			case BVELEV:
			case TLF:
			case BLF:
			case TRT:
			case BRT:
			case THSLID:
			case BHSLID:
			case THELEV:
			case BHELEV:
			case TSIZER:
				if (curobj != NIL)
				{
					curcolor = lastcol;
					draw_curobj(tree);
					curcolor = savecol;
				}
				curobj = overobj;
				curIsTed = IsTed(curobj);
				if (curIsTed)
				{
					lastcol = TedColor(curobj);
				} else
				{
					lastcol = ObColor(curobj);
				}
				draw_curobj(tree);
				break;
			default:
				if (curobj != NIL)
				{
					curcolor = lastcol;
					draw_curobj(tree);
					curcolor = savecol;
					curobj = NIL;
				}
				break;
			}
		}
		Graf_mkstate(&m);
		overobj = objc_find(tree, ROOT, MAX_DEPTH, m.x, m.y);
	} while (m.buttons);
	graf_mouse(ARROW, NULL);
	curobj = savecur;
	curIsTed = saveIsTed;
}




#if 0
static void cgtest(void)
{
	char test[50];
	
	sprintf(test, "[1][WINDOW COLOR DEFAULTS][OK]");
	form_alert(1, test);

	sprintf(test, "[1][name0: %x][OK]", wcolors->name[0]);
	form_alert(1, test);

	sprintf(test, "[1][name1: %x][OK]", wcolors->name[1]);
	form_alert(1, test);

	sprintf(test, "[1][info0: %x][OK]", wcolors->info[0]);
	form_alert(1, test);

	sprintf(test, "[1][ info1: %x][OK]", wcolors->info[1]);
	form_alert(1, test);

	sprintf(test, "[1][sizer0: %x][OK]", wcolors->sizer[0]);
	form_alert(1, test);

	sprintf(test, "[1][sizer1: %x][OK]", wcolors->sizer[1]);
	form_alert(1, test);

	sprintf(test, "[1][closer0: %x][OK]", wcolors->closer[0]);
	form_alert(1, test);

	sprintf(test, "[1][closer1: %x][OK]", wcolors->closer[1]);
	form_alert(1, test);

	sprintf(test, "[1][fuller0: %x][OK]", wcolors->fuller[0]);
	form_alert(1, test);

	sprintf(test, "[1][fuller1: %x][OK]", wcolors->fuller[1]);
	form_alert(1, test);

	sprintf(test, "[1][vbar: %x][OK]", wcolors->vbar);
	form_alert(1, test);

	sprintf(test, "[1][uparrow0: %x][OK]", wcolors->uparrow[0]);
	form_alert(1, test);

	sprintf(test, "[1][uparrow1: %x][OK]", wcolors->uparrow[1]);
	form_alert(1, test);

	sprintf(test, "[1][dnarrow0: %x][OK]", wcolors->dnarrow[0]);
	form_alert(1, test);

	sprintf(test, "[1][dnarrow1: %x][OK]", wcolors->dnarrow[1]);
	form_alert(1, test);

	sprintf(test, "[1][vslid0: %x][OK]", wcolors->vslid[0]);
	form_alert(1, test);

	sprintf(test, "[1][vslid1: %x][OK]", wcolors->vslid[1]);
	form_alert(1, test);

	sprintf(test, "[1][velev0: %x][OK]", wcolors->velev[0]);
	form_alert(1, test);

	sprintf(test, "[1][velev1: %x][OK]", wcolors->velev[1]);
	form_alert(1, test);

	sprintf(test, "[1][hbar: %x][OK]", wcolors->hbar);
	form_alert(1, test);

	sprintf(test, "[1][lfarrow0: %x][OK]", wcolors->lfarrow[0]);
	form_alert(1, test);

	sprintf(test, "[1][lfarrow1: %x][OK]", wcolors->lfarrow[1]);
	form_alert(1, test);

	sprintf(test, "[1][rtarrow0: %x][OK]", wcolors->rtarrow[0]);
	form_alert(1, test);

	sprintf(test, "[1][rtarrow1: %x][OK]", wcolors->rtarrow[1]);
	form_alert(1, test);

	sprintf(test, "[1][hslid0: %x][OK]", wcolors->hslid[0]);
	form_alert(1, test);

	sprintf(test, "[1][hslid1: %x][OK]", wcolors->hslid[1]);
	form_alert(1, test);

	sprintf(test, "[1][helev0: %x][OK]", wcolors->helev[0]);
	form_alert(1, test);

	sprintf(test, "[1][helev1: %x][OK]", wcolors->helev[1]);
	form_alert(1, test);
}
#endif


static void do_form(OBJECT *tree)
{
	GRECT oldfloat, newfloat;
	int exitobj;
	int ox, oy;
	int newpattern;
	int offset;
	MRETS m;
	BOOLEAN shuffled;
	BOOLEAN quit = FALSE;
	GRECT rect;

	click_delay = dcrates[evnt_dclick(0, 0)];
	while (!quit)
	{
		WORD msg[8];

#if PROGRAM
		exitobj = form_do(tree, 0);
		msg[0] = 0;
#else
		exitobj = xcpb->Xform_do(tree, 0, msg);
#endif
		shuffled = FALSE;
		/*
		 * If window was moved, screwit... redraw all the
		 * sliders, from the bottom up.  Otherwise, the fill pattern
		 * under the sliders get screwed.
		 */
		if (!rc_equal(&formrect, &ObRect(ROOT)))
		{
			formrect = ObRect(ROOT);
			ObjcDraw(tree, BORDSLID, MAX_DEPTH, NULL);
			ObjcDraw(tree, TEXTSLID, MAX_DEPTH, NULL);
			ObjcDraw(tree, FILLSLID, MAX_DEPTH, NULL);
		}

		if (exitobj == NIL)
		{
			switch (msg[0])
			{
			case WM_CLOSED:
				exitobj = OK;
				break;
			case AC_CLOSE:
				exitobj = CANCEL;
				break;
			case CT_KEY:
				switch (msg[3])
				{
				case K_UNDO:
					/* undo changes */
					set_wobjects(tree, wcolors);
					break;
				case K_HOME:
					/* set defaults */
					set_wobjects(tree, &defcolors);
					break;
				case K_F1:
				case K_F2:
				case K_F3:
				case K_F4:
				case K_F5:
				case K_F6:
				case K_F7:
				case K_F8:
				case K_F9:
				case K_F10:
					/* set preset */
					set_wobjects(tree, &altcolors[(msg[3] >> 8) - (K_F1 >> 8)]);
					break;
#if 0
				case KA_R:
#endif
				case K_INSERT:
					/* restore */
					set_wobjects(tree, &savecolors[saveindex]);
					break;
				case KA_G:
					/* spanish save */
					if (xcpb->Country_Code == COUNTRY_ES)
					{
						Select(SAVE);
						ObjcDraw(tree, SAVE, MAX_DEPTH, NULL);
						exitobj = SAVE;
						quit = TRUE;
					}
					break;
				case KA_S:
					/* all other countries save */
					if (xcpb->Country_Code != COUNTRY_ES)
					{
						Select(SAVE);
						ObjcDraw(tree, SAVE, MAX_DEPTH, NULL);
						exitobj = SAVE;
						quit = TRUE;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}

		}
		if (exitobj != NIL)
		{
			exitobj &= 0x7fff;
			switch (exitobj)
			{
			case SAVE:
				/*
				 * save current settings to disk
				 */
#if PROGRAM
#else
				if (xcpb->XGen_Alert(SAVE_DEFAULTS))
				{
					set_wcolors(tree);
					set_windows();
					savecolors[saveindex] = *wcolors;
					xcpb->CPX_Save(&savecolors, sizeof(savecolors));
				} else
				{
#if 0
					cgtest();
#endif
					quit = FALSE;
				}
#endif
				Deselect(SAVE);
				rect = ObRect(SAVE);
				objc_offset(tree, SAVE, &rect.g_x, &rect.g_y);
				rect.g_x -= 2;
				rect.g_y -= 2;
				rect.g_w += 4;
				rect.g_h += 4;
				ObjcDraw(tree, SAVE, MAX_DEPTH, &rect);
				break;

			case OK:
				/*
				 * set window colors
				 */
				set_wcolors(tree);
				set_windows();
				/* fall through */

			case CANCEL:
				quit = TRUE;
				Deselect(exitobj);
				break;

			case BGNAME:
			case BGINFO:
			case BGVBAR:
			case BGHBAR:
			case BGSIZER:
			case BFULLER:
			case BUP:
			case BDN:
			case BVSLID:
			case BVELEV:
			case BLF:
			case BRT:
			case BHSLID:
			case BHELEV:
				/*
				 * set up object tree so that FLOATER appears under TBOX
				 */
				shuffled = TRUE;
				ObNext(BGBOX) = FLOATER;

				ObNext(FLOATER) = TBOX;
				ObNext(TBOX) = ROOT;
				ObTail(ROOT) = TBOX;
				/* fall through */
			case TCLOSER:
			case TFULLER:
			case TNAME:
			case TINFO:
			case TUP:
			case TDN:
			case TVSLID:
			case TVELEV:
			case TLF:
			case TRT:
			case THSLID:
			case THELEV:
			case TSIZER:
				if (!shuffled)
				{
					/*
					 * set up object tree so that FLOATER appears over TBOX
					 */
					ObNext(BGBOX) = TBOX;

					ObNext(TBOX) = FLOATER;
					ObNext(FLOATER) = ROOT;
					ObTail(ROOT) = FLOATER;
				}
				/*
				 * Save old floater position for redraw,
				 * set new floater position from selected object
				 */
				getrect(tree, FLOATER, &oldfloat);
				offset = 0;
				if (OBSPEC_GET_FRAMESIZE(ObSpec(FLOATER)) < 0)
					offset = OBSPEC_GET_FRAMESIZE(ObSpec(FLOATER));
				Rc_center(&oldfloat, &oldfloat, offset, offset);

				curobj = exitobj;

				curIsTed = IsTed(curobj);

				getrect(tree, curobj, &ObRect(FLOATER));
				objc_offset(tree, ROOT, &ox, &oy);
				ObX(FLOATER) -= ox;
				ObY(FLOATER) -= oy;

				if (AES_Version >= 0x0330 && ncolors > LWHITE)
				{
					/* Adjust only if we are 3D */
					if (ObFlags(curobj) & OF_FL3DIND)
					{
						ObX(FLOATER) -= 2;
						ObY(FLOATER) -= 2;
						ObW(FLOATER) += 4;
						ObH(FLOATER) += 4;
					}
				}

				Rc_center(&ObRect(FLOATER), &ObRect(FLOATER), -WSHADOW, -WSHADOW);
				getrect(tree, FLOATER, &newfloat);

				Rc_center(&newfloat, &newfloat, offset, offset);

				/*
				 * get color word settings, set sliders, redraw center box
				 */
				set_info(tree);
				/*
				 * undraw old floater, draw new floater
				 */
				ObjcDraw(tree, ROOT, MAX_DEPTH, &oldfloat);
				ObjcDraw(tree, ROOT, MAX_DEPTH, &newfloat);
				wait_bup();
				break;

			case FILL0:
			case FILL1:
			case FILL2:
			case FILL3:
			case FILL4:
			case FILL5:
			case FILL6:
			case FILL7:
				/*
				 * "slide" through fill settings while button is down
				 * setting fill pattern as it changes
				 */
				objc_offset(tree, exitobj, &ox, &oy);
				do
				{
					newpattern = exitobj - FILL0;
					if (COLSPEC_GET_FILLPATTERN(curcolor) != newpattern)
					{
						COLSPEC_SET_FILLPATTERN(curcolor, newpattern);
						if (curIsTed)
							COLSPEC_SET_FILLPATTERN(TedColor(curobj), newpattern);
						else
							COLSPEC_SET_FILLPATTERN(ObColor(curobj), newpattern);
						draw_fills(tree);
						draw_curobj(tree);
					}
					Graf_mkstate(&m);
					exitobj = objc_find(tree, FILLS, MAX_DEPTH, m.x, oy);
					if (exitobj == NIL)
						exitobj = COLSPEC_GET_FILLPATTERN(curcolor) + FILL0;
				} while (m.buttons);
				break;

			case MODE:
				/*
				 * Toggle replace/transparent mode
				 */
				if (COLSPEC_GET_TEXTMODE(TedColor(MODE)))
					COLSPEC_SET_TEXTMODE(TedColor(MODE), 0);
				else
					COLSPEC_SET_TEXTMODE(TedColor(MODE), 1);
				COLSPEC_SET_TEXTMODE(curcolor, COLSPEC_GET_TEXTMODE(TedColor(MODE)));
				if (curIsTed)
					COLSPEC_SET_TEXTMODE(TedColor(curobj), COLSPEC_GET_TEXTMODE(curcolor));
				else
					COLSPEC_SET_TEXTMODE(ObColor(curobj), COLSPEC_GET_TEXTMODE(curcolor));
				ObjcDraw(tree, MODE, MAX_DEPTH, NULL);
				draw_curobj(tree);
				wait_bup();
				break;

			case BORDSLID:
				sl_pagex(tree, BORDSLID, BORDELEV, 1, COLOR_MIN, COLOR_MAX, &curcborder, do_bordslid);
				break;

			case BORDELEV:
				fix_elev(tree, BORDELEV, curcborder);
				sl_dragx(tree, BORDSLID, BORDELEV, COLOR_MIN, COLOR_MAX, &curcborder, do_bordslid);
				break;

			case TEXTSLID:
				sl_pagex(tree, TEXTSLID, TEXTELEV, 1, COLOR_MIN, COLOR_MAX, &curctext, do_textslid);
				break;

			case TEXTELEV:
				fix_elev(tree, TEXTELEV, curctext);
				sl_dragx(tree, TEXTSLID, TEXTELEV, COLOR_MIN, COLOR_MAX, &curctext, do_textslid);
				break;

			case FILLSLID:
				sl_pagex(tree, FILLSLID, FILLELEV, 1, COLOR_MIN, COLOR_MAX, &curcfill, do_fillslid);
				break;

			case FILLELEV:
				fix_elev(tree, FILLELEV, curcfill);
				sl_dragx(tree, FILLSLID, FILLELEV, COLOR_MIN, COLOR_MAX, &curcfill, do_fillslid);
				break;

			case FLOATER:
				/*
				 * "drag" current object's settings to another object
				 */
				drag_curobj(tree);
				break;

			default:
				/*
				 * do nothing
				 */
				break;
			}
		}
	}
}


#if PROGRAM
void main(void)
{
	OBJECT *tree;
	
	appl_init();
	if (!rsrc_load("WCOLORS.RSC"))
	{
		appl_exit();
		return;
	}
	rsrc_gaddr(R_TREE, WCOLPANL, &tree);
	FormDial(FMD_START, &norect, &norect);
	FormCenter(tree, &ObRect(ROOT));
	graf_mouse(ARROW, NULL);

	init_tree(tree);

	ObjcDraw(tree, ROOT, MAX_DEPTH, NULL);
	curobj = TCLOSER;
	curIsTed = FALSE;
	set_info();
	do_form(tree);

	FormDial(FMD_FINISH, &norect, &norect);
	rsrc_free();
	appl_exit();
}

#else

/*
 * Set text fields in resource
 */
static void set_texts(OBJECT *tree, int country)
{
	OBJECT *Xtree = (OBJECT *) rs_trindex[XLATE];
	int i;

#define XText(obj,string) TedText((obj)) = Xtree[(string)].ob_spec.free_string
#define XString(obj,string) ObString((obj)) = Xtree[(string)].ob_spec.free_string

	switch (country)
	{
	case COUNTRY_DE:
		XText(TNAME, FRGWIND);
		XText(TINFO, FRGINFO);
		XText(BORD, FRGBORD);
		XText(TEXT, FRGTEXT);
		XText(FILL, FRGFILL);
		XText(MODE, FRGMODE);
		XString(OK, FRGOK);
		XString(CANCEL, FRGCAN);
		XString(SAVE, FRGSAVE);
		break;
	case COUNTRY_FR:
		XText(TNAME, FRAWIND);
		XText(TINFO, FRAINFO);
		XText(BORD, FRABORD);
		XText(TEXT, FRATEXT);
		XText(FILL, FRAFILL);
		XText(MODE, FRAMODE);
		XString(OK, FRAOK);
		XString(CANCEL, FRACAN);
		XString(SAVE, FRASAVE);
		break;
	case COUNTRY_ES:
		XText(TNAME, SPAWIND);
		XText(TINFO, SPAINFO);
		XText(BORD, SPABORD);
		XText(TEXT, SPATEXT);
		XText(FILL, SPAFILL);
		XText(MODE, SPAMODE);
		XString(OK, SPAOK);
		XString(CANCEL, SPACAN);
		XString(SAVE, SPASAVE);
		break;
	case COUNTRY_IT:
		XText(TNAME, ITAWIND);
		XText(TINFO, ITAINFO);
		XText(BORD, ITABORD);
		XText(TEXT, ITATEXT);
		XText(FILL, ITAFILL);
		XText(MODE, ITAMODE);
		XString(OK, ITAOK);
		XString(CANCEL, ITACAN);
		XString(SAVE, ITASAVE);
		break;
	case COUNTRY_SE:
		XText(TNAME, SWEWIND);
		XText(TINFO, SWEINFO);
		XText(BORD, SWEBORD);
		XText(TEXT, SWETEXT);
		XText(FILL, SWEFILL);
		XText(MODE, SWEMODE);
		XString(OK, SWEOK);
		XString(CANCEL, SWECAN);
		XString(SAVE, SWESAVE);
		break;
	default:
		/* Default case is USA/UK */
		break;
	}
	i = (int) strlen(TedText(TNAME));
	TedText(BGNAME) = &TedText(TNAME)[i - 4];
	i = (int) strlen(TedText(TINFO));
	TedText(BGINFO) = &TedText(TINFO)[i - 4];
}


#if TESTXLATE
static void cycle_country(int *country)
{
	switch (*country)
	{
	case COUNTRY_DE:
		*country = COUNTRY_FR;
		form_alert(1, "[0][ France ][OK]");
		break;
	case COUNTRY_FR:
		*country = COUNTRY_ES;
		form_alert(1, "[0][ Spain ][OK]");
		break;
	case COUNTRY_ES:
		*country = COUNTRY_IT;
		form_alert(1, "[0][ Italy ][OK]");
		break;
	case COUNTRY_IT:
		*country = COUNTRY_SE;
		form_alert(1, "[0][ Sweden ][OK]");
		break;
	case COUNTRY_SE:
		*country = COUNTRY_US;
		form_alert(1, "[0][ USA ][OK]");
		break;
	case COUNTRY_US:
		*country = COUNTRY_DE;
		form_alert(1, "[0][ Germany ][OK]");
		break;
	}
}
#endif


/*
 * CPX user interaction
 */
static BOOLEAN cdecl cpx_call(GRECT *rect)
{
	OBJECT *tree;
#if PROGRAM
	rsrc_gaddr(R_TREE, WCOLPANL, &tree);
#else
	tree = (OBJECT *) rs_trindex[WCOLPANL];
#endif
	formrect = *rect;

	ObX(ROOT) = rect->g_x;
	ObY(ROOT) = rect->g_y;

	set_texts(tree, xcpb->Country_Code);

	curobj = TCLOSER;
	curIsTed = FALSE;

	if (!xcpb->SkipRshFix)
		init_tree(tree);

	set_wobjects(tree, wcolors);
	do_form(tree);
	return FALSE;
}


/*
 * Initialize CPX
 */
CPXINFO *cdecl cpx_init(XCPB * Xcpb)
{
	int work_out[57];
	
	xcpb = Xcpb;

	appl_init();
	AES_Version = _GemParBlk.global[0];
	canSetColors = AES_Version >= 0x0300;

#if TESTXLATE
	cycle_country(&xcpb->Country_Code);
#endif

	vq_extnd(xcpb->handle, 0, work_out);
	ncolors = work_out[13];
	saveindex = 0;
	if (ncolors > 2)
		++saveindex;
	if (ncolors > 4)
		++saveindex;
	wcolors = (WColors *) (xcpb->Get_Buffer());

	if (xcpb->booting)
	{
		if (canSetColors)
		{
			/* Read in defaults */
			*wcolors = savecolors[saveindex];
			set_windows();
		}
	} else
	{
		if (!xcpb->SkipRshFix)
		{
			xcpb->rsh_fix(NUM_OBS, NUM_FRSTR, NUM_FRIMG, NUM_TREE, rs_object,
						  rs_tedinfo, rs_strings, rs_iconblk, rs_bitblk, rs_frstr, rs_frimg, rs_trindex, rs_imdope);
		}

		if (canSetColors)
		{
			cpxinfo.cpx_call = cpx_call;
			cpxinfo.cpx_draw = NULL;
			cpxinfo.cpx_wmove = NULL;
			cpxinfo.cpx_timer = NULL;
			cpxinfo.cpx_key = NULL;
			cpxinfo.cpx_button = NULL;
			cpxinfo.cpx_m1 = NULL;
			cpxinfo.cpx_m2 = NULL;
			cpxinfo.cpx_hook = NULL;
			cpxinfo.cpx_close = NULL;

		} else
		{
#define XAlert(string) form_alert(1, (char *)rs_frstr[(string)])
			switch (xcpb->Country_Code)
			{
			case COUNTRY_DE:
				XAlert(FRGLERT);
				break;
			case COUNTRY_FR:
				XAlert(FRALERT);
				break;
			case COUNTRY_ES:
				XAlert(SPALERT);
				break;
			case COUNTRY_IT:
				XAlert(ITALERT);
				break;
			case COUNTRY_SE:
				XAlert(SWELERT);
				break;
			default:
				XAlert(ALERT);
				break;
			}
			return NULL;
		}
	}
	return &cpxinfo;
}

#endif
