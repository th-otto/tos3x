/*
 * Rlist.c -  Handling rectangle lists for the Window Manager
 *
 * Jun 19 1991	ml.	Created this.
 *
 */
#include "aes.h"
#include "gemlib.h"

#if NEWWIN

#define      Malloc(a)       gemdos(0x48,a)
#define      Mfree(a)        gemdos(0x49,a)


BOOLEAN brkrect PROTO((GRECT *trect, GRECT *brect, uint16_t *hv_pc));
BOOLEAN chgrlist PROTO((uint16_t hv_pc, GRECT *cutrect, RLIST *oldrp, RLIST **rlist));
BOOLEAN mrgrect PROTO((RLIST *rp1, RLIST *rp2));
static RLIST *mkrect PROTO((uint16_t pc, GRECT *trect, GRECT *brect));
static RLIST *newrect PROTO((NOTHING));



/*
 * newrect() - allocate a RLIST structure for a new rectangle
 *	     - return a pointer to an available RLIST structure 
 *	       or return NULL if none is available.
 *
 */
static RLIST *newrect(NOTHING)
{
	register RLIST *rp;						/* pointer to RLIST structure */
	MEMHDR *mp;							/* pointer to memory header */
	register int16_t i;							/* counter */

	for (mp = rmhead; mp && mp->numused == NUMRECT; mp = mp->mnext)
		;

	if (!mp)
	{
		if ((mp = (MEMHDR *) Malloc((int32_t) (NUMRECT * sizeof(RLIST) + sizeof(MEMHDR)))) == NULL)
		{
			return (NULL);
		} else
		{
			/* init RLIST structures as available */
			for (rp = (RLIST *) (mp + 1), i = 0; i < NUMRECT; i++, rp++)
			{
				rp->rstatus.rused = FALSE;
				rp->rwhere = mp;
			}

			rmtail->mnext = mp;
			mp->mnext = NULL;
			rmtail = mp;
			rmtail->numused = 1;
			rp = (RLIST *) (rmtail + 1);
		}
	} else
	{
		for (rp = (RLIST *) (mp + 1); rp->rstatus.rused == TRUE; rp++)
			;

		mp->numused += 1;
	}

	rp->rstatus.rused = TRUE;
	rp->rnext = NULL;
	return (rp);
}



/*
 * delrect() - if a pointer to a rectangle is given, delete 
 *		that rectangle from the rectangle list.
 *		(rlist would be of type RLIST *).
 *	     - if no pointer to a particular rectangle is 
 *		given, delete the entire rectangle list.
 *		(rlist would be of type RLIST **).
 *
 */
BOOLEAN delrect(P(RLIST *)rp, P(VOIDPTR) rlist)
PP(RLIST *rp;)								/* ptr to rectangle to be deleted */
PP(VOIDPTR rlist;)							/* ptr (or address of ptr) to head of rect. list */
{
	register MEMHDR *prvmp,	*curmp,	*nxtmp;								/* memory blocks pointers */
	register RLIST *currp;					/* ptr into current rectangle */
	RLIST **rl1;						/* addr of ptr to rect. list head */

	if (rp)
	{									/* delete one rectangle */
		/*
		 * If rectangle to be deleted is head of list, update head
		 * of list.  If it is not the head of list, just unlink it
		 * from the list.
		 */
		rl1 = (RLIST **) rlist;
		if (rp == *rl1)
			*rl1 = rp->rnext;
		else
		{
			for (currp = *rl1; currp->rnext != rp; currp = currp->rnext)
				;
			currp->rnext = rp->rnext;
		}
		currp = rp;
	} else
	{									/* delete entire rectangle list */
		currp = (RLIST *) rlist;		/* so start at beginning of list */
	}


	/* mark the RLIST structure(s) as available */
	do
	{
		currp->rstatus.rused = FALSE;
		(currp->rwhere)->numused -= 1;
		if (rp)							/* if only delete one rectangle */
			currp = 0L;					/* terminate loop           */
		else
			currp = currp->rnext;		/* else go to next rectangle    */
	} while (currp);


	/* delete unused memory blocks */

	if (rmhead != rmtail)
	{
		prvmp = nxtmp = NULL;
		curmp = rmhead;

		while (curmp != NULL)
		{

			nxtmp = curmp->mnext;

			if (!curmp->numused)
			{

				if (prvmp)
				{
					prvmp->mnext = nxtmp;
				} else
				{
					rmhead = nxtmp;
				}
				Mfree(curmp);

			} else
			{
				prvmp = curmp;
			}

			curmp = nxtmp;
		}

		if (prvmp != rmtail)
			rmtail = prvmp;
	}


	return TRUE;
}



/*
 * Genrlist() - generate (non-optimized) rectangle list for given window.
 *	      - return pointer to rectangle list if successful.
 *	      - return NULL if an error occurred.
 */
RLIST *genrlist(P(uint16_t) handle, P(uint16_t) area)
PP(uint16_t handle;)							/* window handle */
PP(int16_t area;)								/* WF_WORKXYWH: generate rectangle list of work area */
			/* WF_CURRXYWH: generate rectangle list of whole window */
{
	WINDOW *wp;							/* ptr to current window structure */
	GRECT wrect;						/* rectangle of current window */
	RLIST *rlist;						/* ptr to beginning of rectangle list */
	RLIST *rnextrp;						/* ptr to next rectangle in rectangle list */
	register RLIST *currp;					/* ptr to current rectangle in rectangle list */
	uint16_t hv_piece;						/* flag for new rectangles found */

	/*
	 * if cannot locate window in database, or no RLIST 
	 * structure is available, return with an error. 
	 */
	if ((wp = srchwp(handle)) == NULL || (rlist = newrect()) == NULL)
		return NULL;

	/* initialize rectangle list */
	if (area == WF_WORKXYWH)
		rlist->rect = *(GRECT *) & (wp->obj[W_WORK].ob_x);
	else
	{									/* if (area == WF_CURRXYWH) */
		rlist->rect = wp->w_curr;
		rlist->rect.g_w += SHADOW;		/* add thickness for drop shadow */
		rlist->rect.g_h += SHADOW;		/* add thickness for drop shadow */
	}

	/* process all windows that are on top of the given window */
	while ((handle = wp->ontop) != NIL && rlist)
	{
		wp = srchwp(handle);			/* find next window to be processed */
		wrect = wp->w_curr;				/* find its current rectangle */
		wrect.g_w += SHADOW;			/* add thickness for drop shadow */
		wrect.g_h += SHADOW;

		rnextrp = rlist;				/* start from beginning of rect list */
		while ((currp = rnextrp) != NULL)
		{
			rnextrp = currp->rnext;
			if (brkrect(&wrect, &(currp->rect), &hv_piece))
			{
				chgrlist(hv_piece, &wrect, currp, &rlist);
			}
		}
	}

	/* optimize the rectangle list */
	for (currp = rlist; currp != NULL; currp = currp->rnext)
	{

		rnextrp = currp->rnext;
		while (rnextrp)
		{
			if (mrgrect(currp, rnextrp))
			{							/* try to merge 2 rects */
				delrect(rnextrp, &rlist);
				rnextrp = currp->rnext;
			} else
			{
				rnextrp = rnextrp->rnext;	/* progress to the next rect */
			}
		}
	}

	return rlist;
}



/*
 * Brkrect() - check if the top rect breaks the bottom rect
 *	     - return TRUE if the top rect breaks the bottom
 *	       rect and set up mask to reveal how the bottom
 *	       rect is broken down
 *	     - return FALSE if the top rect does not break 
 *	       the bottom rect
 */
BOOLEAN brkrect(P(GRECT *) trect, P(GRECT *) brect, P(uint16_t *) hv_pc)
PP(GRECT *trect;)
PP(GRECT *brect;)									/* ptrs to top and bottom rectangles */
PP(uint16_t *hv_pc;)
{
	/* check if the top rect breaks the bottom rect */
	if ((trect->g_x < brect->g_x + brect->g_w) &&
		(trect->g_x + trect->g_w > brect->g_x) &&
		(trect->g_y < brect->g_y + brect->g_h) && (trect->g_y + trect->g_h > brect->g_y))
	{

		*hv_pc = 0;						/* assume brect is entirely covered */

		/* brect may be broken down into smaller pieces */
		if (trect->g_y > brect->g_y)
			*hv_pc |= TOP;
		if (trect->g_x > brect->g_x)
			*hv_pc |= LEFT;
		if (trect->g_x + trect->g_w < brect->g_x + brect->g_w)
			*hv_pc |= RIGHT;
		if (trect->g_y + trect->g_h < brect->g_y + brect->g_h)
			*hv_pc |= BOTTOM;

		return TRUE;
	}
	return FALSE;
}



/*
 * Chgrlist() - change the rectangle list according to the 
 *		given mask by replacing the given rectangle
 *		with the rectangles specified by the mask
 *		and the cutting rectangle
 *	      - return TRUE if everything is fine or
 *		return FALSE if there is an error
 */
BOOLEAN chgrlist(P(uint16_t) hv_pc, P(GRECT *) cutrect, P(RLIST *) oldrp, P(RLIST **) rlist)
PP(uint16_t hv_pc;)							/* mask */
PP(GRECT *cutrect;)							/* ptr to the cutting rectangle */
PP(RLIST *oldrp;)							/* ptr to rectangle to be replaced */
PP(RLIST **rlist;)							/* addr of ptr to beginning of rectangle list */
{
	GRECT *old;							/* ptr to old rectangle */
	RLIST *xrlist;						/* ptr to the rectangle list expansion */
	RLIST *nrp;							/* ptr to new rectangle generated */
	RLIST *currp;						/* ptr to current rectangle in expansion */
	uint16_t pc;							/* counter */

	/*
	 * If given rectangle is entirely covered, just delete it 
	 * from the rectangle list, and free the RLIST structure.
	 */

	if (hv_pc)
	{
		/*
		 * Given rectangle is being broken into smaller pieces.
		 * Replace the given rectangle with a list of rectangles
		 * to represent the smaller pieces.
		 */

		old = (GRECT *) & (oldrp->rect);
		currp = xrlist = NULL;

		/* generate a rectangle for each piece */
		for (pc = TOP; pc <= BOTTOM; pc <<= 1)
		{
			if (hv_pc & pc)
			{
				if ((nrp = mkrect(pc, cutrect, old)) == NULL)
					return FALSE;

				if (xrlist)
					currp->rnext = nrp;
				else
					xrlist = nrp;

				currp = nrp;
			}
		}

		/* add extension to rectangle list */
		currp->rnext = oldrp->rnext;
		oldrp->rnext = xrlist;
	}
	delrect(oldrp, rlist);
	return TRUE;
}



/*
 * Mkrect() - make a rectangle out of the exposed area of the
 *	      bottom window specified by the flag
 *	    - return pointer to RLIST structure that contains
 *	      the rectangle
 *	    - return NULL if no RLIST structure is available
 */
static RLIST *mkrect(P(uint16_t) pc, P(GRECT *) trect, P(GRECT *) brect)
PP(uint16_t pc;)								/* flag */
PP(register GRECT *trect;)						/* ptr to rectangle of window on top */
PP(register GRECT *brect;)						/* ptr to rectangle of window at the bottom */
{
	RLIST *rp;									/* ptr to RLIST strucuture of new rectangle */
	register GRECT *nrect;						/* ptr to new rectangle */

	if ((rp = newrect()) == NULL)
		return NULL;

	nrect = &rp->rect;

	/* do common calculations */
	nrect->g_x = brect->g_x;
	nrect->g_y = max(brect->g_y, trect->g_y);
	nrect->g_w = brect->g_w;
	nrect->g_h = min(brect->g_y + brect->g_h, trect->g_y + trect->g_h) - nrect->g_y;

	switch (pc)
	{
	case TOP:
		nrect->g_y = brect->g_y;
		nrect->g_h = trect->g_y - brect->g_y;
		break;

	case LEFT:
		nrect->g_w = trect->g_x - brect->g_x;
		break;

	case RIGHT:
		nrect->g_x = trect->g_x + trect->g_w;
		nrect->g_w = (brect->g_x + brect->g_w) - (trect->g_x + trect->g_w);
		break;

	case BOTTOM:
		nrect->g_y = trect->g_y + trect->g_h;
		nrect->g_h = (brect->g_y + brect->g_h) - (trect->g_y + trect->g_h);
		break;
	}
	return rp;
}



/*
 * Mrgrect() - merge the 2 rectangles vertically if possible, and 
 *		save the result in the first rectangle
 *	     - return TRUE if there is a merge
 *	     - return FALSE if there is no merge
 */
BOOLEAN mrgrect(P(RLIST *) rp1, P(RLIST *) rp2)
PP(RLIST *rp1;)
PP(RLIST *rp2;)									/* ptrs to the 2 rectangles */
{
	GRECT *gr1, *gr2;

	gr1 = (GRECT *) & (rp1->rect);
	gr2 = (GRECT *) & (rp2->rect);

	if (gr1->g_x == gr2->g_x && gr1->g_w == gr2->g_w && gr1->g_y + gr1->g_h == gr2->g_y)
	{

		gr1->g_h += gr2->g_h;			/* rect1 = rect1 merge rect2 */
		return TRUE;
	}

	return FALSE;
}

#endif
