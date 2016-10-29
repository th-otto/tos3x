/*      GEMWMLIB.C      4/23/84 - 07/11/85      Lee Lorenzen            */
/*      merge High C vers. w. 2.2               8/24/87         mdf     */
/*      fix wm_delete bug                       10/8/87         mdf     */

/*
 *       Copyright 1999, Caldera Thin Clients, Inc.
 *                 2002-2016 The EmuTOS development team
 *
 *       This software is licenced under the GNU Public License.
 *       Please see LICENSE.TXT for further information.
 *
 *                  Historical Copyright
 *       -------------------------------------------------------------
 *       GEM Application Environment Services              Version 2.3
 *       Serial No.  XXXX-0000-654321              All Rights Reserved
 *       Copyright (C) 1987                      Digital Research Inc.
 *       -------------------------------------------------------------
 */

#include "aes.h"
#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"

#define DESKWH	0x0
#define DROP_SIZE 0						/* Windows don't have drop shadows */

#if MULTIAPP
#define NUM_MWIN NUM_WIN+1
#define INACROOT NUM_WIN
#endif
#if SINGLAPP
#define NUM_MWIN NUM_WIN
#endif


int16_t gl_wtop;
OBJECT *gl_awind;
LPTREE newdesk;
int16_t newroot;
intptr_t ad_windspb;

static OBJECT W_TREE[NUM_MWIN];
/*static OBJECT W_ACTIVE[MAXOBJ];*/



static GRECT *w_getxptr(P(int16_t) which, P(int16_t) w_handle)
PP(int16_t which;)
PP(register int16_t w_handle;)
{
	switch (which)
	{
	case WS_CURR:
	case WS_TRUE:
		return (GRECT *) (&W_TREE[w_handle].ob_x);
	case WS_PREV:
		return &D.w_win[w_handle].w_prev;
	case WS_WORK:
		return &D.w_win[w_handle].w_work;
	case WS_FULL:
		return &D.w_win[w_handle].w_full;
	}
	return 0;
}


/* Get the size (x,y,w,h) of the window				*/

VOID w_getsize(P(int16_t) which, P(int16_t) w_handle, P(GRECT *)pt)
PP(int16_t which;)
PP(int16_t w_handle;)
PP(GRECT *pt;)
{
	rc_copy(w_getxptr(which, w_handle), pt);

#if DROP_SIZE
	if ((which == WS_TRUE) && pt->g_w && pt->g_h)
	{
		pt->g_w += DROP_SIZE;
		pt->g_h += DROP_SIZE;
	}
#endif
}


/*
 * Wm_init() -	initializes window colors, then start up the window
 *		manager.
 *	     -	this is called by geminit; only at boot time; so that
 *		window color defaults don't get munched when apps are
 *		launched; etc.
 */
VOID wm_init(NOTHING)
{
}


/*
 * AES #100 - wind_create - Initializes a new window 
 *
 * Wm_create() - allocates the application's full-size window and 
 *		 returns the window's handle.
 *	       - returns FAILURE (-1) if no handle is available or
 *		 if an error occurred.
 */
int16_t wm_create(P(uint16_t) kind, P(GRECT *) rect)
PP(uint16_t kind;)								/* kind of window to be created */
PP(register GRECT *rect;)						/* x, y, width and height of full size window */
{
	return 0;
}



/*
 * AES #101 - wind_open - Open window
 *
 * Wm_open() - opens a window in its given size and location.
 *	     - returns FALSE (0) if given handle is invalid,
 *	       or if window has already been opened.
 *	     - returns TRUE (1) if everything is fine.
 */
int16_t wm_open(P(int16_t) handle, P(GRECT *) rect)
PP(int16_t handle;)							/* handle of window to be opened */
PP(register GRECT *rect;)						/* x, y, width and height of opened window */
{
	return 0;
}


/*
 * AES #102 - wind_close - Close window
 *
 * Wm_close() - closes an opened window
 *	      - returns FALSE (0) if given handle is invalid,
 *	        or if window has already been closed.
 *	      - returns TRUE (1) if everything is fine.
 *
 */
int16_t wm_close(P(int16_t) handle)
PP(int16_t handle;)							/* handle of window to be closed */
{
	return 0;
}


/*
 * AES #103 - wind_delete - Delete window
 *
 * Wm_delete() - closes the window if it is not already closed,
 *		 and frees the window structure.
 *	       - returns FALSE (0) if given handle is invalid.
 *	       - returns TRUE (1) if everything is fine.
 *
 */
int16_t wm_delete(P(int16_t) handle)
PP(int16_t handle;)							/* handle of window to be deleted */
{
	return 0;
}


/*
 * AES #105 - wind_get - Obtains various properties of a window.
 *
 * Wm_get() - returns information of window in the given array
 *	    - returns FALSE (0) if given handle is invalid
 *	    - returns TRUE (1) if everything is fine
 *
 */
#if AES3D
int16_t wm_get(P(int16_t) handle, P(int16_t) field, P(int16_t *)ow, P(const int16_t *) iw)
PP(register int16_t handle;)						/* window handle */
PP(int16_t field;)								/* flag to identify what info to be returned */
PP(register int16_t *ow;)							/* return values */
PP(const int16_t *iw;)
#else
int16_t wm_get(P(int16_t) handle, P(int16_t) field, P(int16_t *)ow)
PP(register int16_t handle;)						/* window handle */
PP(int16_t field;)								/* flag to identify what info to be returned */
PP(register int16_t *ow;)							/* return values */
#endif
{
	return 0;
}


/*
 * AES #106 - wind_set - Alter various window attributes.
 *
 * Wm_set() - changes information of a window
 *	    - returns FALSE (0) if given handle is invalid
 *	    - returns TRUE (1) if everything is fine
 *
 */
int16_t wm_set(P(int16_t) handle, P(int16_t) field, P(int16_t *) iw)
PP(register int16_t handle;)						/* window handle */
PP(register int16_t field;)							/* flag to identify what info to be changed */
PP(register int16_t *iw;)							/* values to change to */
{
	return 0;
}


/*
 * AES #106 - wind_find - Find the ID of a window at the given coordinates.
 *
 * Wm_find() - finds which window is under the mouse's x, y position
 *
 */
int16_t wm_find(P(int) mx, P(int) my)
PP(int mx;)									/* mouse's x position */
PP(int my;)									/* mouse's y position */
{
	return 0;
}


/*
 * AES #107 - wind_update - Blocks or releases screen operations.
 *
 * Wm_update() - locks or unlocks the current state of the window 
 *		 tree while an application is responding to a 
 *		 window update message in his message pipe or is 
 *		 making some other direct screen update based on
 *		 his current rectangle list.
 *
 */
int16_t wm_update(P(int) code)
PP(int code;)								/* flag for the call's function */
{
	return 0;
}


/*
 * AES #108 - wind_calc - Calculates the limits or the total space requirement of a window 
 *
 *	Given a width and height of a Work Area and the Kind of window
 *	desired calculate the required window size including the 
 *	Border Area.  or...  Given the width and height of a window
 *	including the Border Area and the Kind of window desired, calculate
 *	the result size of the window Work Area.
 */
int16_t wm_calc(P(int16_t) type, P(int16_t) kind, P(int16_t) ix, P(int16_t) iy, P(int16_t) iw, P(int16_t) ih, P(int16_t *) ox, P(int16_t *) oy, P(int16_t *) ow, P(int16_t *) oh)
PP(int16_t type;)								/* the type of calculation to perform */
PP(int16_t kind;)								/* components present in the window */
PP(int16_t ix;)									/* input x-coordinate of window */
PP(int16_t iy;)									/* input y-coordinate of window */
PP(int16_t iw;)									/* input width of window */
PP(int16_t ih;)									/* input height of window */
PP(int16_t *ox;)								/* output x-coordinate of work/border area */
PP(int16_t *oy;)								/* output y-coordinate of work/border area */
PP(int16_t *ow;)								/* output width of work/border area */
PP(int16_t *oh;)								/* output height of work/border area */
{
	return 0;
}


/*
 * AES #109 - wind_new - Close all windows.
 *
 * Wm_new() - Delete all the window structures and clean 
 *	      up the window update semaphore.  This 
 *	      routine is very critical, so don't call 
 *	      it when you are not sure.  You must call 
 *	      this guy right after you return from the 
 *	      child process.	Only at this moment the 
 *	      system can be able to recover itself.
 *
 * Comments:
 *	(Jul 23 91 ml.) This may need to be modified for
 * future multitasking system.
 *	
 */
int16_t wm_new(NOTHING)
{
	return 0;
}


/*
 * W_drawchange() - draw the borders of opened windows which 
 *		    intersects with the dirty area, and send
 *		    redraw messages to those windows
 */
VOID w_drawchange(P(GRECT *) dirty, P(uint16_t) skip, P(uint16_t) stop)
PP(GRECT *dirty;)							/* rectangle of dirty area */
PP(uint16_t skip;)
PP(uint16_t stop;)								/* window to be skipped */
{
}


static int16_t w_top(NOTHING)
{
    return (gl_wtop != NIL) ? gl_wtop : DESKWH;
}


VOID w_setactive(NOTHING)
{
    GRECT   d;
    int16_t    wh;
    PD   *ppd;

    wh = w_top();
    w_getsize(WS_WORK, wh, &d);
    ppd = D.w_win[wh].w_owner;

    /* BUGFIX 2.1: don't chg own if null */
    if (ppd != NULL)
        ct_chgown(ppd, &d);
}


VOID w_bldactive(P(int16_t) w_handle)
PP(register int16_t w_handle;)
{
}


/* 
 * Ap_sendmsg() - send message to current process
 */
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
	ap_msg[0] = type;
	ap_msg[1] = rlr->p_pid;
	ap_msg[2] = 0;
	ap_msg[3] = w3;
	ap_msg[4] = w4;
	ap_msg[5] = w5;
	ap_msg[6] = w6;
	ap_msg[7] = w7;
	ap_rdwr(AQWRT, towhom, 16, ADDR(&ap_msg[0]));
}
