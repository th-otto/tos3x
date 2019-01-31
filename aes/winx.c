#include "aes.h"
#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"

#if TP_WINX /* whole file */


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


/* 306de/winx22: 00e222b4 */
int16_t wx_get(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t *)poutwds)
PP(register int16_t w_handle;)						/* window handle */
PP(int16_t w_field;)								/* flag to identify what info to be returned */
PP(register int16_t *poutwds;)							/* return values */
{
	return 0;
}


/* 306de/winx22: 00e22434 */
int16_t wx_set(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t *) pinwds)
PP(register int16_t w_handle;)						/* window handle */
PP(register int16_t w_field;)						/* flag to identify what info to be changed */
PP(register int16_t *pinwds;)							/* values to change to */
{
	return 0;
}


/* 306de/winx22: 00e2259e */
int16_t wx_find(P(int) mx, P(int) my)
PP(int mx;)									/* mouse's x position */
PP(int my;)									/* mouse's y position */
{
	return 0;
}


/* 306de/winx22: 00e225d4 */
int16_t wx_update(P(int) beg_update)
PP(register int beg_update;)								/* flag for the call's function */
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
VOID wm_init(NOTHING)
{
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


static BOOLEAN wx_21424(P(int16_t) func, P(VOIDPTR) arg, P(int32_t *) val)
PP(int16_t func;)
PP(VOIDPTR arg;)
PP(int32_t *val;)
{
	return 0;
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

	blk = dos_alloc(size + WX_MAXWIN * sizeof(WX_WINDOW) + 11556);
	winxvars.magic = 0x57494e58L; /* 'WINX' */
	winxvars.version = WX_VERSION;
	winxvars.date = WX_DATE;
	winxvars.global_flags = WX_GLOBAL;
	winxvars.wx_mem = (char *)blk + size;
	winxvars.info = &info;
	getcookie(0x57494e58L, &winxfunc);
	winxvars.winxfunc = (int32_t (CDECL *) PROTO((short, ...)))winxfunc;
	if (wx_21424(1, &winxvarswinx, &flags))
		winxvars.global_flags = (uint16_t)flags;
	
	return blk;
}


#endif /* TP_WINX */
