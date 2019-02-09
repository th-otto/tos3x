#include "aes.h"
#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"

#if TP_WINX /* whole file */

#include "crysbind.h"

#define DROP_SIZE 2						/* Windows have drop shadows */

#undef WS_FULL
#undef WS_WORK
#undef WS_PREV
#undef WS_CURR
#undef WS_TRUE
#define WS_FULL 0
#define WS_WORK 1
#define WS_PREV 2
#define WS_CURR 3

STATIC BOOLEAN w_walkflag;
struct _winxvars winxvars;


/* 306de/winx22: 00e21e68 */
int16_t wx_create(P(uint16_t) kind, P(GRECT *) rect)
PP(uint16_t kind;)						/* kind of window to be created */
PP(GRECT *rect;)						/* x, y, width and height of full size window */
{
	return 0;
}


/* 306de/winx22: 00e21f78 */
int16_t wx_open(P(int16_t) w_handle, P(GRECT *) rect)
PP(int16_t w_handle;)					/* handle of window to be opened */
PP(GRECT *rect;)						/* x, y, width and height of opened window */
{
	return 0;
}


/* 306de/winx22: 00e22004 */
int16_t wx_close(P(int16_t) w_handle)
PP(int16_t w_handle;)							/* handle of window to be closed */
{
	return 0;
}


/* 306de/winx22: 00e21fdc */
int16_t wx_delete(P(int16_t) w_handle)
PP(int16_t w_handle;)							/* handle of window to be deleted */
{
	return 0;
}


/* 306de/winx22: 00e21220 */
static WX_MEM *wx_gmem(NOTHING)
{
	return winxvars.wx_mem;
}


/* 306de/winx22: 00e21228 */
static WX_WINDOW *wx_getwin(P(int16_t) handle)
PP(int16_t handle;)
{
	return &wx_gmem()->win[handle];
}


/* 306de/winx22: e21236 */
static BOOLEAN wx_chkglobl(P(uint16_t) mask)
PP(uint16_t mask;)
{
	return (winxvars.global_flags & mask) != 0;
}


/* 306de/winx22: e2124e */
static BOOLEAN wx_chkpd(P(WX_PD *) pd, P(uint16_t) mask)
PP(WX_PD *pd;)
PP(uint16_t mask;)
{
	return (pd->p_flags & mask) != 0;
}


/* 306de/winx22: e21246 */
static BOOLEAN wx_chkwin(P(int16_t) handle, P(uint16_t) mask)
PP(int16_t handle;)
PP(uint16_t mask;)
{
	return wx_chkpd((WX_PD *)wx_getwin(handle)->w_owner, mask);
}


/* 306de/winx22: e213aa */
static BOOLEAN wx_bgctrl(P(int16_t) handle)
PP(int16_t handle;)
{
	if (handle == gl_wtop)
		return TRUE;
	return wx_chkwin(handle, WX_BGCTRL);
}


/* 306de/winx22: e2123e */
static BOOLEAN wx_chkcurr(P(uint16_t) mask)
PP(uint16_t mask;)
{
	return wx_chkpd((WX_PD *)rlr, mask);
}


/* 306de/winx22: e21266 */
static VOID wx_grect(P(int16_t) handle, P(int16_t) which, P(GRECT *) work)
PP(int16_t handle;)
PP(int16_t which;)
PP(register GRECT *work;)
{
	register GRECT *gr;
	register WX_WINDOW *wp;
	
	wp = wx_getwin(handle);
	switch (which)
	{
	case 0:
		gr = &wp->w_full;
		break;
	case WS_WORK:
	case -WS_WORK:
		gr = &wp->w_work;
		break;
	case WS_PREV:
	case -WS_PREV:
		gr = &wp->w_prev;
		break;
	case WS_CURR:
	case -WS_CURR:
		gr = &wp->w_curr;
		break;
	default:
		return;
	}
	rc_copy(gr, work);
	if (which < 0 && work->g_w != 0 && work->g_h != 0)
	{
		work->g_w += DROP_SIZE;
		work->g_h += DROP_SIZE;
	}
}


/* 306de/winx22: e2128c */
static VOID wx_get_globals(P(GRECT *) work)
PP(GRECT *work;)
{
	register WX_WINDOW *wp;
	
	wp = wx_getwin(DESK);
	rc_copy(&gl_rfull, &wp->w_full);
	rc_copy(&gl_rscreen, &wp->w_curr);
	rc_copy(&gl_rscreen, &wp->w_prev);
	if (work)
		rc_copy(work, &wp->w_work);
}


/* 306de/winx22: 00e22aa4 */
static VOID wx_error(P(int16_t) op)
PP(int16_t op;)
{
}


/* 306de/winx22: 00e21fee */
static BOOLEAN wx_chkuse(P(int16_t) w_handle)
PP(int16_t w_handle;)							/* handle of window to be deleted */
{
	if ((uint16_t)w_handle >= WX_MAXWIN || (!wx_getwin(w_handle)->w_flags & VF_INUSE))
		return FALSE;
	return TRUE;
}


/* 306de/winx22: 00e21e22 */
static VOID wx_owns(P(int16_t) w_handle, P(ORECT *) po, P(GRECT *)poutwds)
PP(register int16_t w_handle;)
PP(register ORECT *po;)
PP(register GRECT *poutwds;)
{
	GRECT t;
	
	wx_grect(w_handle, WS_WORK, &t);
	/* TODO */
}


/* 306de/winx22: 00e2227a */
static VOID wx_top(P(int16_t) w_handle, P(int16_t *)poutwds)
PP(register int16_t w_handle;)
PP(register int16_t *poutwds;)
{
	/* TODO */
}


/* 306de/winx22: 00e222b4 */
int16_t wx_get(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t *)poutwds, P(const int16_t *) iw)
PP(register int16_t w_handle;)						/* window handle */
PP(int16_t w_field;)								/* flag to identify what info to be returned */
PP(register int16_t *poutwds;)						/* return values */
PP(const int16_t *iw;)
{
	register WX_WINDOW *pwin = 0;
	register int16_t ret = TRUE;
	register int16_t which = 0;
	
	poutwds[0] = 0;
	poutwds[1] = 0;
	poutwds[2] = 0;
	poutwds[3] = 0;
	
	if ((uint16_t)w_field <= WF_BOTTOM)
	{
		/*
		 * check for codes that need a valid window handle
		 */
		if (((1L << WF_WORKXYWH) |
		     (1L << WF_CURRXYWH) |
		     (1L << WF_PREVXYWH) |
		     (1L << WF_FULLXYWH) |
		     (1L << WF_HSLIDE) |
		     (1L << WF_VSLIDE) |
		     (1L << WF_FIRSTXYWH) |
		     (1L << WF_NEXTXYWH) |
		     (1L << WF_HSLSIZE) |
		     (1L << WF_VSLSIZE) |
		     (1L << WF_COLOR) |
		     (1L << WF_OWNER) |
		     (1L << WF_BEVENT)
			) & (1L << w_field))
		{
			if (!wx_chkuse(w_handle))
			{
				if (w_field != WF_OWNER)
					wx_error(WIND_GET);
				w_field = 0;
			}
 			pwin = wx_getwin(w_handle);
		}
	}
	
	switch (w_field)
	{
	default:
	case 0:
	case WF_KIND:
	case WF_NAME:
	case WF_INFO:
		ret = FALSE;
		break;
	case WF_CURRXYWH:
		which++;
		/* fall through */
	case WF_PREVXYWH:
		which++;
		/* fall through */
	case WF_WORKXYWH:
		which++;
		/* fall through */
	case WF_FULLXYWH:
		wx_grect(w_handle, which, (GRECT *)poutwds);
		break;
	case WF_VSLSIZE:
		poutwds[0] = pwin->w_vslsiz;
		break;
	case WF_HSLSIZE:
		poutwds[0] = pwin->w_hslsiz;
		break;
	case WF_VSLIDE:
		poutwds[0] = pwin->w_vslide;
		break;
	case WF_HSLIDE:
		poutwds[0] = pwin->w_hslide;
		break;
	case WF_TOP:
		poutwds[0] = gl_wtop < 0 ? DESK : gl_wtop;
		wx_top(gl_wtop, poutwds);
		break;
	case WF_FIRSTXYWH:
	case WF_NEXTXYWH:
		wx_owns(w_handle, w_field == WF_FIRSTXYWH ? pwin->w_rlist : pwin->w_rnext, (GRECT *)poutwds);
		break;
	case WF_SCREEN:
		gsx_mret((VOIDPTR *)poutwds, (int32_t *)(poutwds + 2));
		break;
	case WF_NEWDESK:
		*((LPTREE *)poutwds) = gl_newdesk;
		poutwds[2] = gl_newroot;
		break;
	case WF_BEVENT:
		poutwds[0] = (pwin->w_flags >> 8) & 0xff;
		break;
	case WF_COLOR:
		which = iw[0];
		poutwds[0] = which;
		poutwds[1] = pwin->w_tcolor[which];
		poutwds[2] = pwin->w_bcolor[which];
		break;
	case WF_DCOLOR:
		which = iw[0];
		{
			int16_t *colors = wx_gmem()->tcolor;
			poutwds[0] = which;
			poutwds[1] = colors[which];
			poutwds[2] = colors[which + WX_MAXOBJ];
		}
		break;

	case WF_WINX:
		poutwds[0] = winxvars.version;
		poutwds[1] = winxvars.date;
		*((intptr_t *)&poutwds[2]) = (intptr_t) winxvars.winxfunc;
		ret = w_field;
		break;
	case WF_WINXCFG:
		poutwds[0] = 0x1fff;
		poutwds[1] = winxvars.global_flags;
		poutwds[2] = 0xfff;
		poutwds[3] = ((WX_PD *)rlr)->p_flags;
		ret = w_field;
		break;
	case WF_DDELAY:
		{
			WX_MEM *mem = wx_gmem();
			poutwds[0] = mem->init_scroll;
			poutwds[1] = mem->cont_scroll;
			poutwds[2] = mem->cont_close;
			poutwds[3] = mem->cont_full;
		}
		ret = w_field;
		break;
		
	/*
	 * the patching version redirects appl_getinfo() calls to wm_get; so do we
	 */
	case WF_APG11:
		poutwds[0] = 0x7f;
		poutwds[3] = 1;
		break;
	case WF_APG12:
		if (wx_chkglobl(WX_FGBACK))
			poutwds[0] |= 0x40;
		if (wx_chkcurr(WX_ONTOP))
			poutwds[0] |= 0x6;
		break;
	}

	return ret;
}


/* 306de/winx22: 00e218e8 */
static VOID do_walk(P(int16_t) wh, P(int16_t) which, P(int16_t) obj, P(int16_t) depth, P(OBJECT *) tree, P(GRECT *) pc)
PP(register int16_t wh;)
PP(int16_t which;)
PP(int16_t obj;)
PP(int16_t depth;)
PP(OBJECT *tree;)
PP(register GRECT *pc;)
{
}


/* 306de/winx22: 00e21c82 */
static VOID draw_top(P(int16_t) wh, P(int16_t) which)
PP(register int16_t wh;)
PP(int16_t which;)
{
}


/* 306de/winx22: 00e21bb0 */
static VOID draw_change(P(int16_t) w_handle, P(GRECT *) pt)
PP(register int16_t w_handle;)
PP(register GRECT *pt;)
{
}


static VOID wx_setcolor(P(int16_t *) colors, P(int16_t) idx, P(int16_t) color)
PP(int16_t *colors;)
PP(int16_t idx;)
PP(int16_t color;)
{
	if (color != -1)
		colors[idx] = color;
	colors[W_UPGRIP] = colors[W_UPARROW];
	colors[W_DNGRIP] = colors[W_DNARROW];
	colors[W_LFGRIP] = colors[W_LFARROW];
	colors[W_RTGRIP] = colors[W_RTARROW];
}


/* 306de/winx22: 00e22434 */
int16_t wx_set(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t *) pinwds)
PP(register int16_t w_handle;)						/* window handle */
PP(register int16_t w_field;)						/* flag to identify what info to be changed */
PP(register int16_t *pinwds;)						/* values to change to */
{
	register WX_WINDOW *pwin = 0;
	register int16_t ret = TRUE;
	register int16_t which = 0;
	register int16_t *pint;
	register int16_t kind;
	WX_MEM *mem = wx_gmem();
	
	wx_update(BEG_UPDATE);
	if ((uint16_t)w_field <= WF_BOTTOM)
	{
		/*
		 * check for codes that need a valid window handle
		 */
		if (((1L << WF_NAME) |
		     (1L << WF_INFO) |
		     (1L << WF_CURRXYWH) |
		     (1L << WF_HSLIDE) |
		     (1L << WF_VSLIDE) |
		     (1L << WF_TOP) |
		     (1L << WF_HSLSIZE) |
		     (1L << WF_VSLSIZE) |
		     (1L << WF_COLOR) |
		     (1L << WF_BEVENT) |
		     (1L << WF_BOTTOM)
			) & (1L << w_field))
		{
			if (!wx_chkuse(w_handle))
			{
				wx_error(WIND_SET);
				w_field = 0;
			}
 			pwin = wx_getwin(w_handle);
		}
	}
	
	switch (w_field)
	{
	default:
	case 0:
	case WF_KIND:
	case WF_WORKXYWH:
	case WF_PREVXYWH:
	case WF_FULLXYWH:
	case WF_FIRSTXYWH:
	case WF_NEXTXYWH:
	case WF_SCREEN:
	case WF_OWNER:
	case WF_WINX:
	case WF_WINXCFG:
	case WF_APG11:
	case WF_APG12:
		ret = FALSE;
		break;
	
	case WF_NAME:
		which = W_NAME;
		pwin->w_pname = *(char **)pinwds;
		if (pwin->w_kind & NAME)
			if (pwin->w_flags & VF_INTREE)
				do_walk(w_handle, which, -1, -1, NULL, NULL);
		break;
	
	case WF_INFO:
		which = W_INFO;
		pwin->w_pinfo = *(char **)pinwds;
		if (pwin->w_kind & INFO)
			if (pwin->w_flags & VF_INTREE)
				do_walk(w_handle, which, -1, -1, NULL, NULL);
		break;

	case WF_TOP:
		which = -1;
		/* fall through */
	case WF_BOTTOM:
		if (w_handle == DESK)
		{
			wx_error(WIND_SET);
		} else
		{
			if (pwin->w_flags & VF_INTREE)
				draw_top(w_handle, which);
		}
		break;
		
	case WF_CURRXYWH:
		if (w_handle == DESK || !(pwin->w_flags & VF_INTREE))
			wx_error(WIND_SET);
		else
			draw_change(w_handle, (GRECT *)pinwds);
		break;

	case WF_VSLSIZE:
		pint = &pwin->w_vslsiz;
		which = W_VSLIDE;
		kind = VSLIDE;
		goto set_int;
	case WF_HSLSIZE:
		pint = &pwin->w_hslsiz;
		which = W_HSLIDE;
		kind = HSLIDE;
		goto set_int;
	case WF_VSLIDE:
		pint = &pwin->w_vslide;
		which = W_VSLIDE;
		kind = VSLIDE;
		goto set_int;
	case WF_HSLIDE:
		pint = &pwin->w_hslide;
		which = W_HSLIDE;
		kind = HSLIDE;
	set_int:
		w_field = pinwds[0];
		if (w_field < -1)
			w_field = -1;
		else if (w_field > 1000)
			w_field = 1000;
		if (*pint != w_field)
		{
			*pint = w_field;
			if (pwin->w_kind & kind)
				if (wx_bgctrl(w_handle))
					if (pwin->w_flags & VF_INTREE)
						do_walk(w_handle, which, -1, -1, NULL, NULL);
		}
		break;

	case WF_RESVD:
		w_walkflag = w_handle;
		if (w_handle == DESK)
		{
			wx_drawdesk((GRECT *)pinwds);
		}
		break;

	case WF_NEWDESK:
		if (*((LPTREE *)pinwds) == 0)
		{
			gl_newdesk = ad_stdesk;
			gl_newroot = ROOT;
		} else
		{
			gl_newdesk = *((LPTREE *)pinwds);
			gl_newroot = pinwds[2];
		}
		break;

	case WF_BEVENT:
		pwin->w_flags = (pwin->w_flags & 0xff) | (pinwds[0] << 8);
		break;

	case WF_COLOR:
		wx_setcolor(pwin->w_tcolor, pinwds[0], pinwds[1]);
		wx_setcolor(pwin->w_bcolor, pinwds[0], pinwds[2]);
		if (pwin->w_flags & VF_INTREE)
			do_walk(w_handle, which, -1, -1, NULL, NULL);
		break;

	case WF_DCOLOR:
		wx_setcolor(mem->tcolor, pinwds[0], pinwds[1]);
		wx_setcolor(mem->bcolor, pinwds[0], pinwds[2]);
		break;

	case WF_DDELAY:
		mem->init_scroll = pinwds[0];
		mem->cont_scroll = pinwds[1];
		mem->cont_close = pinwds[2];
		mem->cont_full = pinwds[3];
		break;
	}

	wx_update(END_UPDATE);
	return ret;
}


/* 306de/winx22: 00e2259e */
int16_t wx_find(P(int) mx, P(int) my)
PP(int mx;)									/* mouse's x position */
PP(int my;)									/* mouse's y position */
{
	WX_MEM *mem = wx_gmem();

	(VOID)mem;
	return 0;
}


/* 306de/winx22: 00e225d4 */
int16_t wx_update(P(int) beg_update)
PP(register int beg_update;)					/* flag for the call's function */
{
	return 0;
}


/* 306de/winx22: 00e22652 */
VOID wx_calc(P(int16_t) wtype, P(int16_t) kind, P(int16_t) ix, P(int16_t) iy, P(int16_t) iw, P(int16_t) ih, P(int16_t *) ox, P(int16_t *) oy, P(int16_t *) ow, P(int16_t *) oh)
PP(int16_t wtype;)								/* the type of calculation to perform */
PP(register int16_t kind;)						/* components present in the window */
PP(int16_t ix;)									/* input x-coordinate of window */
PP(int16_t iy;)									/* input y-coordinate of window */
PP(int16_t iw;)									/* input width of window */
PP(int16_t ih;)									/* input height of window */
PP(int16_t *ox;)								/* output x-coordinate of work/border area */
PP(int16_t *oy;)								/* output y-coordinate of work/border area */
PP(int16_t *ow;)								/* output width of work/border area */
PP(int16_t *oh;)								/* output height of work/border area */
{
}


/* 306de/winx22: 00e22784 */
int16_t wx_new(NOTHING)
{
	return 0;
}


/* 306de/winx22: 00e21b5a */
VOID wx_setactive(NOTHING)
{
}


/* 306de/winx22: 00e21824 */
VOID wx_drawdesk(P(GRECT *) dirty)
PP(register GRECT *dirty;)							/* rectangle of dirty area */
{
}


/* 306de/winx22: 00E22152 */
BOOLEAN wx_init(NOTHING)
{
	return FALSE;
}


/* 306de/winx22: 00e2208a */
PD *wx_srchwp(P(int16_t) wh, P(int16_t) newbutton)
PP(int16_t wh;)
PP(int16_t newbutton;)
{
	return 0;
}


/* 306de/winx22: 00e21404 */
VOID wx_nameit(P(PD *) p, P(const char *) pname)
PP(PD *p;)
PP(const char *pname;)
{
}


/* 306de/winx22: 00e22aea */
VOID wx_rmerge(P(int16_t *) nm, P(int16_t *) om)
PP(int16_t *nm;)
PP(int16_t *om;)
{
}


/* 306de/winx22: 00e221b0 */
BOOLEAN wx_start(NOTHING)
{
	return 0;
}


/* 306de/winx22: 00e22c04 */
VOID wx_getmouse(NOTHING)
{
}


/* 306de/winx22: 00e21424 */
BOOLEAN wx_callfunc(P(int16_t) func, P(VOIDPTR) arg, P(int32_t *) val)
PP(int16_t func;)
PP(VOIDPTR arg;)
PP(int32_t *val;)
{
	int32_t ret;
	
	if (winxvars.winxfunc == 0 || (ret = (*winxvars.winxfunc)(func, arg)) < 0)
		return FALSE;
	*val = ret;
	return TRUE;
}


static VOID wx_21dba(NOTHING)
{
}


/* 306de/winx22: 00e34ce8 */
VOIDPTR wx_alloc(P(long ) size)
PP(long size;)
{
	register VOIDPTR blk;
	int32_t winxfunc = 0;
	int32_t flags = 0;
	
	static struct wxinfo const info = {
		&winxvars.wx_mem,
		WX_MAXWIN,
		sizeof(WX_WINDOW),
		&rlr,
		144,
		WX_QUEUE_SIZE,
		(VOIDPTR *) &gl_newdesk,
		&gl_newroot,
		(VOIDPTR *) &ad_stdesk,
		wx_21dba
	};

	blk = dos_alloc(size + sizeof(WX_MEM));
	winxvars.magic = 0x57494e58L; /* 'WINX' */
	winxvars.version = WX_VERSION;
	winxvars.date = WX_DATE;
	winxvars.global_flags = WX_GLOBAL;
	winxvars.wx_mem = (WX_MEM *)((char *)blk + size);
	winxvars.info = &info;
	getcookie(0x57494e58L, &winxfunc);
	winxvars.winxfunc = (int32_t (CDECL *) PROTO((short, VOIDPTR)))winxfunc;
	if (wx_callfunc(1, &winxvars, &flags))
		winxvars.global_flags = (uint16_t)flags;
	
	return blk;
}


/* 
 * ap_sendmsg() - send message to current process
 */
/* 206us: 00e1e2ea */
/* 306de: 00e21daa */
VOID ap_sendmsg(P(int16_t *) ap_msg, P(int16_t) type, P(int16_t) towhom, P(int16_t) w3, P(int16_t) w4, P(int16_t) w5, P(int16_t) w6, P(int16_t) w7)
PP(register int16_t *ap_msg;)
PP(int16_t type;)
PP(int16_t towhom;)
PP(int16_t w3;)
PP(int16_t w4;)
PP(int16_t w5;)
PP(int16_t w6;)
PP(int16_t w7;)
{
	register int16_t pid;
	register PD *pd;
	
	ap_msg[0] = type;
	ap_msg[1] = pid = (pd = rlr)->p_pid;
	ap_msg[2] = 0;
	ap_msg[3] = w3;
	ap_msg[4] = w4;
	ap_msg[5] = w5;
	ap_msg[6] = w6;
	ap_msg[7] = w7;
	if (towhom != pid || pd->p_qindex <= (WX_QUEUE_SIZE - 16))
		ap_rdwr(AQWRT, towhom, 16, &ap_msg[0]);
	(VOID)wx_get_globals;
}

#endif /* TP_WINX */
