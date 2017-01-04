/*	GEMOBLIB.C	03/15/84 - 02/08/85	Gregg Morris		*/
/*	to 68k		03/09/85 - 04/05/85	Lowell Webster		*/
/*	Reg Opt		03/11/85		Derek Mui		*/
/*	Clean up	09/19/85		Derek Mui		*/
/*	Trying 1.2	10/11/85		Derek Mui		*/
/*	removed one variable from get_par	01/03/87 Mike Schmal	*/
/*	Fix at ob_draw of get_par	3/9/87	Derek Mui		*/
/*	Restore back into C language from Ver 3.01	8/29/90	D.Mui	*/
/*	Added 3D and color icon			7/7/92	D.Mui		*/
/*	Folded in new 3D codes			8/1/92	D.Mui		*/
/*	Various 3D button & 256 color fixes,				*/
/*	new call ob_gclip()			8/6/92	kbad		*/

/*
 *	-------------------------------------------------------------
 *	GEM Application Environment Services		  Version 1.1
 *	Serial No.  XXXX-0000-654321		  All Rights Reserved
 *	Copyright (C) 1985			Digital Research Inc.
 *	-------------------------------------------------------------
 */

#include "aes.h"
#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"


#define      Malloc(a)       gemdos(0x48,a)
#define      Mfree(a)        gemdos(0x49,a)


VOID just_draw PROTO((LPTREE tree, int16_t obj, int16_t sx, int16_t sy));
int16_t ob_user PROTO((LPTREE tree, int16_t obj, GRECT *pt, intptr_t userblk, int16_t curr_state, int16_t new_state));




#if COLORICON_SUPPORT

/*****************************************************************/
/*                   COLOR ICON DECLARATIONS                     */
/*****************************************************************/

STATIC int16_t gl_colmask[128];					/* global mask used by color icons */

				/* WARNING:  The size of this array has been */
				/* set to 128 words and no bound checking is */
				/* done by the code currently.  This can  */
				/* handle icons of 64 by 64 pixels.       */

/* This is the color table of RGB values that VDI expects in pixel-
 * packed mode (This whole table can be moved to a separate file).
 * The values were derived from 4 bit R, G, and B values that were
 * converted to 5 bit values.  The original values are from VDI's 
 * palette settings.
 */
static int16_t const rgb_tab[] = { 0xFFDF,
	0xF800, 0x7C0, 0xFFC0, 0x1F, 0xF81F, 0x7DF, 0xB596, 0x8410, 0xA000, 0x500,
	0xA500, 0x14, 0xA014, 0x514, 0x0, 0xFFDF, 0xE71C, 0xD69A, 0xC618, 0xB596,
	0xA514, 0x9492, 0x8410, 0x738E, 0x630C, 0x528A, 0x4208, 0x3186, 0x2104, 0x1082,
	0x0, 0xF800, 0xF802, 0xF804, 0xF806, 0xF808, 0xF80A, 0xF80C, 0xF80E, 0xF810,
	0xF812, 0xF814, 0xF816, 0xF818, 0xF81A, 0xF81C, 0xF81F, 0xE01F, 0xD01F, 0xC01F,
	0xB01F, 0xA01F, 0x901F, 0x801F, 0x701F, 0x601F, 0x501F, 0x401F, 0x301F, 0x201F,
	0x101F, 0x1F, 0x9F, 0x11F, 0x19F, 0x21F, 0x29F, 0x31F, 0x39F, 0x41F,
	0x49F, 0x51F, 0x59F, 0x61F, 0x69F, 0x71F, 0x7DF, 0x7DC, 0x7DA, 0x7D8,
	0x7D6, 0x7D4, 0x7D2, 0x7D0, 0x7CE, 0x7CC, 0x7CA, 0x7C8, 0x7C6, 0x7C4,
	0x7C2, 0x7C0, 0x17C0, 0x27C0, 0x37C0, 0x47C0, 0x57C0, 0x67C0, 0x77C0, 0x87C0,
	0x97C0, 0xA7C0, 0xB7C0, 0xC7C0, 0xD7C0, 0xE7C0, 0xFFC0, 0xFF00, 0xFE80, 0xFE00,
	0xFD80, 0xFD00, 0xFC80, 0xFC00, 0xFB80, 0xFB00, 0xFA80, 0xFA00, 0xF980, 0xF900,
	0xF880, 0xB000, 0xB002, 0xB004, 0xB006, 0xB008, 0xB00A, 0xB00C, 0xB00E, 0xB010,
	0xB012, 0xB014, 0xB016, 0xA016, 0x9016, 0x8016, 0x7016, 0x6016, 0x5016, 0x4016,
	0x3016, 0x2016, 0x1016, 0x16, 0x96, 0x116, 0x196, 0x216, 0x296, 0x316,
	0x396, 0x416, 0x496, 0x516, 0x596, 0x594, 0x592, 0x590, 0x58E, 0x58C,
	0x58A, 0x588, 0x586, 0x584, 0x582, 0x580, 0x1580, 0x2580, 0x3580, 0x4580,
	0x5580, 0x6580, 0x7580, 0x8580, 0x9580, 0xA580, 0xB580, 0xB500, 0xB480, 0xB400,
	0xB380, 0xB300, 0xB280, 0xB200, 0xB180, 0xB100, 0xB080, 0x7000, 0x7002, 0x7004,
	0x7006, 0x7008, 0x700A, 0x700C, 0x700E, 0x600E, 0x500E, 0x400E, 0x300E, 0x200E,
	0x100E, 0xE, 0x8E, 0x10E, 0x18E, 0x20E, 0x28E, 0x30E, 0x38E, 0x38C,
	0x38A, 0x388, 0x386, 0x384, 0x382, 0x380, 0x1380, 0x2380, 0x3380, 0x4380,
	0x5380, 0x6380, 0x7380, 0x7300, 0x7280, 0x7200, 0x7180, 0x7100, 0x7080, 0x4000,
	0x4002, 0x4004, 0x4006, 0x4008, 0x3008, 0x2008, 0x1008, 0x8, 0x88, 0x108,
	0x188, 0x208, 0x206, 0x204, 0x202, 0x200, 0x1200, 0x2200, 0x3200, 0x4200,
	0x4180, 0x4100, 0x4080, 0xFFDF, 0x0
};

/******************** END COLOR *******************************/
#endif


#if BINEXACT
STATIC TEDINFO edblk;
STATIC BITBLK bi; /* WTF */
STATIC ICONBLK ib;
#endif

typedef uint16_t (*PARMBFUNC) PROTO((PARMBLK *f_data));

uint16_t far_call PROTO((PARMBFUNC f_code, PARMBLK *f_data));

/* 306de: 00e232d0 */
uint16_t far_call(P(PARMBFUNC) fcode, P(PARMBLK *) fdata)
PP(PARMBFUNC fcode;)
PP(PARMBLK *fdata;)
{
	return (*fcode) (fdata);
}



#if AES3D
/*
 * AES #48 - objc_sysvar - Manipulation of 3D objects. 
 *
 *	Routine to get or set object extension settings
 */
int16_t ob_sysvar(P(uint16_t) mode, P(uint16_t) which, P(uint16_t) inval1, P(uint16_t) inval2, P(int16_t *) outval1, P(int16_t *) outval2)
PP(uint16_t mode;)
PP(uint16_t which;)
PP(uint16_t inval1;)
PP(uint16_t inval2;)
PP(int16_t *outval1;)
PP(int16_t *outval2;)
{
	int ret;
	ret = TRUE;							/* assume OK */

	if (mode)
	{									/* if set */
		switch (which)
		{
		case LK3DIND:
		case LK3DACT:
			if (inval1 != -1)
			{
				if (which == LK3DIND)
				{
					ind3dtxt = inval1;
				} else
				{
					act3dtxt = inval1;
				}
			}

			if (inval2 != -1)
			{
				if (which == LK3DIND)
				{
					ind3dface = inval2;
				} else
				{
					act3dface = inval2;
				}
			}

			break;

		case INDBUTCOL:
		case ACTBUTCOL:
		case ALRTCOL:
			if (gl_ws.ws_ncolors <= inval1)
				return (FALSE);			/* return error if invalid */

			if (which == INDBUTCOL)
				gl_indbutcol = inval1;	/* set indicator button color */
			else if (which == ACTBUTCOL)
				gl_actbutcol = inval1;	/* set activator button color */
			else
				gl_alrtcol = inval1;	/* set alert background color */

			break;

		default:
			ret = FALSE;				/* error */
			break;
		}

	} else
	{
		switch (which)
		{
		case LK3DIND:
			*outval1 = ind3dtxt;
			*outval2 = ind3dface;
			break;
		case LK3DACT:
			*outval1 = act3dtxt;
			*outval2 = act3dface;
			break;
		case INDBUTCOL:
			*outval1 = gl_indbutcol;
			break;
		case ACTBUTCOL:
			*outval1 = gl_actbutcol;
			break;
		case ALRTCOL:
			*outval1 = gl_alrtcol;
			break;
		case AD3DVALUE:
			*outval1 = ADJ3DPIX;		/* horizontal   */
			*outval2 = ADJ3DPIX;		/* vertical */
			break;
		default:
			ret = FALSE;				/* error */
			break;
		}

	}
	return ret;
}
#endif


/*
 *	Routine to take an unformatted raw string and based on a
 *	template string build a formatted string.
 */
/* 306de: 00e232e4 */
VOID ob_format(P(int16_t) just, P(char *) raw_str, P(char *) tmpl_str, P(char *) fmtstr)
PP(int16_t just;)
PP(char *raw_str;)
PP(char *tmpl_str;)
PP(char *fmtstr;)
{
	register char *pfbeg, *ptbeg, *prbeg;
	char *pfend, *ptend, *prend;
	register int16_t inc, ptlen, prlen;

	UNUSED(pfend);
	
	if (*raw_str == '@')
		*raw_str = '\0';

	pfbeg = fmtstr;
	ptbeg = tmpl_str;
	prbeg = raw_str;

	ptlen = strlen(tmpl_str);
	prlen = strlen(raw_str);

	inc = 1;
	pfbeg[ptlen] = '\0';
	if (just == TE_RIGHT)
	{
		inc = -1;
		pfbeg = pfbeg + ptlen - 1;
		ptbeg = ptbeg + ptlen - 1;
		prbeg = prbeg + prlen - 1;
	}

	pfend = pfbeg + (inc * ptlen);
	ptend = ptbeg + (inc * ptlen);
	prend = prbeg + (inc * prlen);

	while (ptbeg != ptend)
	{
		if (*ptbeg != '_')
		{
			*pfbeg = *ptbeg;
		} else
		{
			if (prbeg != prend)
			{
				*pfbeg = *prbeg;
				prbeg += inc;
			} else
				*pfbeg = '_';
		}
		pfbeg += inc;
		ptbeg += inc;
	}
}


/*
 *	Routine to load up and call a user defined object draw or change 
 *	routine.
 */
/* 306de: 00e2339a */
int16_t ob_user(P(LPTREE) tree, P(int16_t) obj, P(GRECT *) pt, P(intptr_t) userblk, P(int16_t) curr_state, P(int16_t) new_state)
PP(LPTREE tree;)
PP(int16_t obj;)
PP(GRECT *pt;)
PP(intptr_t userblk;)
PP(int16_t curr_state;)
PP(int16_t new_state;)
{
	PARMBLK pb;

	pb.pb_tree = (OBJECT *)tree;
	pb.pb_obj = obj;
	pb.pb_prevstate = curr_state;
	pb.pb_currstate = new_state;
	rc_copy(pt, (GRECT *)&pb.pb_x);
	gsx_gclip((GRECT *)&pb.pb_xc);
	pb.pb_parm = LLGET(userblk + 4);
	return far_call((PARMBFUNC)(intptr_t)LLGET(userblk), ADDR(&pb));
}



#if AES3D

int16_t xor16 PROTO((int16_t col));
BOOLEAN xor_ok PROTO((int16_t type, int16_t flags, intptr_t spec));
VOID draw_hi PROTO((GRECT *prect, int16_t state, int16_t clip, int16_t th, int16_t icol));

/*
 *  Draw highlights around a rectangle depending on its state
 */
VOID draw_hi(P(GRECT *) prect, P(int16_t) state, P(int16_t) clip, P(int16_t) th, P(int16_t) icol)
PP(register GRECT *prect;)						/* object rectangle */
PP(int16_t state;)								/* NORMAL or SELECTED */
PP(int16_t clip;)								/* 1: set clipping rect to object itself */
PP(int16_t th;)									/* thickness of rectangle */
PP(int16_t icol;)								/* interior color */
{
	int16_t pts[12], col;
	GRECT r;

	gsx_moff();
	gsx_1code(15, 7);
	gsx_1code(113, -1);					/* solid line style */

	if (clip)
		gsx_sclip(prect);

	rc_copy(prect, &r);

	if (th > 0)
	{									/* if border is inside,     */
		r.g_x += th;					/* object area is 1 pixel   */
		r.g_y += th;					/* in all around        */
		r.g_w -= th << 1;
		r.g_h -= th << 1;
	}

	/* Draw hilite:
	 *   +--------------+
	 *   |[2,3] - [4,5] |
	 *   |  |           |
	 *   |[0,1]         |
	 *   |              |
	 *   +--------------+
	 */
	pts[0] = pts[2] = r.g_x;
	pts[3] = pts[5] = r.g_y;
	pts[1] = pts[3] + r.g_h - 2;
	pts[4] = pts[2] + r.g_w - 2;

	col = (state & SELECTED) ? BLACK : WHITE;
	gsx_attr(0, MD_REPLACE, col);
	av_pline(3, pts);

	/* Draw shadow:
	 *   +--------------+
	 *   |              |
	 *   |         [4,5]|
	 *   |           |  |
	 *   | [0,1] - [2,3]|
	 *   +--------------+
	 */
	pts[0]++;
	pts[3] = ++pts[1];
	pts[2] = ++pts[4];
	pts[5]++;

	if (state & SELECTED)
	{
		col = LWHITE;
		if (icol == col || gl_ws.ws_ncolors < col)
			col = WHITE;
	} else
	{
		col = LBLACK;
		if (icol == col || gl_ws.ws_ncolors < col)
			col = BLACK;
	}
	gsx_attr(0, MD_REPLACE, col);
	av_pline(3, pts);

	gsx_mon();
}


/*
 * Routine to XOR color in any resolution as if there is only 16 colors.
 * FROM THE VDI MANUAL: Section 6 (Raster Operations) Table 6-2
 * Pixel VDI #  Default color
 * ----- -----  -------------
 * 0000	    0	white
 * 0001	    2	red
 * 0010	    3	green
 * 0011	    6	yellow
 * 0100	    4	blue
 * 0101	    7	magenta
 * 0110	    5	cyan
 * 0111	    8	low white
 * 1000	    9	grey
 * 1001	    10	light red
 * 1010	    11	light green
 * 1011	    14	light yellow
 * 1100	    12	light blue
 * 1101	    15	light magenta
 * 1110	    13  light cyan
 * 1111	    1	black
 */


int16_t xor16(P(int16_t) col)
PP(int16_t col;)
{
	static int16_t const xor16tab[] = {
    /*  WHITE,  BLACK,  RED,   GREEN,    BLUE,    CYAN,  YELLOW,   MAGENTA */
		BLACK,  WHITE,  LCYAN, LMAGENTA, LYELLOW, LRED,  LBLUE,    LGREEN,
    /*  LWHITE, LBLACK, LRED,  LGREEN,   LBLUE,   LCYAN, LYELLOW,  LMAGENTA */
		LBLACK, LWHITE, CYAN,  MAGENTA,  YELLOW,  RED,   BLUE,     GREEN
	};
	static int16_t const xor4tab[] = { BLACK, WHITE, GREEN, RED };

	if (col >= 16 || col >= gl_ws.ws_ncolors)	/* ws_ncolors */
		col = WHITE;
	else if (gl_ws.ws_ncolors <= 4)
		col = xor4tab[col];
	else
		col = xor16tab[col];

	return col;
}


/*
 * Return 1 if XOR writing mode is OK to toggle an object's SELECTED state,
 * otherwise return 0.
 *
 * All strings and titles are OK to XOR, since they're always white/black.
 * Other NON-3d objects are OK to XOR if 16 or less colors are available,
 * or if they are all black & white.
 *
 * (used by just_draw() and ob_change())
 */
BOOLEAN xor_ok(P(int16_t) type, P(int16_t) flags, P(intptr_t) spec)
PP(int16_t type;)
PP(int16_t flags;)
PP(intptr_t spec;)
{
	int16_t i, tcol, icol;

	if (type == G_STRING || type == G_TITLE || type == G_IBOX)
		return 1;
	if (type == G_IMAGE || (flags & IS3DOBJ))
		return 0;
	if (gl_ws.ws_ncolors <= 16)
		return 1;

/* Get color word */
	switch (type)
	{
	case G_BOXTEXT:
	case G_FBOXTEXT:
	case G_TEXT:
	case G_FTEXT:
		i = ((TEDINFO *) spec)->te_color;
		break;
	case G_BOX:
	case G_BOXCHAR:
	case G_IBOX:
		i = LLOWD(spec);
		break;
	default:							/* black border, black text, transparent, hollow, white fill */
		i = 0xff00;
	}
	gr_crack(i, &i, &tcol, &i, &icol, &i);

	return (tcol < RED && icol < RED);
}
#endif /* AES3D */


/*
 *	Routine to draw an object from an object tree.
 */
/* 306de: 00e233f2 */
VOID just_draw(P(LPTREE) tree, P(int16_t) obj, P(int16_t) sx, P(int16_t) sy)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(register int16_t sx;)
PP(register int16_t sy;)
{
	int16_t bcol, tcol, ipat, icol, tmode, th;
	int16_t state, obtype, len, flags;
	intptr_t spec;
	int16_t tmpx, tmpy, tmpth;
#if AES3D
	int16_t thick;
#endif
	char ch;
	GRECT t, c;
	register GRECT *pt;
#if !BINEXACT
	BITBLK bi;
	ICONBLK ib;
	TEDINFO edblk;
#endif
#if AES3D
	BOOLEAN mvtxt;
	BOOLEAN chcol;
	BOOLEAN three_d;
#endif

	pt = &t;

	ch = ob_sst(tree, obj, &spec, &state, &obtype, &flags, pt, &th);

	if ((flags & HIDETREE) || (spec == -1L))
		return;

#if AES3D
	thick = th;
#endif

	pt->g_x = sx;
	pt->g_y = sy;

#if AES3D
	/*
	 * Adjust 3d object extents & get color change/move text flags
	 */
	if (flags & IS3DOBJ)
	{
		three_d = TRUE;					/* object is 3D */
		tmpx = ADJ3DPIX;
		pt->g_x -= tmpx;				/* expand object to accomodate */
		pt->g_y -= tmpx;				/*  hi-lights for 3D */
		pt->g_w += (tmpx << 1);
		pt->g_h += (tmpx << 1);

		if ((flags & IS3DACT) == 0)
		{								/* if it's a 3D indicator */
			mvtxt = ind3dtxt;
			chcol = ind3dface;
		} else
		{								/* if it's a 3D activator */
			mvtxt = act3dtxt;
			chcol = act3dface;
		}
	} else
	{
		/* For non-3d objects, force color change if XOR is not ok. */
		three_d = FALSE;
		chcol = !xor_ok(obtype, flags, spec);
		mvtxt = FALSE; /* quiet compiler */
	}
#endif

	/*
	 * do trivial reject with full extent including, outline, shadow,
	 * & thickness
	 * BUG: this rejects USERDEFs which draw more than a 3 border outline.
	 * It also rejects TEDINFOs with an outline thickness of more than 3 pixels.
	 */
	if (gl_wclip && gl_hclip)
	{
		rc_copy(pt, &c);
		if (state & OUTLINED)
			gr_inside(&c, -3);
		else
			gr_inside(&c, ((th < 0) ? (3 * th) : (-3 * th)));
		if (!(gsx_chkclip(&c)))
			return;
	}
	
	/*
	 * for all tedinfo types get copy of ted and crack the
	 * color word and set the text color
	 */
#if AES3D
	rc_copy(pt, &c);
#endif
	if (obtype != G_STRING)
	{
		tmpth = (th < 0) ? 0 : th;
		tmode = MD_REPLACE;
		tcol = BLACK;

		switch (obtype)
		{
		case G_BOXTEXT:
		case G_FBOXTEXT:
		case G_TEXT:
		case G_FTEXT:
			LBCOPY(&edblk, (VOIDPTR)spec, sizeof(TEDINFO));
			gr_crack(edblk.te_color, &bcol, &tcol, &ipat, &icol, &tmode);

#if AES3D
			/*
			 * if it's a 3D background object, draw it in 3D color
			 */
			/*
			 * this can get complicated, because gr_text will always draw a white
			 * background in replace mode. So we have to change TEXT objects
			 * into BOXTEXT objects with the right color background and with
			 * transparent text (and no borders)
			 */
			if ((flags & (IS3DOBJ | IS3DACT)) && icol == WHITE && ipat == IP_HOLLOW)
			{
				ipat = IP_SOLID;
				switch (flags & (IS3DOBJ | IS3DACT))
				{
				case IS3DOBJ:
					icol = gl_indbutcol;
					break;
				case (IS3DOBJ | IS3DACT):
					icol = gl_actbutcol;
					break;
				case IS3DACT:
					icol = gl_alrtcol;
					/* fall through */
				default:				/* should never happen!! */
					break;
				}
				icol = gl_alrtcol;
				if (tmode == MD_REPLACE)
				{
					if (obtype == G_TEXT)
					{
						obtype = G_BOXTEXT;
						tmpth = th = 0;
					} else if (obtype == G_FTEXT)
					{
						obtype = G_FBOXTEXT;
						tmpth = th = 0;
					}
					tmode = MD_TRANS;
				}
			}
#endif
			break;
		}
		
		/*
		 * for all box types crack the color if not ted and
		 * draw the box with border
		 */
		switch (obtype)
		{
		case G_BOX:
		case G_BOXCHAR:
		case G_IBOX:
			gr_crack(LLOWD(spec), &bcol, &tcol, &ipat, &icol, &tmode);
#if AES3D
			if (obtype != G_IBOX && ipat == IP_HOLLOW && icol == WHITE)
			{
				switch (flags & (IS3DOBJ | IS3DACT))
				{
				case IS3DOBJ:
					icol = gl_indbutcol;
					ipat = IP_SOLID;
					break;
				case (IS3DOBJ | IS3DACT):
					icol = gl_actbutcol;
					ipat = IP_SOLID;
					break;
				case IS3DACT:
					icol = gl_alrtcol;
					ipat = IP_SOLID;
					/* fall through */
				default:
					break;
				}
			}
#endif
			/* fall through */
		case G_BUTTON:
			if (obtype == G_BUTTON)
			{
				bcol = BLACK;

#if AES3D
				/* May 13 1992 - ml */
				if (three_d)
				{						/* 8/1/92 */
					ipat = IP_SOLID;
					if ((flags & IS3DACT) == 0)
					{
						icol = gl_indbutcol;
					} else
					{
						icol = gl_actbutcol;
					}
				} else
#endif
				{
					ipat = IP_HOLLOW;
					icol = WHITE;
				}

			}
			/* fall through */
		case G_BOXTEXT:
		case G_FBOXTEXT:
			/* draw box's border */
			if (th != 0)
			{
				gsx_attr(FALSE, MD_REPLACE, bcol);
				gr_box(pt->g_x, pt->g_y, pt->g_w, pt->g_h, th);
			}
			/* draw filled box  */
			if (obtype != G_IBOX)		/* 8/1/92 */
			{
				gr_inside(pt, tmpth);
#if AES3D
				if (chcol && (state & SELECTED))
				{
					/* Explicitly set a 4-bit XOR fill color.
					 * If pattern is hollow, make it solid
					 * so the color is visible.
					 */
					if (ipat == IP_HOLLOW)
					{
						ipat = IP_SOLID;
						icol = BLACK;
					} else
					{
						icol = xor16(icol);
					}
				}
#endif

				gr_rect(icol, ipat, pt);
				gr_inside(pt, -tmpth);

			}
			break;
		}

#if AES3D
		if (chcol && (state & SELECTED))
		{
			tmode = MD_TRANS;
			tcol = xor16(tcol);
		}
#endif

		gsx_attr(TRUE, tmode, tcol);
		
		/* do whats left for all the other types */
		switch (obtype)
		{
		case G_FTEXT:
		case G_FBOXTEXT:
			LSTCPY(&D.g_rawstr[0], edblk.te_ptext);
			LSTCPY(&D.g_tmpstr[0], edblk.te_ptmplt);
			ob_format(edblk.te_just, D.g_rawstr, D.g_tmpstr, D.g_fmtstr);
			/* fall thru to gr_gtext */
		case G_BOXCHAR:
			edblk.te_ptext = D.g_fmtstr;
			if (obtype == G_BOXCHAR)
			{
				D.g_fmtstr[0] = ch;
				D.g_fmtstr[1] = '\0';
				edblk.te_just = TE_CNTR;
				edblk.te_font = IBM;
			}
			/* fall thru to gr_gtext */
		case G_TEXT:
		case G_BOXTEXT:
			gr_inside(pt, tmpth);

			/* July 30 1992 - ml.  Draw text of 3D objects */ /* 8/1/92 */
#if AES3D
			if (three_d)
			{
				if (!(state & SELECTED) && mvtxt)
				{
					pt->g_x -= 1;
					pt->g_y -= 1;
					gr_gtext(edblk.te_just, edblk.te_font, edblk.te_ptext, pt, tmode);
					pt->g_x += 1;
					pt->g_y += 1;
				} else
				{
					gr_gtext(edblk.te_just, edblk.te_font, edblk.te_ptext, pt, tmode);
				}

			} else
			{
				gr_gtext(edblk.te_just, edblk.te_font, edblk.te_ptext, pt, tmode);
			}
#else
			gr_gtext(edblk.te_just, edblk.te_font, edblk.te_ptext, pt, tmode);
#endif
			gr_inside(pt, -tmpth);
			break;
		case G_IMAGE:
			LBCOPY(&bi, (VOIDPTR)spec, sizeof(BITBLK));
#if AES3D
			if (state & SELECTED)
			{
				/* If selected, XOR the background before drawing the image */
				bb_fill(MD_XOR, FIS_SOLID, IP_SOLID, pt->g_x, pt->g_y, pt->g_w, pt->g_h);
				bi.bi_color = xor16(bi.bi_color);
			}
#endif
			gsx_blt(bi.bi_pdata, bi.bi_x, bi.bi_y, bi.bi_wb,
					0x0L, pt->g_x, pt->g_y, gl_width / 8, bi.bi_wb * 8, bi.bi_hl, MD_TRANS, bi.bi_color, WHITE);
			break;
		case G_ICON:
			LBCOPY(&ib, (VOIDPTR)spec, sizeof(ICONBLK));
			ib.ib_xicon += pt->g_x;
			ib.ib_yicon += pt->g_y;
			ib.ib_xtext += pt->g_x;
			ib.ib_ytext += pt->g_y;
			gr_gicon(state, ib.ib_pmask, ib.ib_pdata, ib.ib_ptext,
					 ib.ib_char, ib.ib_xchar, ib.ib_ychar, (GRECT *)&ib.ib_xicon, (GRECT *)&ib.ib_xtext);
			state &= ~SELECTED;
			break;
#if COLORICON_SUPPORT
		case G_CICON:
			/*
			 * Identical to the monochrome icon case (above)
			 * except for the gr_cicon() call.
			 */
			LBCOPY(&ib, (VOIDPTR)spec, sizeof(ib));
			ib.ib_xicon += pt->g_x;
			ib.ib_yicon += pt->g_y;
			ib.ib_xtext += pt->g_x;
			ib.ib_ytext += pt->g_y;
			gr_cicon(state, ib.ib_pmask, ib.ib_pdata, ib.ib_ptext,
					 ib.ib_char, ib.ib_xchar, ib.ib_ychar, (GRECT *)&ib.ib_xicon, (GRECT *)&ib.ib_xtext, (CICONBLK *) spec);
			state &= ~SELECTED;
			break;
#endif
		case G_USERDEF:
			state = ob_user(tree, obj, pt, spec, state, state);
			break;
		}
	}
	if (obtype == G_STRING ||			/* 8/1/92 */
		obtype == G_TITLE ||
		obtype == G_BUTTON)
	{
		len = LBWMOV(ad_intin, (VOIDPTR)spec);
		if (len)
		{								/* 8/3/92 */
#if AES3D
			if ((state & SELECTED) && obtype == G_BUTTON && chcol)
				tcol = WHITE;
			else
				tcol = BLACK;
			gsx_attr(TRUE, MD_TRANS, tcol);
#else
			gsx_attr(TRUE, MD_TRANS, BLACK);
#endif

			tmpy = pt->g_y + ((pt->g_h - gl_hchar) / 2);
			if (obtype == G_BUTTON)
			{
				tmpx = pt->g_x + ((pt->g_w - (len * gl_wchar)) / 2);

#if AES3D
				/* July 30 1992 - ml. */
				if (three_d)
				{
					if (!(state & SELECTED) && mvtxt)
					{
						tmpx -= 1;
						tmpy -= 1;
					}
				}
#endif
			} else
			{
				tmpx = pt->g_x;
			}

			gsx_tblt(IBM, tmpx, tmpy, len);
		}
	}

	if (state)
	{
		if (state & OUTLINED)
		{
#if AES3D
			if ((flags & (IS3DACT | IS3DOBJ)) != IS3DACT)
#endif
			{
				gsx_attr(FALSE, MD_REPLACE, BLACK);
				gr_box(pt->g_x - 3, pt->g_y - 3, pt->g_w + 6, pt->g_h + 6, 1);
				gsx_attr(FALSE, MD_REPLACE, WHITE);
				gr_box(pt->g_x - 2, pt->g_y - 2, pt->g_w + 4, pt->g_h + 4, 2);
#if AES3D
			} else
			{
				/* draw a 3D outline for 3D background objects: 1/18/93 ERS */
				gsx_attr(FALSE, MD_REPLACE, LBLACK);
				gsx_cline(pt->g_x + pt->g_w + 2, pt->g_y - 3, pt->g_x + pt->g_w + 2, pt->g_y + pt->g_h + 2);
				gsx_cline(pt->g_x + pt->g_w + 1, pt->g_y - 2, pt->g_x + pt->g_w + 1, pt->g_y + pt->g_h + 1);
				gsx_cline(pt->g_x + pt->g_w, pt->g_y - 1, pt->g_x + pt->g_w, pt->g_y + pt->g_h);
				gsx_cline(pt->g_x - 3, pt->g_y + pt->g_h + 2, pt->g_x + pt->g_w + 2, pt->g_y + pt->g_h + 2);
				gsx_cline(pt->g_x - 2, pt->g_y + pt->g_h + 1, pt->g_x + pt->g_w + 2, pt->g_y + pt->g_h + 1);
				gsx_cline(pt->g_x - 1, pt->g_y + pt->g_h, pt->g_x + pt->g_w + 2, pt->g_y + pt->g_h);

				gsx_attr(FALSE, MD_REPLACE, WHITE);
				gsx_cline(pt->g_x - 3, pt->g_y - 3, pt->g_x + pt->g_w + 2, pt->g_y - 3);
				gsx_cline(pt->g_x - 3, pt->g_y - 2, pt->g_x + pt->g_w + 1, pt->g_y - 2);
				gsx_cline(pt->g_x - 3, pt->g_y - 1, pt->g_x + pt->g_w, pt->g_y - 1);
				gsx_cline(pt->g_x - 3, pt->g_y - 3, pt->g_x - 3, pt->g_y + pt->g_h + 2);
				gsx_cline(pt->g_x - 2, pt->g_y - 3, pt->g_x - 2, pt->g_y + pt->g_h + 1);
				gsx_cline(pt->g_x - 1, pt->g_y - 3, pt->g_x - 1, pt->g_y + pt->g_h);
#endif
			}
		}

		if (th > 0)
			gr_inside(pt, th);
		else
			th = -th;

		if ((state & SHADOWED) && th)
		{
			vsf_color(bcol);
			/* draw the vertical line */
			bb_fill(MD_REPLACE, FIS_SOLID, 0, pt->g_x, pt->g_y + pt->g_h + th, pt->g_w + th, 2 * th);
			/* draw the horizontal line */
			bb_fill(MD_REPLACE, FIS_SOLID, 0, pt->g_x + pt->g_w + th, pt->g_y, 2 * th, pt->g_h + (3 * th));
		}

		if (state & CHECKED)
		{
			gsx_attr(TRUE, MD_TRANS, BLACK);
			intin[0] = 0x08;			/* a check mark */
			gsx_tblt(IBM, pt->g_x + 2, pt->g_y, 1);
		}

		if (state & CROSSED)
		{
			gsx_attr(FALSE, MD_TRANS, WHITE);
			gsx_cline(pt->g_x, pt->g_y, pt->g_x + pt->g_w - 1, pt->g_y + pt->g_h - 1);
			gsx_cline(pt->g_x, pt->g_y + pt->g_h - 1, pt->g_x + pt->g_w - 1, pt->g_y);
		}

		if (state & DISABLED)
		{
#if AES3D
			if ((flags & (IS3DOBJ | IS3DACT)) == IS3DACT)
				vsf_color(gl_alrtcol);
			else
#endif
				vsf_color(WHITE);

			bb_fill(MD_TRANS, FIS_PATTERN, IP_4PATT, pt->g_x, pt->g_y, pt->g_w, pt->g_h);
		}

		if ((state & SELECTED)
#if AES3D
			&& !(chcol || three_d)
#endif
			)
		{
			bb_fill(MD_XOR, FIS_SOLID, IP_SOLID, pt->g_x, pt->g_y, pt->g_w, pt->g_h);
		}
		/* July 30 1992 - ml */ /* 8/1/92 */
	}

#if AES3D
	if (three_d)
	{
		if (state & SELECTED)
			draw_hi(&c, SELECTED, FALSE, thick, icol);
		else
			draw_hi(&c, NORMAL, FALSE, thick, icol);
	}
#endif
}



/*
 * AES #42 - objc_draw - Draw an AES object tree.
 *
 *	Object draw routine that walks tree and draws appropriate objects.
 */
/* 306de: 00e23ab2 */
VOID ob_draw(P(LPTREE) tree, P(int16_t) obj, P(int16_t) depth)
PP(register LPTREE tree;)
PP(int16_t obj;)
PP(int16_t depth;)
{
	int16_t last, pobj;
	int16_t sx, sy;

	last = (obj == ROOT) ? NIL : LWGET(OB_NEXT(obj));

	pobj = get_par(tree, obj);

	if (pobj != NIL)
		ob_offset(tree, pobj, &sx, &sy);
	else
		sx = sy = 0;

	gsx_moff();
	everyobj(tree, obj, last, just_draw, sx, sy, depth);
	gsx_mon();
}


/*
 * AES #43 - objc_find - Find which object lies at the specified screen position.
 *
 *	Routine to find out which object a certain mx,my value is
 *	over.  Since each parent object contains its children the
 *	idea is to walk down the tree, limited by the depth parameter,
 *	and find the last object the mx,my location was over.
 */
/************************************************************************/
/* o b _ f i n d                                                        */
/************************************************************************/
/* 306de: 00e23b52 */
int16_t ob_find(P(LPTREE) tree, P(int16_t) currobj, P(int16_t) depth, P(int16_t) mx, P(int16_t) my)
PP(register LPTREE tree;)
PP(register int16_t currobj;)
PP(register int16_t depth;)
PP(int16_t mx;)
PP(int16_t my;)
{
	int16_t lastfound;
#if AES3D
	int16_t dummy;
#endif
	int16_t dosibs;
	BOOLEAN done;
#if AES3D
	int16_t state;
#endif
	GRECT t, o;
	int16_t parent, childobj, flags;
	register GRECT *pt;

	pt = &t;

	lastfound = NIL;

	if (currobj == 0)
	{
		r_set(&o, 0, 0, 0, 0);
	} else
	{
		parent = get_par(tree, currobj);
		ob_actxywh(tree, parent, &o);
	}

	done = FALSE;
	dosibs = FALSE;

	while (!done)
	{
		/* if inside this obj, might be inside a child, so check */

#if AES3D
		state = LWGET(OB_STATE(currobj));
		if (state & SHADOWED)
#endif
		{
			ob_relxywh(tree, currobj, pt);
			pt->g_x += o.g_x;
			pt->g_y += o.g_y;
#if AES3D
		} else
		{
			ob_gclip(tree, currobj, &dummy, &dummy, &pt->g_x, &pt->g_y, &pt->g_w, &pt->g_h);
#endif
		}
		
		flags = LWGET(OB_FLAGS(currobj));
		if (inside(mx, my, pt) && !(flags & HIDETREE))
		{
			lastfound = currobj;

			childobj = LWGET(OB_TAIL(currobj));
			if ((childobj != NIL) && depth)
			{
				currobj = childobj;
				depth--;
				o.g_x = pt->g_x;
				o.g_y = pt->g_y;
				dosibs = TRUE;
			} else
			{
				done = TRUE;
			}
		} else
		{
			if (dosibs && lastfound != NIL)
			{
				currobj = get_prev(tree, lastfound, currobj);
				if (currobj == NIL)
					done = TRUE;
			} else
			{
				done = TRUE;
			}
		}
	}
	/* if no one was found this will return NIL */
	return lastfound;
}


/*
 * AES #40 - objc_add - Insert object in an object tree.
 *
 *	Routine to add a child object to a parent object.  The child
 *	is added at the end of the parent's current sibling list.
 *	It is also initialized.
 */
/* 306de: 00e23c82 */
VOID ob_add(P(LPTREE) tree, P(int16_t) parent, P(int16_t) child)
PP(register LPTREE tree;)
PP(register int16_t parent;)
PP(register int16_t child;)
{
	register int16_t lastkid;
	register LPTREE ptail;

	if (parent != NIL && child != NIL)
	{
		/* initialize child */
		LWSET(OB_NEXT(child), parent);

		lastkid = LWGET(ptail = OB_TAIL(parent));
		if (lastkid == NIL)
			/* this is parent's 1st kid, so both head and tail pt to it  */
			LWSET(OB_HEAD(parent), child);
		else
			/* add kid to end of kid list */
			LWSET(OB_NEXT(lastkid), child);
		LWSET(ptail, child);
	}
}


/*
 * AES #41 - objc_delete - Remove object from an object tree.
 *
 *	Routine to delete an object from the tree.
 */
/* 306de: 00e23cee */
/* BUG: doesnt return FALSE as documented */
VOID ob_delete(P(LPTREE) tree, P(int16_t) obj)
PP(register LPTREE tree;)
PP(register int16_t obj;)
{
	register int16_t parent;
	int16_t prev, nextsib;
	register LPTREE ptail, phead;

	if (obj != ROOT)
	{
		nextsib = LWGET(OB_NEXT(obj));
		parent = get_par(tree, obj);
	} else
		return;

	if (LWGET(phead = OB_HEAD(parent)) == obj)
	{
		/* this is head child in list */
		if (LWGET(ptail = OB_TAIL(parent)) == obj)
		{
			/* this is only child in list, so fix head & tail ptrs */
			nextsib = NIL;
			LWSET(ptail, NIL);
		}
		/* move head ptr to next child in list */
		LWSET(phead, nextsib);
	} else
	{
		/* it's somewhere else, so move pnext around it */
		prev = get_prev(tree, parent, obj);
		LWSET(OB_NEXT(prev), nextsib);
		if (LWGET(ptail = OB_TAIL(parent)) == obj)
			/* this is last child in list, so move tail ptr to prev child in list*/
			LWSET(ptail, prev);
	}
}


/*
 * AES #45 - objc_order - Alter order of object in object tree.
 *
 *	Routine to change the order of an object relative to its
 *	siblings in the tree.  0 is the head of the list and NIL
 *	is the tail of the list.
 */
/* 306de: 00e23d9c */
/* BUG: doesnt return FALSE as documented */
VOID ob_order(P(LPTREE) tree, P(int16_t) mov_obj, P(int16_t) new_pos)
PP(register LPTREE tree;)
PP(register int16_t mov_obj;)
PP(int16_t new_pos;)
{
	register int16_t parent;
	int16_t chg_obj, ii;
	register int32_t phead, pnext, pmove;

	if (mov_obj != ROOT)
		parent = get_par(tree, mov_obj);
	else
		return;

	ob_delete(tree, mov_obj);
	chg_obj = LWGET(phead = OB_HEAD(parent));
	pmove = OB_NEXT(mov_obj);
	if (new_pos == 0)
	{
		/* put mov_obj at head of list        */
		LWSET(pmove, chg_obj);
		LWSET(phead, mov_obj);
	} else
	{
		/* find new_pos     */
		if (new_pos == NIL)
		{
			chg_obj = LWGET(OB_TAIL(parent));
		} else
		{
			for (ii = 1; ii < new_pos; ii++)
				chg_obj = LWGET(OB_NEXT(chg_obj));
		}
		/* now add mov_obj after chg_obj */
		LWSET(pmove, LWGET(pnext = OB_NEXT(chg_obj)));
		LWSET(pnext, mov_obj);
	}
	if (LWGET(pmove) == parent)
		LWSET(OB_TAIL(parent), mov_obj);
}



/************************************************************************/
/* o b _ e d i t                                                        */
/************************************************************************/
/* see OBED.C */

/*
 * AES #47 - objc_change - Alter display of an object within specified limits.
 *
 *	Routine to change the state of an object and redraw that
 *	object using the current clip rectangle.
 */
/* 306de: 00e23e7a */
/* BUG: doesnt return FALSE as documented */
VOID ob_change(P(LPTREE) tree, P(int16_t) obj, P(int16_t) new_state, P(int16_t) redraw)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(uint16_t new_state;)
PP(int16_t redraw;)
{
	int16_t flags, obtype, th;
	GRECT t;
	int16_t curr_state;
	intptr_t spec;
	register GRECT *pt;

	pt = &t;

	ob_sst(tree, obj, &spec, &curr_state, &obtype, &flags, pt, &th);
	
	if ((curr_state == new_state) || (spec == -1L))
		return;

	LWSET(OB_STATE(obj), new_state);

	if (redraw)
	{
		ob_offset(tree, obj, &pt->g_x, &pt->g_y);

		gsx_moff();

		th = (th < 0) ? 0 : th;

		if (obtype == G_USERDEF)
		{
			ob_user(tree, obj, pt, spec, curr_state, new_state);
			redraw = FALSE;
		} else if (obtype != G_ICON && ((new_state ^ curr_state) & SELECTED))
		{
			/* For non-icon objects, see if we can toggle selection by XOR.
			 * G_IMAGE objects must be DEselected by XOR draw, *and*
			 * the image must be redrawn by just_draw().  If they're selected,
			 * just_draw() does the XOR box before redrawing the image.
			 */
#if AES3D
			BOOLEAN xok = xor_ok(obtype, flags, spec);

			if (xok || (obtype == G_IMAGE && !(new_state & SELECTED)))
#endif
			{
				bb_fill(MD_XOR, FIS_SOLID, IP_SOLID, pt->g_x + th,
						pt->g_y + th, pt->g_w - (2 * th), pt->g_h - (2 * th));
#if AES3D
				redraw = !xok;
#else
				redraw = FALSE;
#endif
			}
		}

		if (redraw)
			just_draw(tree, obj, pt->g_x, pt->g_y);

		gsx_mon();
	}
}


/* 306de: 00e23fb6 */
uint16_t ob_fs(P(LPTREE) tree, P(int16_t) ob, P(int16_t *) pflag)
PP(LPTREE tree;)
PP(int16_t ob;)
PP(int16_t *pflag;)
{
	*pflag = LWGET(OB_FLAGS(ob));
	return (LWGET(OB_STATE(ob)));
}


/************************************************************************/
/* o b _ a c t x y w h                                                  */
/************************************************************************/
/* 306de: 00e23fea */
VOID ob_actxywh(P(LPTREE) tree, P(int16_t) obj, P(GRECT *) pt)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(register GRECT *pt;)
{
#if AES3D
	int16_t x, y;

	ob_gclip(tree, obj, &x, &y, &pt->g_x, &pt->g_y, &pt->g_w, &pt->g_h);
#else
	ob_offset(tree, obj, &pt->g_x, &pt->g_y);
	pt->g_w = LWGET(OB_WIDTH(obj));
	pt->g_h = LWGET(OB_HEIGHT(obj));
#endif
}


/************************************************************************/
/* o b _ r e l x y w h                                                  */
/************************************************************************/
/* 306de: 00e24042 */
VOID ob_relxywh(P(LPTREE) tree, P(int16_t) obj, P(GRECT *)pt)
PP(LPTREE tree;)
PP(int16_t obj;)
PP(GRECT *pt;)
{
	LWCOPY(ADDR(pt), (VOIDPTR)OB_X(obj), sizeof(GRECT) / 2);
}


VOID ob_setxywh(P(LPTREE) tree, P(int16_t) obj, P(GRECT *) pt)
PP(LPTREE tree;)
PP(int16_t obj;)
PP(GRECT *pt;)
{
	LWCOPY((VOIDPTR)OB_X(obj), ADDR(pt), sizeof(GRECT) / 2);
}


/*
 * AES #44 - objc_offset - Calculate the true position of an object on the screen.
 *
 *	Routine to find the x,y offset of a particular object relative
 *	to the physical screen.  This involves accumulating the offsets
 *	of all the objects parents up to and including the root.
 */
/* 306de: 00e2409a */
VOID ob_offset(P(LPTREE) tree, P(int16_t) obj, P(int16_t *) pxoff, P(int16_t *) pyoff)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(register int16_t *pxoff;)
PP(register int16_t *pyoff;)
{
	*pxoff = *pyoff = 0;
	do
	{
		/* have our parent-- add in his x, y */
		*pxoff += LWGET(OB_X(obj));
		*pyoff += LWGET(OB_Y(obj));
		obj = get_par(tree, obj);
	} while (obj != NIL);
}


#if AES3D
/*
 * Return X, Y, W, and H deltas between basic object
 * rectangle and object clip rectangle.  These are the
 * numbers that you add to the x, y, w, h in the OBJECT
 * to get the clip rectangle.
 */
VOID ob_dxywh(P(LPTREE) tree, P(int16_t) obj, P(int16_t *) pdx, P(int16_t *) pdy, P(int16_t *) pdw, P(int16_t *) pdh)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(int16_t *pdx;)
PP(int16_t *pdy;)
PP(int16_t *pdw;)
PP(int16_t *pdh;)
{
	GRECT relr, actr;

	ob_relxywh(tree, obj, &relr);
#if BINEXACT
	/*
	 * BUG: ob_gclip takes separate arguments, not GRECT *
	 * this is serious, because ob_gclip() will write to pointers
	 * that are not passed here and most likely crash
	 */
	ob_gclip(tree, obj, &relr.g_x, &relr.g_y, &actr);
#else
	ob_gclip(tree, obj, &relr.g_x, &relr.g_y, &actr.g_x, &actr.g_y, &actr.g_w, &actr.g_h);
#endif
	*pdx = actr.g_x - relr.g_x;
	*pdy = actr.g_y - relr.g_y;
	*pdw = actr.g_w - relr.g_w;
	*pdh = actr.g_h - relr.g_h;
}
#endif

#if NEWWIN | AES3D

#define ADJOUTLPIX 3
#define ADJSHADPIX 2

/*
 * Return a clip rectangle describing an object's screen coordinates.
 */
VOID ob_gclip(P(LPTREE) tree, P(int16_t) obj, P(int16_t *) pxoff, P(int16_t *) pyoff, P(int16_t *) pxcl, P(int16_t *) pycl, P(int16_t *) pwcl, P(int16_t *) phcl)
PP(register LPTREE tree;)
PP(register int16_t obj;)
PP(int16_t *pxoff;)
PP(int16_t *pyoff;)
PP(int16_t *pxcl;)
PP(int16_t *pycl;)
PP(int16_t *pwcl;)
PP(int16_t *phcl;)
{
	GRECT r;
	int32_t spec;
	int16_t state, type, flags, border;
	int16_t x, y, off3d, offset;

	/*
	 * Get the object's base rectangle, & other stuff.
	 * NOTE: ob_sst() accounts for EXIT/DEFAULT button & G_TITLE outlines.
	 */
	ob_offset(tree, obj, &x, &y);
	ob_sst(tree, obj, &spec, &state, &type, &flags, &r, &border);
	*pxoff = r.g_x = x;
	*pyoff = r.g_y = y;

	/* Get gr_inside() offset */
	off3d = (flags & IS3DOBJ) ? ADJ3DPIX : 0;

	if (state & OUTLINED)
		offset = -ADJOUTLPIX - off3d;
	else if (border >= 0)				/* interior or no border */
		offset = -off3d, border = -border;
	else								/* outside border */
		offset = border - off3d;

	if (offset)
		gr_inside(&r, offset);

	/* Adjust for shadow */
	if (state & SHADOWED)
	{
		border = border * ADJSHADPIX - offset + border;
		if (border < 0)
		{
			r.g_w -= border;
			r.g_h -= border;
		}
	}
	r_get(&r, pxcl, pycl, pwcl, phcl);
}
#endif


/*
 *	Routine to find the object that is previous to us in the
 *	tree.  The idea is we get our parent and then walk down
 *	his list of children until we find a sibling who points to
 *	us.  If we are the first child or we have no parent then
 *	return NIL.
 */
/* 306de: 00e240fe */
int16_t get_prev(P(LPTREE) tree, P(int16_t) parent, P(int16_t) obj)
PP(register LPTREE tree;)
PP(int16_t parent;)
PP(register int16_t obj;)
{
	register int16_t prev, nobj, pobj;

	UNUSED(prev);
	pobj = LWGET(OB_HEAD(parent));
	if (pobj != obj)
	{
		while (TRUE)
		{
			nobj = LWGET(OB_NEXT(pobj));
			if (nobj != obj)
				pobj = nobj;
			else
				return (pobj);
		}
	} else
		return (NIL);
}


/***************************************************************************/
/*                                                                         */
/*                      COLOR ICON ROUTINES                                */
/*                                                                         */
/***************************************************************************/

#if COLORICON_SUPPORT

VOID convert_mask PROTO((int16_t *mask, int16_t *gl_mask, int16_t width, int16_t height));
CICON *fix_mono PROTO((CICONBLK *ptr, long *plane_size, int *tot_res));
CICON *fix_res PROTO((CICON *ptr, long mono_size, CICON ***next_res));
VOID fixup_cicon PROTO((CICON *ptr, int tot_icons, CICONBLK **carray));
VOID my_trans PROTO((int16_t *saddr, uint16_t swb, int16_t *daddr, uint16_t dwb, uint16_t h, uint16_t nplanes));
VOID trans_cicon PROTO((int tot_icons, CICONBLK **carray));
VOID tran_check PROTO((int16_t *saddr, int16_t *daddr, int16_t *mask, int w, int h, int nplanes));
int16_t get_rgb PROTO((int16_t index));


/*	Takes a list of icons and returns the first icon that 
 *	has the same number of planes.  Returns a null pointer if no match.
 */
CICON *match_planes(P(CICON *) iconlist, P(int) planes)
PP(CICON *iconlist;)
PP(int planes;)
{
	CICON *tempicon;

	tempicon = iconlist;
	while (tempicon)
	{
		if (tempicon->num_planes == planes)
			break;
		else
			tempicon = tempicon->next_res;
	}
	return (tempicon);
}

/*	Takes a list of icons and returns the first icon that 
 *	has equal or smaller planes.  Returns a null pointer if no match.
 */
CICON *find_eq_or_less(P(CICON *) iconlist, P(int) planes)
PP(CICON *iconlist;)
PP(int planes;)
{
	CICON *tempicon, *lasticon;

	tempicon = iconlist;
	lasticon = 0L;
	while (tempicon)
	{
		if (tempicon->num_planes == planes)
			break;
		else
		{
			if (tempicon->num_planes < planes)
			{
				if (!lasticon || (lasticon->num_planes < tempicon->num_planes))
					lasticon = tempicon;
			}
			tempicon = tempicon->next_res;
		}
	}
	if (tempicon)
		return (tempicon);
	else
		return (lasticon);
}


/*
 *	Routine to draw a color icon, which is a graphic image with a text
 *	string underneath it.  Note that this routine is very similar to
 *	gr_icon().   It has an extra parameter which is the list of color
 *	icons for different resolutions.
 */
VOID gr_cicon(P(int16_t) state, P(int16_t *) pmask, P(int16_t *)pdata, P(const char *) ptext, P(int16_t) ch, P(int16_t) chx, P(int16_t) chy, P(GRECT *) pi, P(GRECT *) pt, P(CICONBLK *) cicon)
PP(register int16_t state;)
PP(int16_t *pmask;)
PP(int16_t *pdata;)
PP(char *ptext;)
PP(register int16_t ch;)
PP(int16_t chx;)
PP(int16_t chy;)
PP(register GRECT *pi;)
PP(register GRECT *pt;)
PP(CICONBLK *cicon;)
{
	register int16_t fgcol, bgcol, tmp;
	/* crack the color/char definition word */
	CICON *color;
	int col_select;						/* is there a color select icon */
	int i, j;

	UNUSED(i);
	UNUSED(j);
	/* color = match_planes( cicon->mainlist, gl_nplanes ); */
	/* 8/31/92 DLF  Don't need this routine since mainlist is patched to
	 * contain the icon for this resolution.
	 */
	color = cicon->mainlist;

	fgcol = (ch >> 12) & 0x000f;
	bgcol = (ch >> 8) & 0x000f;
	ch &= 0x0ff;
	/* invert if selected   */
	if (state & SELECTED)
	{
		tmp = fgcol;
		fgcol = bgcol;
		bgcol = tmp;
	}

	col_select = 0;
	/* substitue mask if color avail */
	if (color)
	{
		if ((state & SELECTED) && color->sel_data)
		{
			col_select = 1;
			pdata = color->sel_data;
			pmask = color->sel_mask;
		} else
		{
			pdata = color->col_data;
			pmask = color->col_mask;
		}
		if (state & SELECTED)
		{
			tmp = fgcol;
			fgcol = bgcol;
			bgcol = tmp;
		}
	}

	/* do mask unless its on */
	/* a white background   */
	if (!((state & WHITEBAK) && (bgcol == WHITE)))
	{
		/* for pixel-packed mode, must blit in black (to zero out backgd) */
		if ((gl_nplanes == 16) && (color))
			gsx_blt(pmask, 0, 0, pi->g_w / 8, 0x0L, pi->g_x, pi->g_y,
					gl_width / 8, pi->g_w, pi->g_h, MD_TRANS, 1, fgcol);
		else
			gsx_blt(pmask, 0, 0, pi->g_w / 8, 0x0L, pi->g_x, pi->g_y,
					gl_width / 8, pi->g_w, pi->g_h, MD_TRANS, bgcol, fgcol);

		if (ptext[0])
		{								/* only print bar if string is not null 9/3/92 */
			if (color && (state & SELECTED))
				gr_rect(fgcol, IP_SOLID, pt);
			else
				gr_rect(bgcol, IP_SOLID, pt);
		}
	}

	/* draw the data */
	if (color)
	{
		/* NOTE: The call below is commented out because it does a transform form
		 * on the color data every time it is drawn.  The code in the color icon 
		 * resource load should do all of the transforms.  Uncomment this line for
		 * testing purposes only.
		 */
		/* my_trans( pdata, pi->g_w/8, pdata,pi->g_w/8,pi->g_h ); */

		gsx_cblt(pdata, 0, 0, pi->g_w / 8, 0x0L, pi->g_x, pi->g_y, gl_width / 8, pi->g_w, pi->g_h, S_OR_D,	/* 6/30/92 */
				 color->num_planes);
		if (state & SELECTED)
		{
			tmp = fgcol;
			fgcol = bgcol;
			bgcol = tmp;
			if (!col_select)
			{							/* check if we need to darken */
				convert_mask(pmask, gl_colmask, pi->g_w, pi->g_h);
				gsx_blt(gl_colmask, 0, 0, pi->g_w / 8, 0x0L, pi->g_x, pi->g_y,
						gl_width / 8, pi->g_w, pi->g_h, MD_TRANS, 1, 0);
			}
		}
	} else
	{
		gsx_blt(pdata, 0, 0, pi->g_w / 8, 0x0L, pi->g_x, pi->g_y,
				gl_width / 8, pi->g_w, pi->g_h, MD_TRANS, fgcol, bgcol);
	}
	
	/* draw the character   */
	gsx_attr(TRUE, MD_TRANS, fgcol);
	if (ch)
	{
		intin[0] = ch;
		gsx_tblt(SMALL, pi->g_x + chx, pi->g_y + chy, 1);
	}
	/* draw the label   */
	gr_gtext(TE_CNTR, SMALL, ptext, pt, MD_TRANS);
}


/*	Takes the blit code and makes sure that the number of planes is
 *	set.  This code was taken from gsx_blt() and modified so that the
 *	number of planes is passed in and the source and destination MFDB's
 *	had that value set correctly.  Otherwise, it is the same code.
 */
VOID gsx_cblt(P(int16_t *) saddr, P(uint16_t) sx, P(uint16_t) sy, P(uint16_t) swb, P(int16_t *) daddr, P(uint16_t) dx, P(uint16_t) dy, P(uint16_t) dwb, P(uint16_t) w, P(uint16_t) h, P(uint16_t) rule, P(int16_t) numplanes)
PP(int16_t *saddr;)
PP(register uint16_t sx;)
PP(register uint16_t sy;)
PP(register uint16_t swb;)
PP(int16_t *daddr;)
PP(register uint16_t dx;)
PP(register uint16_t dy;)
PP(uint16_t dwb;)
PP(uint16_t w;)
PP(uint16_t h;)
PP(uint16_t rule;)
PP(int16_t numplanes;)
{
	int16_t *ppts;

	ppts = &ptsin[0];

	gsx_fix(&gl_src, saddr, swb, h);
	gsx_fix(&gl_dst, daddr, dwb, h);

	gl_src.fd_nplanes = numplanes;
	gl_dst.fd_nplanes = numplanes;

	gsx_moff();

	ppts[0] = sx;
	ppts[1] = sy;
	ppts[2] = sx + w - 1;
	ppts[3] = sy + h - 1;
	ppts[4] = dx;
	ppts[5] = dy;
	ppts[6] = dx + w - 1;
	ppts[7] = dy + h - 1;
	avro_cpyfm(rule, &ppts[0], &gl_src, &gl_dst);
	gsx_mon();
}


/*	Takes a ptr to a mask and copies it to another while dithering
 *	the data.  Note that this does not check the limits of the gl_mask.
 */
VOID convert_mask(P(int16_t *) mask, P(int16_t *) dst_mask, P(int16_t) width, P(int16_t) height)
PP(int16_t *mask;)
PP(int16_t *dst_mask;)
PP(int16_t width;)
PP(int16_t height;)
{
	int i, j, wdwide, no_bytes;

	UNUSED(no_bytes);
	wdwide = width / 16;
	no_bytes = width / 8 * height;
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < wdwide; j++)
		{
			dst_mask[j + i * wdwide] = mask[j + i * wdwide] & 0x5555;
		}
		for (j = wdwide; j < 2 * wdwide; j++)
		{
			dst_mask[j + i * wdwide] = mask[j + i * wdwide] & 0xAAAA;
		}
	}
}


/* FIX_MONO()
 * Do fixups on the monochrome icon then pass back a ptr to the next part.
 */
CICON *fix_mono(P(CICONBLK *) ptr, P(long *) plane_size, P(int *) tot_res)
PP(CICONBLK *ptr;)
PP(long *plane_size;)
PP(int *tot_res;)
{
	VOIDPTR *temp;
	int16_t width, height;
	long size;

	width = ptr->monoblk.ib_wicon;
	height = ptr->monoblk.ib_hicon;
	temp = (VOIDPTR *)&ptr->mainlist;
	/* in the file, first link contains number of CICON structures */
	*tot_res = (int)(intptr_t)(*temp);
	/* BUG: works only for width being multiple of 16, should be rounded up */
	*plane_size = size = (long) ((width / 16) * height * 2);
	/* data follows CICONBLK structure */
	temp = (VOIDPTR *)&ptr->monoblk.ib_pdata;
	*temp = ptr + 1;
	/* mask follows data */
	temp = (VOIDPTR *)&ptr->monoblk.ib_pmask;
	*temp = (VOIDPTR)((intptr_t) (ptr + 1) + size);
	/* text follows mask */
	temp = (VOIDPTR *)&ptr->monoblk.ib_ptext;
	*temp = (VOIDPTR *)((intptr_t) (ptr + 1) + 2 * size);
	/* icon text for color icons is limited to 12 chars, next CICONBLK structure follows it */
	temp = (VOIDPTR *)(*temp) + 12L;
	return (CICON *)temp;
}


/* FIX_RES()
 * Does fixups on the resolution dependent color icons.  Returns
 * a pointer past the last icon fixed up.
 */
CICON *fix_res(P(CICON *) ptr, P(long) mono_size, P(CICON ***) next_res)
PP(CICON *ptr;)
PP(long mono_size;)
PP(CICON ***next_res;)
{
	VOIDPTR *temp;
	VOIDPTR end;
	int select;

	*next_res = &ptr->next_res;

	temp = (VOIDPTR *)&ptr->sel_data;
	/* BUG: sel_data is LONG offset to data, if it happens to be on a 256-byte boundary this will be cast to zero */
	select = (int)(intptr_t)(*temp);
	temp = (VOIDPTR *)&ptr->col_data;
	*temp = (VOIDPTR)((intptr_t) ptr + (intptr_t) sizeof(CICON));
	temp = (VOIDPTR *)&ptr->col_mask;
	*temp = (VOIDPTR)((intptr_t) ptr + (intptr_t) sizeof(CICON) + ((intptr_t) ptr->num_planes * mono_size));
	end = (char *)(*temp) + mono_size;			/* push pointer past the mask */
	if (select)
	{									/* there are some selected icons */
		temp = (VOIDPTR *)&ptr->sel_mask;
		*temp = end;
		temp = (VOIDPTR *)&ptr->sel_mask;			/* selected mask */
		end = (char *) end + ((intptr_t) ptr->num_planes * mono_size);
		*temp = end;
		end = (char *) end + mono_size;
	}
	return (CICON *)end;
}


/* FIXUP_CICON()
 * Does fixups on the pointers in the color icon structures.
 */
VOID fixup_cicon(P(CICON *) ptr, P(int) tot_icons, P(CICONBLK **) carray)
PP(CICON *ptr;)
PP(int tot_icons;)
PP(CICONBLK **carray;)
{
	int tot_resicons, tot_selicons;
	int i, j, k;
	long mono_size;						/* size of a single mono icon in bytes */
	CICON **next_res;
	CICONBLK *cicon;

	UNUSED(k);
	UNUSED(tot_selicons);
	for (i = 0; i < tot_icons; i++)
	{
		cicon = (CICONBLK *) ptr;
		carray[i] = cicon;
		ptr = fix_mono((CICONBLK *) ptr, &mono_size, &tot_resicons);
		if (tot_resicons)				/* 7/9/92 */
		{
#if !BINEXACT
			next_res = NULL; /* quiet compiler */
#endif
			cicon->mainlist = ptr;
			for (j = 0; j < tot_resicons; j++)
			{
				ptr = fix_res(ptr, mono_size, &next_res);
				*next_res = ptr;
			}
			*next_res = 0L;
		}
	}
}


/* GET_COLOR_RSC()
 * This is the main routine that fixes up the color icons.
 * Pass in a pointer to the array of CICONBLK's.  Count the number of 
 * entries (up until an entry has a -1L) to get the number of CICONBLK's.
 */

VOID get_color_rsc(P(CICONBLK **) cicondata)
PP(CICONBLK **cicondata;)
{
	CICON *ptr;
	CICONBLK **array_ptr;
	int totalicons;

	array_ptr = (CICONBLK **) cicondata;
	totalicons = 0;
	while (*array_ptr++ != (CICONBLK *)-1L)
		totalicons++;

	ptr = (CICON *) array_ptr;

	fixup_cicon(ptr, totalicons, (CICONBLK **) cicondata);	/* fixup pointers */
	trans_cicon(totalicons, (CICONBLK **) cicondata);	/* transform form */
}




/* MY_TRANS()
 * C code stolen from the apgsxif.s file.  This is call is identical to
 * gsx_trans except that gsx_trans is hardwired to one-plane transforms.
 * BUG FIX - pass in the number of planes instead of using the global
 *	dlf - 7/14/92
 */
VOID my_trans(P(int16_t *) saddr, P(uint16_t) swb, P(int16_t *) daddr, P(uint16_t) dwb, P(uint16_t) h, P(uint16_t) nplanes)
PP(int16_t *saddr;)
PP(uint16_t swb;)
PP(int16_t *daddr;)
PP(uint16_t dwb;)
PP(register uint16_t h;)
PP(uint16_t nplanes;)
{
	gsx_fix(&gl_src, saddr, swb, h);
	gl_src.fd_stand = TRUE;
	gl_src.fd_nplanes = nplanes;

	gsx_fix(&gl_dst, daddr, dwb, h);
	gl_dst.fd_stand = FALSE;			/* 7/23/92 */
	gl_dst.fd_nplanes = nplanes;
	avr_trnfm(&gl_src, &gl_dst);
}


/* TRANS_CICON()
 * Takes an array of color icons (and the number of icons), and changes
 * all of the color data from device-independent form to device-dependent.
 * 8/31/92 DLF - Changed this function so that if there is no match for
 * the current resolution, it will create one out of the next smallest icon.
 * In either case, the memory will be malloced so that we can avoid the
 * transform form in place.  Must deallocate at a later time.
 * The data is put in a buffer of the same size or larger (if no match for
 * current resolution).  
 */
VOID trans_cicon(P(int) tot_icons, P(CICONBLK **) carray)
PP(int tot_icons;)
PP(CICONBLK **carray;)
{
	int i;
	CICONBLK *ciconblk;
	CICON *cic, *ctemp;
	int w, h;
	int16_t *databuffer, *selbuffer, *tempbuffer;
	int32_t tot_size;

	UNUSED(cic);
	for (i = 0; i < tot_icons; i++)
	{
		ciconblk = carray[i];
		w = ciconblk->monoblk.ib_wicon;
		h = ciconblk->monoblk.ib_hicon;
		ctemp = find_eq_or_less(ciconblk->mainlist, gl_nplanes);

		ciconblk->mainlist = ctemp;
		if (ctemp)
		{
			tot_size = w / 8 * h * gl_nplanes;

			/* if not same size, allocate bigger buffers and expand */
			if (ctemp->num_planes != gl_nplanes)
			{
				tempbuffer = (int16_t *) Malloc(tot_size);
				databuffer = (int16_t *) Malloc(tot_size);
				if (!tempbuffer || !databuffer)
				{
					ciconblk->mainlist = 0L;
					return;				/* just quit */
				}
				expand_data(ctemp->col_data, tempbuffer, ctemp->col_mask, ctemp->num_planes, gl_nplanes, w, h);
				tran_check(tempbuffer, databuffer, ctemp->col_mask, w, h, gl_nplanes);
				ctemp->col_data = databuffer;
				if (ctemp->sel_data)
				{
					selbuffer = (int16_t *) Malloc(tot_size);
					if (selbuffer)
					{
						expand_data(ctemp->sel_data, tempbuffer, ctemp->sel_mask, ctemp->num_planes, gl_nplanes, w, h);
						tran_check(tempbuffer, selbuffer, ctemp->col_mask, w, h, gl_nplanes);
					}
					ctemp->sel_data = selbuffer;
				}
				Mfree(tempbuffer);
			} else
			{							/* just allocate same size, copy over because */
				/* we don't want to transform form in place */
				databuffer = (int16_t *) Malloc(tot_size);
				if (!databuffer)
				{
					ciconblk->mainlist = 0L;
					return;
				}
				my_trans(ctemp->col_data, w / 8, databuffer, w / 8, h, gl_nplanes);
				ctemp->col_data = databuffer;
				if (ctemp->sel_data)
				{
					selbuffer = (int16_t *) Malloc(tot_size);
					if (selbuffer)
					{
						my_trans(ctemp->sel_data, w / 8, selbuffer, w / 8, h, gl_nplanes);
					}
					ctemp->sel_data = selbuffer;
				}
			}

			ctemp->num_planes = gl_nplanes;
			ctemp->next_res = 0L;		/* won't need the rest of the list */
		}
	}
}


/* Frees extra memory allocated when fixing up the icons.  There should
 * be freeable memory for every icon, since fixup even mallocs memory
 * for non-expanded data (so that we avoid the transform form in place)
 */
VOID free_cicon(P(CICONBLK **) carray)
PP(CICONBLK **carray;)
{
	int i;
	CICONBLK *ciconblk, **ptr;
	CICON *ctemp;
	int16_t tot_icons;

	ptr = carray;
	tot_icons = 0;
	while (*ptr++ != (CICONBLK *)-1L)
		tot_icons++;

	for (i = 0; i < tot_icons; i++)
	{
		ciconblk = carray[i];
		ctemp = ciconblk->mainlist;
		if (ctemp)
		{
			Mfree(ctemp->col_data);
			if (ctemp->sel_data)
				Mfree(ctemp->sel_data);
		}
	}
}


/* does the transform form but also checks to see if this is pixel-packed
 * mode.  If pixel-packed, then after the transformation, the blacks and
 * whites must be swapped.  Also inverts mask.
 * dlf 9/1/92
 * Special Note for Pixel-Packed mode:  
 *	In pixel-packed mode, VDI no longer expects indexes as color data;
 *	data must be in 5-bit red, 5-bit green (plus one bit), and 5-bit
 *	blue.  So we must take the color indexes and convert them into
 *	this format.  After we expand data in this routine, we do a
 *	transform form on the data.  The result of the transform is that
 *	each word represents a pixel, and the value corresponds to a color
 *	index (0-255, but with reversed bits).  The rgb_tab has the correct
 *	RGB settings per index.  Therefore, we substitute indexes (i.e.
 *	reversed indexes) with RGB.
 */
VOID tran_check(P(int16_t *) saddr, P(int16_t *) daddr, P(int16_t *) mask, P(int) w, P(int) h, P(int) nplanes)
PP(int16_t *saddr;)
PP(int16_t *daddr;)
PP(int16_t *mask;)
PP(int w;)
PP(int h;)
PP(int nplanes;)
{
	int no_words, no_longs, i;
	int16_t *wptr;
	int32_t *lptr;

	UNUSED(lptr);
	UNUSED(no_longs);
	
	my_trans(saddr, w / 8, daddr, w / 8, h, nplanes);

	if (nplanes == 16)
	{
		no_words = w / 16 * h * nplanes;
		wptr = daddr;
		for (i = 0; i < no_words; i++)
		{
			switch (wptr[i])
			{
			case 0x0000:
				wptr[i] = 0xFFFF;
				break;
			case 0xFFFF:
				wptr[i] = 0x0000;
				break;
			default:
				wptr[i] = get_rgb(wptr[i]);
				break;
			}
		}
	}
}


int16_t get_rgb(P(int16_t) index)
PP(int16_t index;)
{
	int16_t rindex;						/* reversed bits of index */

	rindex = reverse(index);

	return rgb_tab[rindex];
}

#endif /* COLORICON_SUPPORT */


#ifdef NEVER
/* Takes saddr, the source address for color icon data with splanes of info,
 * and puts it into daddr, expanding it to dplanes.  It handles the expansion
 * cases.
 * DLF 8/31/92
 * Special Note for Pixel-packed mode:
 *	True-color behaves differently for blits.  The mask must be blitted
 *	in black because black is 0.  You can then OR the color data into
 *	black and preserve the colors.  Furthermore, you must make sure that
 *	outside of the mask, there are 0's.  In this routine, we must make
 *	the blacks be 1's (in the same way as 4 and 8 plane), but in
 *	tran_check(), 0x0000's are reversed for 0xffff and vice versa.
 * NOTE:  This has been optimized in  an assembly file.
 */
VOID expand_data(P(int16_t *) saddr, P(int16_t *) daddr, P(int16_t *) mask, P(int) splanes, P(int) dplanes, P(int) w, P(int) h)
PP(int16_t *saddr;)
PP(int16_t *daddr;)
PP(int16_t *mask;)
PP(int splanes;)
PP(int dplanes;)
PP(int w;)
PP(int h;)
{
	int plane_size, orig_size, end_size;
	register int i, n;
	register char *stemp, *dtemp;

	plane_size = w / 8 * h;
	orig_size = plane_size * splanes;
	end_size = plane_size * dplanes;
	for (i = 0; i < orig_size; i++)		/* copy source */
		daddr[i] = saddr[i];

	for (i = orig_size; i < end_size; i++)	/* fill with 1's */
		daddr[i] = 0xFF;
	/* now "and" splanes into the next plane */
	dtemp = &daddr[orig_size];
	for (n = 0; n < splanes; n++)
	{
		stemp = &saddr[n * plane_size];
		for (i = 0; i < plane_size; i++)
			dtemp[i] = stemp[i] & dtemp[i];
	}
	/* now copy plane into the rest of the planes */
	stemp = &daddr[orig_size];
	for (n = 0; n < (dplanes - splanes - 1); n++)
	{
		dtemp = &daddr[orig_size + ((n + 1) * plane_size)];
		for (i = 0; i < plane_size; i++)
			dtemp[i] = stemp[i];
	}
	/* Now "and" the mask into all planes */
	for (n = 0; n < dplanes; n++)
	{
		dtemp = &daddr[n * plane_size];
		/* We special-case pixel-packed because we need to make the outside
		 * of the mask all 1's, since later on in tran_check(), all 1's
		 * become 0's (which is black in pixel packed mode ).
		 */
		if (dplanes == 16)
			for (i = 0; i < plane_size; i++)
				dtemp[i] = ~mask[i] | dtemp[i];
		else
			for (i = 0; i < plane_size; i++)
				dtemp[i] = mask[i] & dtemp[i];
	}
}

#endif
