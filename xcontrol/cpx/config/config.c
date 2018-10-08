/* ======================================================================
 * FILE: CONFIG.C
 * ======================================================================
 * DATE: October 1, 1992  - Re-created from scratch
 *	 January 14, 1993 - Handle new 3D format
 */


/* INCLUDE FILES
 * ======================================================================
 */
#include "gemskel.h"
#include <string.h>
#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "country.h"
#include "config.h"
#include "config.rsh"
#include "xstructs.h"
#include "cpxdata.h"


/* DEFINES
 * ======================================================================
 */
#define TIME_DELAY	200, 0



/* PROTOTYPES
 * ======================================================================
 */
BOOLEAN cdecl cpx_call(GRECT * rect);

void open_vwork(void);
void close_vwork(void);
void XSelect(OBJECT * tree, int button);
void XDeselect(OBJECT * tree, int button);
void SetBlock(CPXNODE * curptr);
void myitoa(unsigned int inword, char *numbuf);
void XObjcDraw(OBJECT * tree, int obj);

void Do3D(void);
void MakeActivator(int obj);
void MakeIndicator(int obj);
void MakeOffsets(int obj);
void MakeIndex(int obj);
void MakeTed(int obj);


/* EXTERNALS
 * ======================================================================
 */


/* GLOBALS
 * ======================================================================
 */
XCPB *xcpb;								/* XControl Parameter Block   */
CPXINFO cpxinfo;						/* CPX Information Structure  */

OBJECT *tree;							/* Global tree variable...    */
OBJECT *ad_tree;


/* VDI arrays */
int contrl[12],
 intin[128],
 intout[128],
 ptsin[128],
 ptsout[128],
 work_in[12],
 work_out[57];
int pxyarray[10];						/* input point array */
int vhandle = -1;						/* virtual workstation handle */
int hcnt = 0;							/* handle count */
int numcol;
int currez;

/* AES variables */
int gl_apid,
 gl_hchar,
 gl_wchar,
 gl_hbox,
 gl_wbox;
GRECT desk;
int AES_Version;

char TempString[50];
CPXNODE *cur_ptr;
CPXNODE *head_ptr;
CPXNODE *tptr;
char TColorString[3];
char IColorString[3];
MFORM mouse;

#define ICOLSPEC_GET_DATACOL(color)   ( ((color) >> 12) & 0x0f )

#define ICOLSPEC_SET_DATACOL(color, datacol) color = ( ((color) & 0x0fff) | (((datacol) & 0x0f) << 12) )

#define COLSPEC_SET_TEXTCOL(color, textcol)         color = ( ((color) & 0xf0ff) | (((textcol)     & 0x0f) <<  8) )
#define COLSPEC_GET_TEXTCOL(color)     (((color) >>  8) & 0x0f)


#define IS3DOBJ OF_FL3DIND
#define IS3DACT OF_FL3DBAK
#ifndef OF_FL3DIND
#define OF_FL3DIND 0x200
#define OF_FL3DBAK 0x400
#endif

#ifndef CT_KEY
#define CT_KEY 53
#endif

/* FUNCTIONS
 * ======================================================================
 */


/* cpx_init()
 * ======================================================================
 * cpx_init() is where a CPX is first initialized.
 * There are TWO parts.
 *
 * PART ONE: cpx_init() is called when the CPX is first 'booted'. This is
 *	     where the CPX should read in its defaults and then set the
 *	     hardware based on those defaults.  
 *
 * PART TWO: The other time cpx_init() is called is when the CPX is being
 *	     executed.  This is where the resource is fixed up and current
 *	     cpx variables are updated from the hardware.  In addition,
 *	     this is where the CPXINFO structure should be filled out.
 *
 * IN:  XCPB	*Xcpb:	Pointer to the XControl Parameter Block
 * OUT: CPXINFO  *ptr:	Pointer to the CP Information Structure
 */
CPXINFO *cdecl cpx_init(XCPB * Xcpb)
{

	appl_init();

	xcpb = Xcpb;

	currez = Getrez();					/* find current resolution */
	open_vwork();
	close_vwork();
	numcol = work_out[13];				/* size of CLUT */

	if (xcpb->booting)
	{
		head_ptr = cur_ptr = (*xcpb->Get_Head_Node) ();
		do
		{
			if ((cur_ptr) && (cur_ptr->vacant))
			{
				cur_ptr->cpxhead.flags.was_resident = 0;
				if (cur_ptr->cpxhead.flags.resident)
					cur_ptr->cpxhead.flags.was_resident = 1;
			}
			cur_ptr = cur_ptr->next;
		} while ((cur_ptr) && (cur_ptr->vacant));

		return ((CPXINFO *) TRUE);
	} else
	{
		appl_init();
		AES_Version = _GemParBlk.global[0];

		if (!xcpb->SkipRshFix)
		{
			(*xcpb->rsh_fix) (NUM_OBS, NUM_FRSTR, NUM_FRIMG, NUM_TREE,
							  rs_object, rs_tedinfo, rs_strings, rs_iconblk,
							  rs_bitblk, rs_frstr, rs_frimg, rs_trindex, rs_imdope);
		}

		ad_tree = (OBJECT *) rs_trindex[CONFIG];

		/* Initialize the CPXINFO structure */
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

		/* Convert to 3D if possible - ONLY if AES 0x0330 or Greater!
		 * AND if # of pens is greater than LWHITE
		 */
		if (!xcpb->SkipRshFix)
		{
			if ((AES_Version >= 0x0330) && (numcol > LWHITE))
			{
				Do3D();
			}
		}


		/* Return the pointer to the CPXINFO structure to XCONTROL */
		return (&cpxinfo);
	}
}




/* cpx_call()
 * ======================================================================
 * Called ONLY when the CPX is being executed. Note that it is CPX_INIT()
 * that returned the ptr to cpx_call().
 * CPX_CALL() is the entry point to displaying and manipulating the
 * dialog box.
 *
 * IN: GRECT *rect:	Ptr to a GRECT that describes the current work
 *			area of the XControl window.
 *
 * OUT:
 *   FALSE:     The CPX has exited and no more messages are
 *		needed.  XControl will either return to its
 *		main menu or close its windows.
 *		This is used by XForm_do() type CPXs.
 *
 *   TRUE:	The CPX requests that XCONTROL continue to
 *		send AES messages.  This is used by Call-type CPXs.
 */
BOOLEAN cdecl cpx_call(GRECT * rect)
{
	int button;
	int quit = 0;
	WORD msg[8];
	int num;
	MRETS mk;

	ActiveTree(ad_tree);

	ObX(ROOT) = rect->g_x;
	ObY(ROOT) = rect->g_y;

	head_ptr = cur_ptr = (*xcpb->Get_Head_Node) ();
	SetBlock(cur_ptr);

	Objc_draw(tree, ROOT, MAX_DEPTH, NULL);

	do
	{
		button = (*xcpb->Xform_do) (tree, 0, msg);

		if ((button != -1) && (button & 0x8000))
			button &= 0x7fff;

		switch (button)
		{
		case OK:
			strcpy(cur_ptr->cpxhead.title_txt, TempString);
			cur_ptr->cpxhead.i_color = IconColor(ICON);
			cur_ptr->cpxhead.t_color = TedColor(TEXT);

		case CANCEL:
			quit = TRUE;
			Deselect(button);
			break;

		case SAVE:						/* save current values */
			if ((*xcpb->XGen_Alert) (SAVE_DEFAULTS) == TRUE)
			{
#if 0
				unsigned int temp;

				strcpy(cur_ptr->cpxhead.title_txt, TempString);
				cur_ptr->cpxhead.i_color = IconColor(ICON);
				cur_ptr->cpxhead.t_color = TedColor(TEXT);

				temp = cur_ptr->cpxhead.flags;
				cur_ptr->cpxhead.flags &= 0x7FFB;
				temp &= 0x7FFF;

				if (IsSelected(YES1))
				{
					cur_ptr->cpxhead.flags |= 0x0004;
					temp |= 0x8000;
				}
				(*xcpb->Save_Header) (cur_ptr);
				cur_ptr->cpxhead.flags = temp;
#else
				unsigned int resident, was_resident;
				
				strcpy(cur_ptr->cpxhead.title_txt, TempString);
				cur_ptr->cpxhead.i_color = IconColor(ICON);
				cur_ptr->cpxhead.t_color = TedColor(TEXT);

				resident = cur_ptr->cpxhead.flags.resident;
				was_resident = cur_ptr->cpxhead.flags.was_resident;
				cur_ptr->cpxhead.flags.resident = 0;
				cur_ptr->cpxhead.flags.was_resident = 0;
				was_resident = 0;

				if (IsSelected(YES1))
				{
					cur_ptr->cpxhead.flags.resident = 1;
					was_resident = 1;
				}
				(*xcpb->Save_Header) (cur_ptr);
				cur_ptr->cpxhead.flags.was_resident = was_resident;
				cur_ptr->cpxhead.flags.resident = resident;
#endif
			}
			XDeselect(tree, button);
			break;

		case LEFT:
			do
			{
				tptr = head_ptr;
				if (tptr != cur_ptr)
				{
					while (tptr && (tptr->next->vacant) && (tptr->next != cur_ptr))
						tptr = tptr->next;

					if (tptr && (tptr->next->vacant) && (tptr->next == cur_ptr))
					{
						if (!IsSelected(LEFT))
							XSelect(tree, LEFT);
						cur_ptr = tptr;
						SetBlock(cur_ptr);
						Objc_draw(tree, BASE, MAX_DEPTH, NULL);
						XObjcDraw(tree, TEXTNUM);
						XObjcDraw(tree, ICONNUM);
						XObjcDraw(tree, YES1);
						XObjcDraw(tree, NO1);
					}
				}
				Graf_mkstate(&mk);

				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(LEFT))
				XDeselect(tree, LEFT);
			break;

		case RIGHT:
			do
			{
				if ((cur_ptr->next) && (cur_ptr->next->vacant))
				{
					if (!IsSelected(RIGHT))
						XSelect(tree, RIGHT);
					cur_ptr = cur_ptr->next;
					SetBlock(cur_ptr);
					Objc_draw(tree, BASE, MAX_DEPTH, NULL);
					XObjcDraw(tree, TEXTNUM);
					XObjcDraw(tree, ICONNUM);
					XObjcDraw(tree, YES1);
					XObjcDraw(tree, NO1);
				}
				Graf_mkstate(&mk);
				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(RIGHT))
				XDeselect(tree, RIGHT);
			break;


		case TEXTLFT:
			do
			{
				num = COLSPEC_GET_TEXTCOL(TedColor(TEXT));
				if (num > 0)
				{
					if (!IsSelected(TEXTLFT))
						XSelect(tree, TEXTLFT);
					num--;
					COLSPEC_SET_TEXTCOL(TedColor(TEXT), num);
					myitoa(num, &TColorString[0]);
					TedText(TEXTNUM) = TColorString;
					XObjcDraw(tree, TEXTNUM);
					Objc_draw(tree, TEXT, MAX_DEPTH, NULL);
				}
				Graf_mkstate(&mk);
				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(TEXTLFT))
				XDeselect(tree, TEXTLFT);
			break;


		case TEXTRT:
			do
			{
				num = COLSPEC_GET_TEXTCOL(TedColor(TEXT));
				if (num < 15)
				{
					if (!IsSelected(TEXTRT))
						XSelect(tree, TEXTRT);
					num++;
					COLSPEC_SET_TEXTCOL(TedColor(TEXT), num);
					myitoa(num, &TColorString[0]);
					TedText(TEXTNUM) = TColorString;
					XObjcDraw(tree, TEXTNUM);
					Objc_draw(tree, TEXT, MAX_DEPTH, NULL);
				}
				Graf_mkstate(&mk);
				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(TEXTRT))
				XDeselect(tree, TEXTRT);
			break;

		case ICONLFT:
			do
			{
				num = ICOLSPEC_GET_DATACOL(IconColor(ICON));
				if (num > 0)
				{
					if (!IsSelected(ICONLFT))
						XSelect(tree, ICONLFT);
					num--;
					ICOLSPEC_SET_DATACOL(IconColor(ICON), num);
					myitoa(num, &IColorString[0]);
					TedText(ICONNUM) = IColorString;
					XObjcDraw(tree, ICONNUM);
					Objc_draw(tree, ICON, MAX_DEPTH, NULL);
				}
				Graf_mkstate(&mk);
				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(ICONLFT))
				XDeselect(tree, ICONLFT);
			break;

		case ICONRT:
			do
			{
				num = ICOLSPEC_GET_DATACOL(IconColor(ICON));
				if (num < 15)
				{
					if (!IsSelected(ICONRT))
						XSelect(tree, ICONRT);
					num++;
					ICOLSPEC_SET_DATACOL(IconColor(ICON), num);
					myitoa(num, &IColorString[0]);
					TedText(ICONNUM) = IColorString;
					XObjcDraw(tree, ICONNUM);
					Objc_draw(tree, ICON, MAX_DEPTH, NULL);
				}
				Graf_mkstate(&mk);
				if (mk.buttons)
					evnt_timer(TIME_DELAY);
			} while (mk.buttons);

			if (IsSelected(ICONRT))
				XDeselect(tree, ICONRT);
			break;

		default:
			if (button == -1)
			{
				switch (msg[0])
				{
				case WM_REDRAW:		/* redraw the cpx */
					break;

				case AC_CLOSE:			/* treated like a cancel */
				case WM_CLOSED:
					quit = TRUE;		/* treated like an OK */
					break;

				case CT_KEY:
					break;
				default:
					break;
				}
			}
			break;
		}
	} while (!quit);
	return (FALSE);
}



/*
 * Open virtual workstation
 */
void open_vwork(void)
{
	int i;

	if (hcnt == 0)
	{
		for (i = 1; i < 10;)
			work_in[i++] = 1;
		work_in[0] = currez + 2;
		work_in[10] = 2;
		vhandle = xcpb->handle;
		v_opnvwk(work_in, &vhandle, work_out);
	}
	hcnt++;
}


/*
 * Close virtual workstation
 */
void close_vwork(void)
{
	hcnt--;
	if (!hcnt)
	{
		v_clsvwk(vhandle);
		vhandle = -1;
	}
}



/*	
 * Convert binary number to ascii value
 */
void myitoa(unsigned int inword, char *numbuf)
{
	unsigned int temp1,
	 value;
	register int i,
	 j;
	char tmpbuf[10];
	register char *ascbuf;

	ascbuf = numbuf;
	i = 0;								/* if the value is non zero  */

	if (!inword)
		*ascbuf++ = '0';
	else
	{
		value = inword;
		while (value)
		{
			temp1 = value % 10;			/*  find the remainder  */
			temp1 += 0x0030;			/*  convert to ASCII    */
			tmpbuf[i++] = temp1;		/*  buffer is reverse   */
			value = value / 10;
		}

		for (j = i - 1; j >= 0; j--)	/* reverse it back  */
			*ascbuf++ = tmpbuf[j];
	}

	*ascbuf = 0;						/* end of string mark   */
	return;
}


void XDeselect(OBJECT * tree, int button)
{
	GRECT rect;

	Deselect(button);
	rect = ObRect(button);
	objc_offset(tree, button, &rect.g_x, &rect.g_y);
	rect.g_x -= 2;
	rect.g_y -= 2;
	rect.g_w += 4;
	rect.g_h += 4;
	Objc_draw(tree, button, MAX_DEPTH, &rect);
}

void XSelect(OBJECT * tree, int button)
{
	GRECT rect;

	Select(button);
	rect = ObRect(button);
	objc_offset(tree, button, &rect.g_x, &rect.g_y);
	rect.g_x -= 2;
	rect.g_y -= 2;
	rect.g_w += 4;
	rect.g_h += 4;
	Objc_draw(tree, button, MAX_DEPTH, &rect);
}



void SetBlock(CPXNODE * curptr)
{
	unsigned int num;

	strcpy(TempString, curptr->cpxhead.title_txt);
	TedText(TEXT) = TempString;
	TedColor(TEXT) = curptr->cpxhead.t_color;
	IconWidth(ICON) = 32;
	IconHeight(ICON) = 24;
	IconData(ICON) = (int *)curptr->cpxhead.sm_icon;
	IconText(ICON) = curptr->cpxhead.i_text;
	IconColor(ICON) = curptr->cpxhead.i_color;

	num = COLSPEC_GET_TEXTCOL(TedColor(TEXT));
	myitoa(num, &TColorString[0]);
	TedText(TEXTNUM) = TColorString;

	num = ICOLSPEC_GET_DATACOL(IconColor(ICON));
	myitoa(num, &IColorString[0]);
	TedText(ICONNUM) = IColorString;

	Deselect(YES1);
	Deselect(NO1);
	if (curptr->cpxhead.flags.was_resident)
		Select(YES1);
	else
		Select(NO1);
}


void XObjcDraw(OBJECT * tree, int obj)
{
	GRECT rect;

	rect = ObRect(obj);
	objc_offset(tree, obj, &rect.g_x, &rect.g_y);
	rect.g_x -= 2;
	rect.g_y -= 2;
	rect.g_w += 4;
	rect.g_h += 4;
	Objc_draw(tree, obj, MAX_DEPTH, &rect);
}



void Do3D(void)
{
	ActiveTree(ad_tree);
	MakeActivator(SAVE);
	MakeActivator(OK);
	MakeActivator(CANCEL);

	MakeOffsets(SAVE);
	MakeOffsets(OK);
	MakeOffsets(CANCEL);

	MakeActivator(LEFT);
	MakeIndex(LEFT);

	MakeActivator(RIGHT);
	MakeIndex(RIGHT);

	MakeOffsets(LEFT);
	MakeOffsets(RIGHT);

	MakeActivator(TEXTNUM);
	MakeTed(TEXTNUM);

	MakeActivator(TEXTLFT);
	MakeIndex(TEXTLFT);

	MakeActivator(TEXTRT);
	MakeIndex(TEXTRT);

	MakeActivator(ICONNUM);
	MakeTed(ICONNUM);

	MakeActivator(ICONLFT);
	MakeIndex(ICONLFT);

	MakeActivator(ICONRT);
	MakeIndex(ICONRT);

	MakeIndicator(YES1);
	MakeIndicator(NO1);

	MakeOffsets(TEXTLFT);
	MakeOffsets(TEXTRT);
	MakeOffsets(TEXTNUM);
	MakeOffsets(ICONLFT);
	MakeOffsets(ICONRT);
	MakeOffsets(ICONNUM);
}



void MakeActivator(int obj)
{
	ObFlags(obj) |= IS3DOBJ;
	ObFlags(obj) |= IS3DACT;
}


void MakeIndicator(int obj)
{
	ObFlags(obj) |= IS3DOBJ;
	ObFlags(obj) &= ~IS3DACT;
}


void MakeOffsets(int obj)
{
	ObX(obj) += 2;
	ObY(obj) += 2;
	ObW(obj) -= 4;
	ObH(obj) -= 4;
}



void MakeIndex(int obj)
{
	ObIndex(obj) = ((ObIndex(obj) & 0xffffff70L) | LWHITE | 0x70L);
}



void MakeTed(int obj)
{
	TedColor(obj) = (TedColor(obj) & 0xFF70) | LWHITE | 0x70;
}
