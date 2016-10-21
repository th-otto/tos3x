/*
 ******************************** ttcolor.c *************************************
 *
 * ==============================================================================
 * $Author: lozben $	$Date: 91/07/30 16:39:20 $
 * ==============================================================================
 *
 * Revision 3.1  91/07/30  16:39:20  lozben
 * Made changes to work with the new linea variable structure.
 * 
 * Revision 3.0  91/01/03  15:20:00  lozben
 * New generation VDI
 * 
 * Revision 2.6  90/06/12  16:43:21  lozben
 * We now map each bank to its own request color area (rezes that have banks).
 * 
 * Revision 2.5  90/05/31  17:46:48  lozben
 * Changed the code so that each bank has its own request color array.
 * This makes sence only in the 320x200, 640x200, 640x480 video modes.
 * 
 * Revision 2.4  90/02/05  17:07:35  lozben
 * Added code to look at the inverse bit in the 640x400x1 mode.
 * 
 * Revision 2.3  89/07/28  21:20:03  lozben
 * We now always (almost always) use MAP_COL[] instead of when the pen is
 * less than 16. Made it so VDI does not know about the togle bit in 640x400
 * duochrome mode.
 * 
 * Revision 2.2  89/04/14  18:49:23  lozben
 * *** Initial Revision ***
 * 
 ********************************************************************************
 */

#include "vdi.h"
#include "fontdef.h"
#include "attrdef.h"
#include "scrndev.h"
#include "lineavar.h"
#include "gsxdef.h"
#include "gsxextrn.h"

#if PLANES8 /* whole file, 2.x uses assembler version */

#define VIDINFO     (* ((volatile short *) 0xff8262L))	/* video information register */
#define LUTADR      ((volatile short *) 0xff8400L)	/* look up table address      */
#define INVERT      2					/* corresponds to invert bit  */
#define HYPERMONO   VIDINFO & 0x1000	/* hyper mono mode            */


#if TOSVERSION < 0x400
/* duplicate; also in tables.c */
static int16_t const plane_mask[] = {
	0, 1,   3,   7,  15,  31,  63, 127, 255,
};
#endif


/* corresponding gun percentage for each absolute value (0-15) */
static short const pcnt_tab[] = {
	0, 67, 133, 200, 267, 333, 400, 467,
	533, 600, 667, 733, 800, 867, 933, 1000
};

/*----------------------------------------------------------------------------*/

/*
 * VDI #14 - vs_color - Set color representation
 */
#if TOSVERSION >= 0x400
VOID tt_vs_color(NOTHING)
#else
/* 306de: 00e06ea4 */
VOID vs_color(NOTHING)
#endif
{
	register short *ptr = LV(INTIN);
	register short *rgb;
	register short j, pen;
	register short mode = (VIDINFO >> 8) & 7;	/* get video info     */
	register short temp;
	register short total;


#if TOSVERSION >= 0x400
	j = tplane_mask[LV(v_planes)];			/* max pen allowed    */
#else
	j = plane_mask[LV(v_planes)];			/* max pen allowed    */
#endif


	if ((pen = *ptr++) > j)				/* is col in range    */
		return;

	/* assume we don't want the extended color array */
	rgb = &LV(REQ_COL)[pen][0];

	switch (mode)
	{
	case _320x200:
	case _640x200:
	case _640x480:

#if PLANES8
		temp = (VIDINFO & 15) << 4;		/* temp = bank * 16   */
		if (temp != 0)
			rgb = &LV(REQ_X_COL)[temp + pen - 16][0];	/* rgb -> extnded ary */
#endif

		pen = (MAP_COL[pen] & j) + temp;	/* get lut offset     */
		break;


	case _640x400:
		if (INVERT & LUTADR[0])			/* see if the invert bit is on    */
			pen = 1 - pen;				/* exchange bg and fg colors      */

		pen = 254 + pen;				/* regular video                  */
		break;


	case _1280x960:
		break;


	case _320x480:
#if PLANES8
		if (pen > 15)
			rgb = &LV(REQ_X_COL)[pen - 16][0];	/* rgb -> extended col array  */
#endif

		pen = MAP_COL[pen];				/* get lut offset                 */
		break;
	}

	*rgb++ = *ptr++;					/* copy RED, GREEN, components    */
	*rgb++ = *ptr++;					/* into the request col array     */
	*rgb = *ptr;
	ptr = LV(INTIN) + 1;					/* point to red comp              */


	if (mode == _1280x960)
		return;							/* if mono case don't set luts    */


	/*
	 * load RGB (or GRAY scale) values into total
	 */
	for (j = total = 0; j < 3; j++)
	{
		if ((temp = *ptr++) > 1000)
			temp = 1000;				/* make sure temp is in range     */

		if (temp < 0)
			temp = 0;


		if (HYPERMONO)
		{
			if (!j)
				total += SMUL_DIV(temp, 30, 100);	/* red brightness val */
			else if (j == 1)			/*   temp * 30 / 100  */
				total += SMUL_DIV(temp, 59, 100);	/* green brightns val */
			else
			{							/*   temp * 59 / 100  */
				total += SMUL_DIV(temp, 11, 100);	/* blue brightnss val */
				total = SMUL_DIV(total, 255, 1000);	/* we have 8 bit dac  */
			}
		} else
		{
			temp *= 15;					/* gun value          */
			temp += 500;				/* for rounding       */
			temp /= 1000;				/* temp <- % of comp  */
			total = (total << 4) | temp;	/* total has RGB val  */
		}
	}


	LUTADR[pen] = total;				/* register <- total  */
}

/*----------------------------------------------------------------------------*/

/*
 * VDI #26 - vq_color - Inquire color representation
 */
#if TOSVERSION >= 0x400
VOID tt_vq_color(NOTHING)
#else
/* 306de: 00e07052 */
VOID vq_color(NOTHING)
#endif
{
	register short i, j;
	register short *ptr = LV(INTIN);		/* col index val      */
	register short *rgb;
	register short *out = LV(INTOUT);
	register short mode = (VIDINFO >> 8) & 7;	/* get video mode     */
	register short pen;
	register short temp;

	NINTOUT = 4;						/* # of output ints   */

#if TOSVERSION >= 0x400
	j = tplane_mask[LV(v_planes)];			/* max pen allowed    */
#else
	j = plane_mask[LV(v_planes)];			/* max pen allowed    */
#endif

	if ((pen = *ptr++) > j)
	{									/* col ndx in range ? */
		*out = -1;						/* invalid index      */
		return;
	}

	*out++ = pen;

	/* assume we don't want the extended color array */
	rgb = &LV(REQ_COL)[pen][0];


	switch (mode)
	{
	case _1280x960:
		break;

	case _640x400:
		if (INVERT & LUTADR[0])			/* is invert bit on   */
			pen = 1 - pen;				/* reverse bg and fg  */

		pen = 254 + pen;				/* regular video      */
		break;

	case _320x480:
#if PLANES8
		if (pen > 15)
			rgb = &LV(REQ_X_COL)[pen - 16][0];
#endif

		pen = (MAP_COL[pen] & j);
		break;

	default:
#if PLANES8
		i = (VIDINFO & 15) << 4;		/* i = bank * 16      */
		if (i != 0)
			rgb = &LV(REQ_X_COL)[i + pen - 16][0];
#endif

		pen = (MAP_COL[pen] & j) + i;
	}

	if (*ptr--)
	{									/* return col val set */
		if (mode != _1280x960)
		{								/* do color           */
			temp = LUTADR[pen];			/* temp is color info */

			if (HYPERMONO)
			{
				for (i = 0; i < 3; i++)
				{
					temp = *rgb++;		/* old requested val  */

					if (temp > 1000)
						temp = 1000;	/* make temp in range */

					if (temp < 0)
						temp = 0;		/* make temp in range */

					temp *= 15;			/* mult max gun value */
					temp += 500;		/* for rounding       */
					temp /= 1000;		/* truncate           */
					*out++ = pcnt_tab[temp];	/* store component %  */
				}
			}

			else
			{
				i = (temp >> 8) & 15;	/* get red component  */
				*out++ = pcnt_tab[i];

				i = (temp >> 4) & 15;	/* get green compont  */
				*out++ = pcnt_tab[i];

				i = temp & 15;			/* get blue component */
				*out = pcnt_tab[i];
			}
		} else
		{								/* do monochrome      */
			if (*ptr)					/* pen = 1        */
				*out = out[1] = out[2] = 0;	/* color is black     */
			else
				*out = out[1] = out[2] = 1000;	/* color is white     */
		}
	} else
	{									/* ret val requested  */
		*out++ = *rgb++;				/* get red component  */
		*out++ = *rgb++;				/* get green compont  */
		*out = *rgb;					/* get blue component */
	}
}

#endif
