/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Author: mui $	$Date: 89/04/26 18:24:39 $
 * =======================================================================
 *
 * Revision 2.2  89/04/26  18:24:39  mui
 * TT
 * 
 * Revision 2.1  89/02/22  05:27:45  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 * Revision 1.2  89/02/16  10:47:23  mui
 * Fix dclicks: Move gl_bpend from evremove to post_button
 * 
 * Revision 1.1  88/06/02  12:34:18  lozben
 * Initial revision
 * 
 *************************************************************************
 */
/*	GEMINPUT.C	1/28/84 - 02/05/85	Lee Jay Lorenzen	*/
/*	Reg Opt		03/09/85		Derek Mui		*/
/*	1.1		03/21/85 - 06/19/85	Lowell Webster		*/
/*	cur draw	05/14/85		jim eisenstein		*/
/*	Fix mchange for lost of mouse position	Derek Mui		*/
/*	At evremove check the parameter, if it is a double click then	*/
/*	decrement the gl_bpend	2/27/86		Derek Mui		*/
/*	Kludge	at mchange	3/11/86		Derek Mui		*/
/*	For multiple mouse button	10/3/86	Derek Mui		*/
/*	Change at downorup 10/3/86 - 10/23/86	Derek Mui		*/
/*	Change e_return at post_button, abutton, evremove  10/29/86	*/
/*	Change again at post_button, abutton evremove 10/31/86		*/
/*	Unchange downorup	10/31/86	Derek Mui		*/
/*	Clean up at mchange	2/22/88		D.Mui			*/
/*	Change at mowner for new window library	8/1/92	D.Mui		*/

/*
 *	-------------------------------------------------------------
 *	GEM Application Environment Services		  Version 1.1
 *	Serial No.  XXXX-0000-654321		  All Rights Reserved
 *	Copyright (C) 1985			Digital Research Inc.
 *	-------------------------------------------------------------
 */

#include "aes.h"
#include "gemlib.h"
#include "gsxdefs.h"


#define MB_DOWN 0x01


int16_t button;
int16_t xrat;
int16_t yrat;
int16_t kstate;
int16_t mclick;
int16_t mtrans;

int16_t pr_button;
int16_t pr_xrat;
int16_t pr_yrat;
int16_t pr_mclick;


PD *gl_mowner;		/* current mouse owner  */
PD *gl_kowner;		/* current keybd owner  */
PD *gl_cowner;		/* current control rect. owner */
/* screen manager process that controls the mouse when its outside the control rectangle. */
PD *ctl_pd;
/* current control rect. */
GRECT ctrl;


/* # of times into the desired button state      */
int16_t gl_bclick;
/* number of pending events desiring more than a single clicks     */
int16_t gl_bpend;
#if AESVERSION >= 0x330
int16_t gl_button;
#endif
/* the current desired button state   */
int16_t gl_bdesired;
/* the current true button state   */
int16_t gl_btrue;
/* the current amount of time before the button event is considered finished */					
int16_t gl_bdelay;



#if AESVERSION >= 0x200
/*
 * Check if the current click will transfer mouse ownership
 */
/* 306de: 00e1e6fe */
PD *mowner(P(int16_t) new)
PP(int16_t new;)
{
	int16_t wh;
	register int16_t mx, my;
	PD *m;
#if NEWWIN
	WINDOW *wp;
#endif
#if AES3D | NEWWIN
	GRECT t;
#endif

	m = gl_mowner;

#if TP_WINX
	if (new == MB_DOWN ||
		((winxvars.global_flags & 0x02) && new == 2))
#else
	if (new == MB_DOWN)
#endif
	{
		mx = xrat;
		my = yrat;
		/* if inside ctrl rect then owned by active process */
		if (inside(mx, my, &ctrl))
		{
			m = gl_cowner;
		} else							/* if in menu bar then  */
		{								/* owned by ctrl mgr    */
#if NEWWIN
			m = ctl_pd;
#endif

#if AES3D | NEWWIN
			if (!inside(mx, my, &gl_rmenu))
			{							/* Hit any window?  */
				wh = wm_find(mx, my);
				if (wh && wh != NIL)
				{
#if AES3D
					wm_get(wh, WF_WORKXYWH, &t.g_x, NULL);
#else
#if TP_WINX
					wx_get(wh, WF_WORKXYWH, &t.g_x);
#else
					wm_get(wh, WF_WORKXYWH, &t.g_x);
#endif
#endif

#if NEWWIN
					if (inside(mx, my, &t))
					{
						wp = srchwp(wh);
						if (wp->type & 0x01)
							m = wp->w_owner;
					}
#else
					m = ctl_pd;
#endif
				} else					/* hit window 0 background */
				{
					/* added Jul 23 91 for new window manager - ml. */
					m = srchwp(0)->w_owner;
				}
			}
#else
#if TP_WINX
			if (inside(mx, my, &gl_rmenu))
			{							/* Hit any window? */
				wh = NIL;
				m = ctl_pd;
			} else
			{
				wh = wx_find(mx, my) ? NIL : 0;
				m = wx_srchwp(wh, new);
			}
#else
			if (inside(mx, my, &gl_rmenu))
			{							/* Hit any window? */
				wh = NIL;
			} else
			{
				wh = wm_find(mx, my) ? NIL : 0;
			}
			m = wh == NIL ? ctl_pd : srchwp(0)->w_owner;
#endif
#endif
		}
	}

	return m;
}
#endif


#if UNLINKED
/*
 *	Routine to check if the mouse is in part of 
 *	the screen owned by the control manager.  
 *	It returns -1 if it is, 
 *	It returns 0 if it is over the desktop,
 *	or +1 if it is over the active window.
 */

int16_t chk_ctrl(P(int16_t) mx, P(int16_t) my)
PP(register int16_t mx;)
PP(register int16_t my;)
{
	/* if inside ctrl rect then owned by active process */
	if (inside(mx, my, &ctrl))
		return 1;
	/* if in menu bar then owned by ctrl mgr  */
	if (inside(mx, my, &gl_rmenu))
		return -1;
	/* if on any window beside the desktop then ctrl mgr owns */
#if TP_WINX
	return wx_find(mx, my) ? -1 : 0;
#else
	return wm_find(mx, my) ? -1 : 0;
#endif
}


/*	Whenever there is a click, come to here
 *	Button click code call that is from the button interrupt
 *	code with interrupts off.
 */
VOID b_click(P(int16_t) state)
PP(register int16_t state;)
{
	/* ignore it unless it represents a change */
	if (state != gl_btrue)
	{
		/* see if we've already set up a wait  */
		if (gl_bdelay)
		{
			/* if the change is into the desired state then increment cnt */
			if (state == gl_bdesired)
			{
				gl_bclick++;
				gl_bdelay += 3;
			}
		} else
		{
			/*
			 * if someone cares about multiple clicks and this is
			 * not a null mouse then set up delay else just fork it
			 */
			if (
#if !TP_WINX & !TP_48
				gl_bpend &&
#endif
				state)
			{
				/* start click cnt at 1 establish desired state and set wait flag */
				gl_bclick = 1;
				gl_bdesired = state;
				/* button delay set in ev_dclick (5) */
				gl_bdelay = gl_dclick;
			} else
			{
				forkq(bchange, state, 1);
			}
		}
		/* update true state of the mouse */
		gl_btrue = state;
	}
}


/*
 *	Button delay code that is called from the tick interrupt code with 
 *	interrupts off.
 */

VOID b_delay(P(int16_t) amnt)
PP(int16_t amnt;)
{
	/* see if we have a delay for mouse click in progress */
	if (gl_bdelay)
	{
		/* see if decrementing delay cnt causes delay to be over */
		gl_bdelay -= amnt;
		if (!(gl_bdelay))
		{
			forkq(bchange, gl_bdesired, gl_bclick);
			if (gl_bdesired != gl_btrue)
			{
				forkq(bchange, gl_btrue, 1);
			}
		}
	}
}


/*
 *	Set the current control rectangle which is the part of the
 *	screen owned by the active process.  Normally, the work area
 *	of the top window.
 */

VOID set_ctrl(P(GRECT *) pt)
PP(GRECT *pt;)
{
	rc_copy(pt, &ctrl);
}


/*
 *	Get the current control rectangle which is the part of the
 *	screen owned by the active process.  Normally, the work area
 *	of the top window, but sometimes the whole screen during 
 *	form fill-in.
 */

VOID get_ctrl(P(GRECT *) pt)
PP(GRECT *pt;)
{
	rc_copy(&ctrl, pt);
}


/*
 *	Used by form_do to remember the current keyboard and mouse
 *	owners.
 */

VOID get_mkown(P(PD **) pmown, P(PD **) pkown)
PP(PD **pkown;)
PP(PD **pmown;)
{
	*pmown = gl_mowner;
	*pkown = gl_kowner;
}


/*
 *	Used by control manager and form_do to give the mouse or keyboard
 *	to another process.  The mouse should only be given with the 
 *	buttons in an up state.
 */
VOID set_mkown(P(PD *) mp, P(PD *) kp)
PP(PD *mp;)
PP(PD *kp;)
{
	/* change the owner */
	gl_cowner = gl_mowner = mp;
	/* pretend mouse moved to get the right form showing */
	/* and get mouse event posted correctly */
	post_mouse(gl_mowner, xrat, yrat);
	/* post a button event in case the new owner was waiting */
	post_button(gl_mowner, button, 1);

	gl_kowner = kp;
}


/*
 *	EnQueue a character on a circular keyboard buffer.
 */
VOID nq(P(uint16_t) ch, P(CQUEUE *) qptr)
PP(uint16_t ch;)
PP(register CQUEUE *qptr;)
{
	if (qptr->c_cnt < KBD_SIZE)
	{
		qptr->c_buff[qptr->c_rear++] = ch;
		if (qptr->c_rear == KBD_SIZE)
			qptr->c_rear = 0;
		qptr->c_cnt++;
	}
}
#endif /* UNLINKED */


/*
 *	DeQueue a a character from a circular keyboard buffer.
 */
/* 306de: 00e1e7a4 */
/* 104de: 00fde634 */
/* 106de: 00e1fe7c */
uint16_t dq(P(CQUEUE *) qptr)
PP(register CQUEUE *qptr;)
{
	register int16_t q2;

	qptr->c_cnt--;
	q2 = qptr->c_front++;
	if (qptr->c_front == KBD_SIZE)
		qptr->c_front = 0;
	return qptr->c_buff[q2];
}


/*
 *	Flush the characters from a circular keyboard buffer.
 */
/* 306de: 00e1e7da */
/* 104de: 00fde662 */
/* 106de: 00e1feb2 */
VOID fq(NOTHING)
{
	while (cda->c_q.c_cnt)
		dq(&cda->c_q);
}


/*
 * Remove the event from the CDA
 * ret may have number of clicks
 * or the desired character
 * evinsert is in the gemasync.c
 */
/* 306de: 00e1e7fe */
/* 104de: 00fde684 */
/* 106de: 00e1fed6 */
VOID evremove(P(EVB *) e, P(uint16_t) ret)
PP(register EVB *e;)
PP(uint16_t ret;)
{
	/* unlinked this EVB, and aret() */
	e->e_return |= (uint16_t) ret;			/* will remove and free the EVB */
	e->e_pred->e_link = e->e_link;
	if (e->e_link)
		e->e_link->e_pred = e->e_pred;
	zombie(e);
}


/* 306de: 00e1e840 */
/* 104de: 00fde6ba */
/* 106de: 00e1ff18 */
VOID kchange(P(int16_t) ch, P(int16_t) kstat)
PP(int16_t ch;)
PP(int16_t kstat;)
{
	kstate = kstat;
	if (ch)
		post_keybd(gl_kowner, ch);
}


/* 306de: 00e1e864 */
/* 104de: 00fde6dc */
/* 106de: 00e1ff3c */
VOID post_keybd(P(PD *) p, P(uint16_t) ch)
PP(PD *p;)
PP(uint16_t ch;)
{
	register CDA *c;
	register EVB *e;
	register CQUEUE *qptr;

	c = p->p_cda;
	/* if anyone waiting ?  */
	if ((e = c->c_iiowait))				/*   wake him up    */
	{
		evremove(e, ch);
	} else
	{
		/* no one is waiting, just toss it in the buffer */

		/* nq(ch, &c->c_q); */

		qptr = &c->c_q;

		if (qptr->c_cnt < KBD_SIZE)
		{
			qptr->c_buff[qptr->c_rear++] = ch;
			if (qptr->c_rear == KBD_SIZE)
				qptr->c_rear = 0;
			qptr->c_cnt++;
		}
	}
}


/*
 * forker will come here
 */
/* 306de: 00e1e8c2 */
/* 104de: 00fde730 */
/* 106de: 00e1ff9a */
VOID bchange(P(int16_t) new, P(int16_t) clicks)
PP(int16_t new;)
PP(int16_t clicks;)
{
#if AESVERSION >= 0x200
	/* if control mgr. does not own the mouse */
#if AESVERSION < 0x330
	if (gl_mowner != ctl_pd)
#endif
		gl_mowner = mowner(new);
#else
	/* inline version of mowner() above */
	if (gl_mowner != ctl_pd)
	{
		int16_t wh;
		register int16_t mx, my;
	
		if (new == MB_DOWN && button == 0)
		{
			mx = xrat;
			my = yrat;
			/* if inside ctrl rect then owned by active process */
			if (inside(mx, my, &ctrl))
			{
				gl_mowner = gl_cowner;
			} else							/* if in menu bar then  */
			{								/* owned by ctrl mgr    */
				if (inside(mx, my, &gl_rmenu))
				{							/* Hit any window? */
					wh = NIL;
				} else
				{
					wh = wm_find(mx, my) ? NIL : 0;
				}
				gl_mowner = wh == NIL ? ctl_pd : srchwp(0)->w_owner;
			}
		}
	}
#endif
	/* see if this button event causes an ownership change */

	/* if the button went down check to see if ownership should go to the control mgr. */

	mtrans++;
	pr_button = button;
	pr_mclick = mclick;
	pr_xrat = xrat;
	pr_yrat = yrat;
	button = new;
	mclick = clicks;
	post_button(gl_mowner, button, clicks);
}


/* 206de: 00e1aed2 */
/* 104de: 00fde826 */
/* 106de: 00e200a0 */
int16_t downorup(P(int16_t) new, P(intptr_t) buparm)
PP(int16_t new;)
PP(register intptr_t buparm;)
{
	register int16_t flag, mask, val;

	flag = (buparm >> 24) & 0x00ffL;	/* clicks   */
	mask = (buparm >> 8) & 0x00ffL;		/* which button */
	val = (buparm) & 0x00ffL;			/* up or down   */
	return ((mask & (val ^ new)) == 0) != flag;
}




/* 104de: 00fde876 */
/* 106de: 00e200fc */
VOID post_button(P(PD *) p, P(int16_t) new, P(int16_t) clks)
PP(PD *p;)
PP(int16_t new;)
PP(register int16_t clks;)
{
	register CDA *c;
	register EVB *e1, *e;
	register uint16_t clicks;

	c = p->p_cda;
	for (e = c->c_bsleep; e; e = e1)
	{
		e1 = e->e_link;
		if (downorup(new, e->e_parm))
		{
			/* decrement counting semaphore if one of the multi-click guys was satisfied */
			clicks = LHIWD(e->e_parm) & 0x00ffL;
			if (clicks > 1)
				gl_bpend--;

			e->e_return = HW(new);		/* changed */
			evremove(e, (clks > clicks) ? clicks : clks);
		}
	}
}


/*
 * forker of mouse change
 */
/* 306de: 00e1ea10 */
/* 104de: 00fde8ec */
/* 106de: 00e2017c */
VOID mchange(P(int16_t) rx1, P(int16_t) ry1)
PP(register int16_t rx1;)
PP(register int16_t ry1;)
{
	int16_t rx;
	int16_t ry;

	/* zero out button wait if mouse moves more then a little */
#if BINEXACT /* sigh */
	gsx_ncode(MOUSE_ST, 0L);
#else
	gsx_ncode(MOUSE_ST, 0, 0);
#endif
	rx = ptsout[0];
	ry = ptsout[1];

	if (gl_bdelay && ((xrat - rx > 2) || (xrat - rx < -2) || (yrat - ry > 2) || (yrat - ry < -2)))
		b_delay(gl_bdelay);
	/* xrat, yrat hold true */
	xrat = rx;
	yrat = ry;
	/* post the event   */
	if (gl_play)						/* KLUDGE 3/11/86   */
	{
		intin[0] = 1;					/* do a mouse sample mode */
		intin[1] = 2;
#if BINEXACT
		gsx_ncode(SET_INPUT_MODE, 0x00000002L); /* sigh */
#else
		gsx_ncode(SET_INPUT_MODE, 0, 2);
#endif
		drawrat(rx1, ry1);
		ptsin[0] = rx1;
		ptsin[1] = ry1;
#if BINEXACT
		gsx_ncode(LOCATOR_INPUT, 0x00010000L); /* sigh */
#else
		gsx_ncode(LOCATOR_INPUT, 1, 0);
#endif
		xrat = rx1;
		yrat = ry1;
	}

	/*
	 * give mouse to screen handler when not button down and
	 * there is an active menu and it will satisfy his event
	 */
	/* CHANGED LKW      */
	if (!button && gl_mntree && gl_ctwait.m_out != inside(xrat, yrat, (GRECT *)&gl_ctwait.m_x))
		gl_mowner = ctl_pd;

	post_mouse(gl_mowner, xrat, yrat);
}


/* 306de: 00e1eb5e */
/* 104de: 00fdea1a */
/* 106de: 00e202ca */
VOID post_mouse(P(PD *) p, P(int16_t) grx, P(int16_t) gry)
PP(register PD *p;)
PP(int16_t grx;)
PP(int16_t gry;)
{
	register EVB *e;
	register EVB *e1;

	/* check event list to signal waiting pr's */
	for (e = p->p_cda->c_msleep; e; e = e1)
	{
		e1 = e->e_link;
		if (inorout(e, grx, gry))
			evremove(e, 0);
	}
}


/* 306de: 00e1eba0 */
/* 104de: 00fdea52 */
/* 106de: 00e2030c */
int16_t inorout(P(EVB *) e, P(int16_t) rx, P(int16_t) ry)
PP(register EVB *e;)
PP(register int16_t rx;)
PP(register int16_t ry;)
{
	MOBLK mo;

	/* in or out of     */
	/*   specified rectangle */
	mo.m_out = ((e->e_flag & EVMOUT) != 0x0);
	mo.m_x = LHIWD(e->e_parm);
	mo.m_y = LLOWD(e->e_parm);
	mo.m_w = LHIWD(e->e_return);
	mo.m_h = LLOWD(e->e_return);
	return mo.m_out != inside(rx, ry, (GRECT *)&mo.m_x);
}


#if UNLINKED
/*	wait for keybd input	*/
VOID akbin(EVB *e, intptr_t addr)
register EVB *e;
intptr_t addr;
{
	/* find vcb to input, point c at it  */
	if (cda->c_q.c_cnt)
	{
		/* another satisfied customer */
		e->e_return = (uint16_t) dq(&cda->c_q);
		zombie(e);
	} else								/* time to zzzzz... */
	{
		evinsert(e, &cda->c_iiowait);
	}
}


/*
 * wait for timer
 */
VOID adelay(P(EVB *) e, P(int32_t) c)
PP(register EVB *e;)
PP(register int32_t c;)								/* # of ticks to wait  */
{
	register EVB *p, *q;

	if (c == 0)
		c = 1;

	cli();
	if (CMP_TICK)
	{
		/*
		 * if already counting down then reset CMP_TICK to the
		 * lower number but let NUM_TICK grow from its accumulated value
		 */
		if (c <= CMP_TICK)
			CMP_TICK = c;
	} else
	{
		/* if we aren't currently counting down for someone else then start ticking  */
		CMP_TICK = c;
		/* start NUM_TICK out at zero */
		NUM_TICK = 0x0L;
	}

	e->e_flag |= EVDELAY;
	q = (EVB *)((char *) &dlr - elinkoff);
	for (p = dlr; p; p = (q = p)->e_link)
	{
		if (c <= p->e_parm)
			break;
		c -= p->e_parm;
	}
	e->e_pred = q;
	q->e_link = e;
	e->e_parm = c;
	e->e_link = p;
	if (p)
	{
		c = p->e_parm - c;
		p->e_pred = e;
		p->e_parm = c;
	}
	sti();
}


/*
 * wait for button
 */
int16_t abutton(P(EVB *) e, P(int32_t) p)
PP(register EVB *e;)
PP(register int32_t p;)
{
	register int16_t bclicks;

	if (downorup(button, p))
	{
		e->e_return = HW(button);		/* changed */
		zombie(e);						/* 'nuff said       */
	} else
	{
		/* increment counting semaphore to show someone cares about multiple clicks */
		bclicks = LHIWD(p) & 0x000000ffL;
		if (bclicks > 1)
			gl_bpend++;
		e->e_parm = p;
		evinsert(e, &cda->c_bsleep);
	}
}


/*
 * wait for mouse rectangle
 */
VOID amouse(P(EVB *) e, P(int32_t) pmo)
PP(register EVB *e;)
PP(int32_t pmo;)
{
	MOBLK mob;

	LBCOPY(ADDR(&mob), pmo, sizeof(MOBLK));
	/* if already in (or out) signal immediately */
	if (mob.m_out != inside(xrat, yrat, &mob.m_x))
		zombie(e);
	else
	{
		if (mob.m_out)
			e->e_flag |= EVMOUT;
		else
			e->e_flag &= ~EVMOUT;
		e->e_parm = HW(mob.m_x) + mob.m_y;
		e->e_return = HW(mob.m_w) + mob.m_h;
		evinsert(e, &(cda->c_msleep));
	}
}
#endif /* UNLINKED */
