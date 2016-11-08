/*
 *************************************************************************
 *			Revision Control System
 * =======================================================================
 *  $Author: kbad $	$Date: 89/07/28 13:04:59 $
 * =======================================================================
 *
 * Revision 2.4  89/07/28  13:04:59  kbad
 * Added accs_init(), but there are still some strange interactions with
 * the critical error handler, so it is bracketed by #ifdef ACC_DELAY.
 * Also changed the desktop backdrop pattern to solid in color modes here,
 * before gemshlib:sh_main() so that the autoboot application gets the
 * right pattern drawn.  It still gets set in sh_main as well.
 * 
 * Revision 2.3  89/07/27  03:21:49  kbad
 * gsx_malloc: min gl_mlen for TT = 0xc800L
 * 
 * Revision 2.2  89/04/26  18:24:25  mui
 * TT
 * 
 * Revision 2.1  89/02/22  05:27:35  kbad
 * *** TOS 1.4  FINAL RELEASE VERSION ***
 * 
 * Revision 1.15  89/01/24  07:18:57  kbad
 * Change to directory of autoboot app
 * 
 * Revision 1.14  88/10/24  12:27:53  mui
 * Save 1/4 of the screen memory - added gsx_malloc
 * 
 * Revision 1.12  88/10/17  13:36:55  kbad
 * Yanked fs_start to fix fs_input growing clip rect problem
 * 
 * Revision 1.11  88/09/22  04:49:42  kbad
 * Moved rsrc_gaddr of desktop to before wm_start
 * 
 * Revision 1.10  88/09/08  18:57:55  kbad
 * added <flavor.h> for DOWARNING flag
 * 
 * Revision 1.9  88/08/08  18:56:56  mui
 * zero out the dispatcher list before bringing up system alert
 * 
 * Revision 1.8  88/08/08  12:32:38  mui
 * don't wait for accessories to finish, they only get one chance to run
 * 
 * Revision 1.7  88/08/05  16:38:52  mui
 * add escape button
 * 
 * Revision 1.6  88/08/04  19:29:19  mui
 * at autoboot, wait until all the acc finish their work before we go
 * into the auto boot sequence
 * 
 * Revision 1.4  88/08/02  13:03:15  kbad
 * changed dowarning box to match build date of 1.04d 8/1/88
 * 
 * Revision 1.3  88/07/28  17:29:06  mui
 * Nothing
 * 
 * Revision 1.2  88/07/15  16:08:31  mui
 * add warning message
 * 
 * Revision 1.1  88/06/02  12:34:14  lozben
 * Initial revision
 * 
 *************************************************************************
 */
/*      GEMINIT.C       4/23/84 - 02/08/85      Lee Lorenzen            */
/*      to 68k          2/23/75 - 05/03/85      Lowell Webster          */
/*      Ram based                 05/15/85      Derek Mui               */
/*      Fix the main              07/11/85      Derek Mui               */
/*      08/28/85  Fix when no disk attached             Derek Mui       */
/*      10/23/85  Fix at pred_inf to read inf file from root  D.Mui     */
/*      05/22/85  Initialize in dispatch semaphore      M Schmal        */
/*      Fix at pred_dinf for bit on     2/12/87         Derek Mui       */
/*      Change at main to get mouse add 11/30/87        D.Mui           */
/*      Update          12/10/87                        D.Mui           */
/*      Change at ini_dlong     12/17/87                D.Mui           */
/*      Added ad_out1, ad_out2  1/7/88                  D.Mui           */
/*      Added er_num, no_aes    1/8/88                  D.Mui           */
/*      Move drawstk from deskglob to here at main 1/15/88      D.Mui   */
/*      Add ad_fsel             1/21/88                 D.Mui           */
/*      Take out ad_out1 and ad_out2    1/27/88         D.Mui           */
/*      Change pred_dinf                1/28/88         D.Mui           */
/*      Take out ad_envrn               1/28/88         D.Mui           */
/*      Add autoexec                    2/11/88         D.Mui           */
/*      Add g_autoboot  2/11/88                         D.Mui           */
/*      Change the resolution masking   2/23/88         D.Mui           */
/*      Check to turn bit on when autoboot      3/10/88 D.Mui           */
/*      In line coded of ini_dlongs     3/24/88         D.Mui           */
/*      Fix at pred_dinf for scan_2     5/2/88          D.Mui           */
/*      Change at pref_dinf for gl_restype 4/25/89      D.Mui           */
/*      Change at gsx_malloc for new size  7/26/89      D.Mui           */
/*      Turn on the cache in pred_dinf          6/25/90 D.Mui           */
/*      Make sure the scan_2 for cache checking doesn't reset temp      */
/*      Look for newdesk.inf then desktop.inf in pred_dinf 6/26/90      */
/*      Change RES4 to RES5 for solid pattern   6/27/90 D.Mui           */
/*      Changed wm_start to wm_init             900628 kbad             */
/*      Moved all_run to gemcli.c               07/16/90        D.Mui   */
/*      Fix at gsx_malloc to save 1/2 of the screen     7/19/90 D.Mui   */
/*      Added initialization of crt_error semaphore     900731  D.Mui   */
/*      Removed #include flavor.h: DOWARNING now here 900801 kbad       */
/*      Also removed TT conditional.  If it's longframe, it's TT        */
/*      Fixed the background pattern in TT high 11/14/90        D.Mui   */
/*      Init the current mouse form             5/8/91          D.Mui   */
/*      Check control key for nodisk system at main 8/13/91     D.Mui   */
/*      Exit graphic mode before closing work station 12/20/91          */
/*      Change at gsx_malloc for saving 25 columns of screen 7/7/92 Mui */
/*      Add 3D stuff                            7/7/92          D.Mui   */
/*      Add code to do sparrow res change       7/17/92         D.Mui   */
/*      Add new variables                       8/1/92          D.Mui   */

/*
 *       Copyright 1999, Caldera Thin Clients, Inc.                      
 *       This software is licenced under the GNU Public License.         
 *       Please see LICENSE.TXT for further information.                 
 *                                                                       
 *                  Historical Copyright
 *      -------------------------------------------------------------
 *      GEM Application Environment Services              Version 1.0
 *      Serial No.  XXXX-0000-654321              All Rights Reserved
 *      Copyright (C) 1985                      Digital Research Inc.
 *      -------------------------------------------------------------
 */

#include "aes.h"
#include "gemlib.h"
#include "taddr.h"
#include "gsxdefs.h"
#include "gemrsc.h"


#define CACHE_ON	0x00003919L
#define CACHE_OFF	0x00000808L
#define LONGFRAME	*(int16_t *)(0x59eL)


#if AESVERSION >= 0x330
BOOLEAN do_once;
#endif
#if TOSVERSION >= 0x400
int32_t gl_vdo;
#endif
STATIC EVB evx;						/* used only to get elinkoff offset */
intptr_t ad_sysglo;
VOIDPTR ad_armice;
VOIDPTR ad_hgmice;
LPTREE ad_stdesk;
char *ad_fsel;
intptr_t drawstk;
int16_t er_num;						/* for output.s */
int16_t no_aes;						/* gembind.s    */
/* BOOLEAN sh_up; */				/* is the sh_start being ran yet ? */ /* unused */
BOOLEAN autoexec;					/* autoexec a file ?    */
STATIC char g_autoboot[CMDLEN];
STATIC int16_t g_flag;
BOOLEAN ctldown;					/* ctrl key down ? */ /* referenced by DESKTOP */
/* 8/1/92 */
#if AES3D
BOOLEAN act3dtxt;					/* look of 3D activator text */
BOOLEAN act3dface;					/* selected look of 3D activator */
BOOLEAN ind3dtxt;					/* look of 3D indicator text */
BOOLEAN ind3dface;					/* selected look of 3D indicators */
uint16_t gl_indbutcol;				/* indicator button color */
uint16_t gl_actbutcol;				/* activator button color */
uint16_t gl_alrtcol;				/* alert background color */
#endif
int16_t crt_error;					/* critical error handler semaphore     */
				/* set in jbind.s, checked by dispatcher    */
#if (AESVERSION >= 0x330) | !BINEXACT
int16_t adeskp[3];					/* desktop colors & backgrounds */
STATIC int16_t awinp[3];			/* window colors & backgrounds */ /* not used anymore?? */
#endif
#if TOSVERSION >= 0x400
uint16_t d_rezword;					/* default resolution for sparrow */
#endif


#define Getrez() trp14(4)
#define Blitmode(on) trp14(64, on)
#define VcheckMode(mode) trp14(95, mode)

#define Cconws(x) trap(9, x)

STATIC char autopath[CMDLEN];

#if AESVERSION >= 0x320
STATIC MFORM gl_cmform;				/* current aes mouse form   */
STATIC MFORM gl_omform;				/* old aes mouse form       */
#endif

#if DOWARNING
BOOLEAN dowarn;
#endif

#define Kbshift(a) bios(11, a)


#ifdef ACC_DELAY

VOID accs_init PROTO((NOTHING));

/* The AES startup code should wait for all accessories to finish initializing
 * before it starts the first main process running.  To determine that all
 * accessories have initialized, we will wait until every accessory is blocked
 * waiting for a message event.
 *
 * A process with a message block can be identified by walking the PD's EVB
 * list, and finding an EVB that is not an EVDELAY and is not pointed to
 * by anything in the PD's CDA.  This EVB is a message EVB by process of
 * elimination.
 *
 * Thus, we can loop until all accessories have a blocked waiting for a
 * message event with the following code:
 */
VOID accs_init(NOTHING)
{
	PD *pd;
	register EVB *e, *ce;
	register CDA *c;
	BOOLEAN notdone;
	register int16_t pid;

	sh_tographic();
	do
	{
		/* initialize to DONE, change if we find one with no message block */
		notdone = FALSE;
		/* let everybody run */
		all_run();

		/* look at all accessory EVB lists (ctlmgr==0, screenmgr==1) */
		pid = 2;
		pd = fpdnm(NULL, pid);
		while (pd)
		{
			c = pd->p_cda;
			/* walk through EVB list looking for CDA matches */
			for (ce = -1L, e = pd->p_evlist; ce && e; e = e->e_nextp)
			{
				/* timer wait? */
				if (e->e_flag & EVDELAY)
					continue;
				/* input block? */
				for (ce = c->c_iiowait; ce && ce != e; ce = ce->e_link) ;
				if (ce == e)
					continue;
				/* mouse rect? */
				for (ce = c->c_msleep; ce && ce != e; ce = ce->e_link) ;
				if (ce == e)
					continue;
				/* button wait? */
				for (ce = c->c_bsleep; ce && ce != e; ce = ce->e_link) ;
				/* if ce is null here, we found a message EVB */
			}
			if (ce)
			{
				/* found one w/ no message blocks, or w/ NO event blocks */
				/* fall out and let 'em run */
				notdone = TRUE;
				pd = NULL;
			} else
			{
				/* this guy had a message block, try the next pd */
				pd = fpdnm(NULL, ++pid);
			}
		}

	} while (notdone);
}
#endif	/* ACC_DELAY */


#if TOSVERSION >= 0x400
/*
 * Set the resolution
 */
VOID setres(NOTHING)
{
	int mode;

	UNUSED(mode);
	if ((gl_vdo & 0x30000L) == 0x30000L)
	{
		intin[0] = 5;
		d_rezword = VcheckMode(d_rezword);
		/* ptsout[0] = d_rezword; */
		gl_ws.ws_pts0 = d_rezword;
	}
}
#endif


/* 306de: 00e1dca8 */
VOID gem_main(NOTHING)
{
	register int16_t i;
	VOIDPTR tmpadbi;
	LPTREE tree;
	register THEGLO *DGLO;
	PD *lslr;
	register char *apath;
	DGLO = &D;

	UNUSED(lslr);
	
	er_num = ALRT04CRT;					/* output.s */ /* but not used there... */
	no_aes = ALRTNOFUNC;				/* for gembind.s */ /* but not used there... */
	
	/****************************************/
	/*      ini_dlongs();                   */
	/****************************************/

	ad_shcmd = &DGLO->s_cmd[0];
	ad_shtail = &DGLO->s_tail[0];
	ad_path = &DGLO->g_dir[0];

	/* init. long pointer to global array   */
	/* which is used by resource calls  */

	ad_sysglo = (intptr_t)&DGLO->g_sysglo[0];
	ad_windspb = (intptr_t)&wind_spb;

	/****************************************/

	/*
	 * no check for allocation faiure here...
	 * why not allocate that static? its never changed
	 */
	drawstk = (intptr_t)dos_alloc(0x00000400L);	/* draw stack is 1K */
	drawstk += 0x00000400L;			/* init to top */

	/* no ticks during init */
	hcli();

	/* take the 0efh int. ...erm trap #2  */
	takecpm();

	/* init event recorder  */
	gl_recd = FALSE;
	gl_rlen = 0;
	gl_rbuf = 0;

	/* initialize pointers to heads of event list and thread list       */
	elinkoff = (intptr_t)(char *) &evx.e_link - (intptr_t)(char *) &evx;

	/* link up all the evb's to the event unused list */
	eul = 0;
	for (i = 0; i < NUM_EVBS; i++)
	{
		DGLO->g_evb[i].e_nextp = eul;
		eul = &DGLO->g_evb[i];
	}
	/* initialize list and unused lists   */

	drl = 0;
	nrl = 0;
	zlr = 0;
	dlr = 0;
	fpcnt = 0;
	fpt = 0;
	fph = 0;
	infork = 0;
	crt_error = FALSE;

	/* initialize sync blocks */
	wind_spb.sy_tas = 0;
	wind_spb.sy_owner = 0;
	wind_spb.sy_wait = 0;

	gl_btrue = 0x0;
	gl_bdesired = 0x0;
	gl_bdelay = 0x0;
	gl_bclick = 0x0;

	/* init initial process */
	for (i = 0; i < NUM_PDS; i++)
		pinit(&DGLO->g_pd[i], &DGLO->g_cda[i]);

	DGLO->g_pd[0].p_uda = &DGLO->g_uda;
	DGLO->g_pd[1].p_uda = (UDA *)&DGLO->g_2uda;
	DGLO->g_pd[2].p_uda = (UDA *)&DGLO->g_3uda;
	
	/* if not rlr then initialize his stack pointer */
	DGLO->g_2uda.u_spsuper = &DGLO->g_2uda.u_supstk;
	DGLO->g_3uda.u_spsuper = &DGLO->g_3uda.u_supstk;

	curpid = 0;

	rlr = &DGLO->g_pd[curpid];
	rlr->p_pid = curpid++;
	rlr->p_link = 0;
	rlr->p_stat = PS_RUN;
	cda = rlr->p_cda;

	/* restart the tick */
	hsti();

	/* 
	 * screen manager process init.
	 * This process starts out owning the mouse
	 * and the keyboard. it has a pid == 1
	 */
	gl_mowner = gl_kowner = ctl_pd = ictlmgr(rlr->p_pid);

	/* New stuff 8/13/91    */
#if BINEXACT
	/*
	 * Bug: Kbshift() called without mode parameter
	 * The outcome of this is that the lower 16 bits
	 * of some random memory address are passed as parameter
	 */
	ctldown = bios(11) & 0x0004 ? TRUE : FALSE;
#else
	ctldown = Kbshift(-1) & 0x0004 ? TRUE : FALSE;
#endif

	rsc_read();							/* read in resource */

	if (!ctldown)
		ldaccs();						/* load in accessories  */

	pred_dinf();						/* pre read the inf */

	/* get the resolution and the auto boot name         */

	/* load gem resource and fix it up before we go   */

	/* load all desk acc's  */

	/* init button stuff    */
	set_defdrv();						/* set default drive    */

	/* do gsx open work station */
	gsx_init();

	/* 8/1/92   */
	/* July 30 1992 - ml. Init 3D-look of indicators and activators */
#if AES3D
	act3dtxt = TRUE;					/* don't move text for activators */
	act3dface = FALSE;					/* no color change when activator is selected */
	ind3dtxt = FALSE;					/* move text for indicators */
	ind3dface = TRUE;					/* change color when indicator is selected */

	if (gl_ws.ws_ncolors <= LWHITE)
	{									/* init button color */
		gl_actbutcol = gl_indbutcol = WHITE;
		gl_alrtcol = WHITE;				/* init alert background color */
	} else
	{
		gl_actbutcol = gl_indbutcol = LWHITE;
		gl_alrtcol = LWHITE;			/* init alert background color */
	}
#endif

	rom_ram(0, ad_sysglo);

	rs_gaddr(ad_sysglo, R_BIPDATA, MICE0, &ad_armice);
	ad_armice = (VOIDPTR)LLGET((intptr_t)ad_armice);
	rs_gaddr(ad_sysglo, R_BIPDATA, MICE2, &ad_hgmice);
	ad_hgmice = (VOIDPTR)LLGET((intptr_t)ad_hgmice);

#if AESVERSION >= 0x320
	gl_cmform = *((MFORM *) ad_hgmice);
#endif
	/* fix up icons */
	for (i = 0; i < 3; i++)
	{
#if !BINEXACT
		BITBLK bi;
#endif
		rs_gaddr(ad_sysglo, R_BITBLK, i, &tmpadbi);
		LBCOPY(ADDR(&bi), tmpadbi, sizeof(BITBLK));
#if !AES3D
		gsx_trans(bi.bi_pdata, bi.bi_wb, bi.bi_pdata, bi.bi_wb, bi.bi_hl);
#endif
	}
	
	/* take the critical err handler int. */
	cli();
	takeerr();
	sti();

	sh_tographic();						/* go into graphic mode */
	sh_gem = TRUE;

	/* take the tick int. */
	cli();
	gl_ticktime = gsx_tick(tikaddr, &tiksav);
	sti();

	/* set init. click rate */
	ev_dclick(3, TRUE);

	/* get st_desk ptr */
	rs_gaddr(ad_sysglo, R_TREE, SCREEN, (VOIDPTR *)&ad_stdesk);
	tree = ad_stdesk;

	/* fix up the GEM rsc. file now that we have an open WS    */
	/* This code is also in gemshlib, but it belongs here so that the correct
	 * default GEM backdrop pattern is set for accessories and autoboot app.
	 */
	i = Getrez();
	if (i != 2 && i != 6)				/* set solid pattern in color modes */
		LLSET(ad_stdesk + 12, 0x00001173L);
	else
		LLSET(ad_stdesk + 12, 0x00001143L);

	wm_init();
#if SUBMENUS
	mn_init();
#endif

#if AES3D
	ini_fsel();
#endif

	/* startup gem libs */
	/* fs_start(); */

	for (i = 0; i < 3; i++)
		LWSET(OB_WIDTH(i), (gl_wchar * gl_ncols));

	LWSET(OB_HEIGHT(0), (gl_hchar * gl_nrows));
	LWSET(OB_HEIGHT(1), (gl_hchar + 2));
	LWSET(OB_HEIGHT(2), (gl_hchar + 3));

	rs_gaddr(ad_sysglo, R_STRING, FSTRING, (VOIDPTR *)&ad_fsel);

	indisp = FALSE;						/* init in dispatch semaphore to not indisp        */

#if DOWARNING
	if (!dowarn)
	{
		lslr = drl;						/* save the dispatcher list     */
		drl = NULL;						/* Don't allow anybody to run   */
		fm_alert(0, "[1][\
 \016\017 Developer Release \016\017 |\
    08/08/88  TOS 1.4 |\
 |\
 WARNING: This OS release |\
         is NOT supported. ]\
[Launch TOS]");
		dowarn = TRUE;

		drl = lslr;						/* restore it       */
	}
#endif

	/* off we go !!! */
	dsptch();

#ifdef ACC_DELAY
	accs_init();
#else
	/* let them run */
	all_run();
#endif


	if (g_autoboot[0])					/* if there is a auto boot program */
	{
#ifndef ACC_DELAY
		for (i = 0; i < 6; i++)			/* let the acc's have a chance  */
			all_run();
#endif
		/* change to dir of autoboot */
		apath = &autopath[0];
		xstrpcpy(g_autoboot, apath);
		*(sh_name(apath)) = 0;			/* change path\filename to path */
		do_cdir(apath[0] - 'A', apath);

		sh_write(1, (g_flag - 0), 0, &g_autoboot[0], "");
	}

#if AESVERSION >= 0x330
	do_once = FALSE;					/* desktop do it once flag */ /* why the hell is that set here? */
#endif
	sh_main();

	rsc_free();							/* free up resource */

	drawstk -= 0x00000400L;				/* reset to bottom  */
	dos_free((VOIDPTR)drawstk);

	gsx_mfree();
#if SUBMENUS
	mn_new();
#endif

	cli();
	giveerr();
	sti();

	/* free up the acc's */
	free_accs();
	
	/* free up special glue */
	
	/* give back the tick */
	cli();
	gl_ticktime = gsx_tick(tiksav, &tiksav);
	sti();

#if (AESVERSION < 0x330) & BINEXACT
	/* close workstation */
	gsx_wsclose();
	/* BUG: vdi function called after v_clswk */
	gsx_escapes(2);						/* exit alpha mode */
#else
	gsx_escapes(2);						/* exit alpha mode */

	/* close workstation */
	gsx_wsclose();
#endif

	/* return GEM's 0xEF int */
	cli();
	givecpm();
	sti();
}


/*
 * process init
 */
/* 306de: 00e1e226 */
VOID pinit(P(PD *) ppd, P(CDA *) pcda)
PP(register PD *ppd;)
PP(CDA *pcda;)
{
	ppd->p_cda = pcda;
	ppd->p_qaddr = ppd->p_queue;
	ppd->p_qindex = 0;
	bfill(8, ' ', ppd->p_name);
}


#if BINEXACT
/* 306de: 00e1e260 */
int32_t set_cache(P(int32_t) newcacr)
PP(register int32_t newcacr;)
{
	asm("dc.w $4e7a,$0002");			/* movec.l cacr,d0  */
	/* WTF? and who gets newcacr into d7 ??? this is an ugly Alcyon-only hack */
	asm("dc.w $4e7b,$7002");			/* movec.l d7,cacr  */
}
#endif


/*
 *	pre read the desktop.inf file and get the resolution set
 *	accordingly. If there are no disks or a desktop.inf does
 *	not exist or the resolutions match then return FALSE.
 *
 * ++ERS 1/14/93: also read the preferred desktop backgrounds
 */
/* 306de: 00e1e27e */
int16_t pred_dinf(NOTHING)
{
	int16_t res;
	int16_t cache;
	register int16_t bsize, fh, change, cont;
	char *pbuf;
	register char *temp;
	int16_t defdrv;
	char *chrptr;
#if (AESVERSION >= 0x330) | !BINEXACT
	int16_t i;
#endif

	UNUSED(chrptr);
	
#if TOSVERSION >= 0x400
	gl_vdo = 0x0L;
	/* _VDO */ /* 7/17/92 */
	getcookie(0x5F56444FL, &gl_vdo);
#endif

	g_autoboot[0] = 0;
	pbuf = dos_alloc((int32_t) SIZE_AFILE);
	change = FALSE;
	sh_get(pbuf, SIZE_AFILE);

#if BINEXACT
	/* BUG: extra parameter here */
	rom_ram(3, (intptr_t)pbuf, 0);			/* res is default from ROM  */
#else
	rom_ram(3, (intptr_t)pbuf);				/* res is default from ROM  */
#endif

#if (AESVERSION >= 0x330) | !BINEXACT
	adeskp[0] = 0x41;					/* 4 = dither, 1 = black */
	adeskp[1] = 0x73;					/* 7 = solid color, 3 = green */
	adeskp[2] = 0x7D;					/* 7 = solid color, D = light cyan */
	awinp[0] = 0x70;
	awinp[1] = 0x70;
	awinp[2] = 0x70;
#endif

	if (ctldown)
		goto p_1;

	if (isdrive() && diskin)			/* there is a disk  */
	{
		defdrv = dos_gdrv();			/* save default drive   */
		dos_chdir("\\");				/* open newdesk.inf */
		fh = dos_open("NEWDESK.INF", 0x0);
		if (DOS_ERR)					/* no file      */
		{								/* try desktop.inf  */
			fh = dos_open(infdata, 0x0);
			if (DOS_ERR)				/* no file      */
			{
				if (isdrive() & 0x04)	/* try the hard disk    */
				{
					do_cdir(2, "\\");
					fh = dos_open("NEWDESK.INF", 0x0);
					if (DOS_ERR)		/* failed to open   */
						fh = dos_open(infdata, 0x0);
				} else
				{
					DOS_ERR = TRUE;
				}
			}
		}

		if (!DOS_ERR)					/* if no error then */
		{								/* read inf file    */
			bsize = dos_read(fh, SIZE_AFILE, pbuf);
			dos_close(fh);
			change = TRUE;
			dos_sdrv(defdrv);			/* set it back to default   */
		}
	}
	/* if we read it from disk, reschange may have changed. */
  p_1:
	if (change)
	{
		temp = pbuf;
		temp[bsize] = '\0';
		cont = TRUE;
		while (*temp && cont)
		{
			if (*temp != '#')
			{
				temp++;
			} else
			{
				temp++;					/* get the auto boot file   */
				if ((*temp == 'Z') && (autoexec))
				{
					temp += 2;			/* get the flag     */
					temp = scan_2(temp, &g_flag);
					temp = escan_str(temp, &g_autoboot[0]);
#if (AESVERSION >= 0x330) | !BINEXACT
				} else if (*temp == 'Q')
				{
					temp += 2;
					for (i = 0; i < 3; i++)
					{
						temp = scan_2(temp, &adeskp[i]);
						temp = scan_2(temp, &awinp[i]);
					}
#endif
				} else if (*temp == 'E')
				{
#if TOSVERSION < 0x400
					temp += 5;
					scan_2(temp, &res);
#define CACHE_OFFSET 6
#else
#define CACHE_OFFSET 11
#endif
					if (LONGFRAME)
					{
						scan_2(temp + CACHE_OFFSET, &cache);	/* fixed 6/26/90 Mui    */
						/* fixed 7/17/92    */
						if (cache & 2)
							set_cache(CACHE_ON);
					} else
					{					/* turn on the bit ?        */
#if BINEXACT /* sigh... */
						trp14(((res & 0xF0) >> 4) ? 0x00400001L : 0x00400000L);
#else
						Blitmode(((res & 0xF0) >> 4) ? 1 : 0);
#endif
					}

#if TOSVERSION >= 0x400
					/* if sparrow mode 7/17/92  */
					if ((gl_vdo & 0xFFFF0000L) == 0x00030000L)
					{					/* not an extended mode     */
						if (gl_rschange)	/* 7/21/92 */
						{
							temp = save_2(temp + 14, d_rezword >> 8);
							save_2(temp, d_rezword);
						} else
						{
							if (*(temp + 14) != 0xD)
							{
								temp += 14;
								temp = scan_2(temp, &res);
								d_rezword = res << 8;
								temp = scan_2(temp, &res);
								d_rezword |= res;
							} else
								break;
						}
					} else
#endif
					{
#if TOSVERSION >= 0x400
						temp += 5;		/* skip other envr stuff    */
						scan_2(temp, &res);	/* don't get the return code    */
#endif
						if (gl_rschange)	/* if we've been here before    */
						{
							save_2(temp, (res & 0xF0) | gl_restype);
						} else
						{
							res &= 0xF;
							gl_rschange = FALSE;
							if (!app_reschange(res))
								change = FALSE;	/* NO no res change     */
						}
					}

				}
			}
		}
	}
	
	/* put in common area for special desk accessories */
	autoexec = FALSE;
	sh_put(pbuf, SIZE_AFILE);
	dos_free(pbuf);
	return (change);
}


/*
 * Save 25 columns and full height of the screen memory
 */
/* 306de: 00e1e4e6 */
BOOLEAN gsx_malloc(NOTHING)
{
#if (TOSVERSION >= 0x400) | !BINEXACT
	int32_t len;
#endif

#if BINEXACT /* sigh... */
	gsx_fix(&gl_tmp, NULL, 0L);
#else
	gsx_fix(&gl_tmp, NULL, 0, 0);
#endif
#if (TOSVERSION >= 0x400) | !BINEXACT
	len = (int32_t) ((uint16_t) gl_wchar) * (int32_t) 25 * (int32_t) ((uint16_t) gl_height) * (int32_t) ((uint16_t) gl_nplanes);
	len = len / 8;
	gl_mlen = len;
#else
	/*
	 * This buggy multiplication actually works, because it is compiled into a muls,
	 * then the long result is stored without further sign-extension.
	 * It thus make use of one the many bugs in the Alcyon compiler,
	 * and has been fixed above for TOS 4.x
	 */
	gl_mlen = (int32_t)((gl_ws.ws_yres + 1) * (gl_ws.ws_xres + 1));
	gl_mlen = (gl_nplanes * gl_mlen) / 16;
#endif

	gl_tmp.fd_addr = dos_alloc(gl_mlen);
#if (TOSVERSION >= 0x400) | !BINEXACT
	if (!gl_tmp.fd_addr)
	{
		gl_mlen = 0;
		Cconws("Unable to alloc AES blt buffer!\r\n");
		return FALSE;
	}
#endif
#if !BINEXACT
	/* BUG: no return here, which will return the value of gl_tmp.fd_addr casted to int... */
	return TRUE;
#endif
}


/*
 * return TRUE if HARD DISK
 * Doesn't NEED to return anything -- 881109 kbad
 */
/* 306de: 00e1e556 */
VOID set_defdrv(NOTHING)
{
	/* This fugly statement gets drvbits, masks drive C,
	 *  and does a GEMDOS Setdrive to drive A (0) if C doesn't exist,
	 *  or to C (2) if it does exist.  Don't ask.  It had to be shrunk.
	 */
	dos_sdrv((isdrive() & 0x04) >> 1);
}


/* 306de: 00e1e572 */
VOID gsx_xmfset(P(MFORM *) pmfnew)
PP(MFORM *pmfnew;)
{
	gsx_moff();
	LWCOPY(ad_intin, pmfnew, sizeof(MFORM) / 2);
	gsx_ncode(ST_CUR_FORM, 0, sizeof(MFORM) / 2);
	gsx_mon();
}


/* 306de: 00e1e5ae */
VOID gsx_mfset(P(MFORM *) pmfnew)
PP(MFORM *pmfnew;)
{
	gsx_moff();
#if AESVERSION >= 0x320
	gl_omform = gl_cmform;
#endif
	LWCOPY(ad_intin, pmfnew, sizeof(MFORM) / 2);
	gsx_ncode(ST_CUR_FORM, 0, sizeof(MFORM) / 2);
#if AESVERSION >= 0x320
	gl_cmform = *pmfnew;
#endif
	gsx_mon();
}


/*
 * AES #78 - graf_mouse - Change the appearance of the mouse pointer.
 *
 * Graf mouse
 */
/* 306de: 00e1e610 */
VOID gr_mouse(P(int16_t) mkind, P(MFORM *) grmaddr)
PP(register int16_t mkind;)
PP(MFORM *grmaddr;)
{
	VOIDPTR maddr;
	MFORM omform;

	if (mkind > USER_DEF)
	{
		switch (mkind)
		{
		case M_OFF:
			gsx_moff();
			break;

		case M_ON:
			gsx_mon();
			break;

#if AESVERSION >= 0x320
		case M_SAVE:						/* save mouse form  */
			rlr->p_mouse = gl_cmform;
			break;

		case M_RESTORE:						/* restore saved mouse form */
			omform = rlr->p_mouse;
			gsx_mfset(&omform);
			break;

		case M_PREV:						/* restore previous mouse form  */
			omform = gl_omform;
			gsx_mfset(&omform);
			break;
#endif
		}
	} else
	{
		if (mkind != USER_DEF)			/* set new mouse form   */
		{
			/* BUG: gsx_mfset will crash if this fails because number is out of range... */
			rs_gaddr(ad_sysglo, R_BIPDATA, MICE0 + mkind, &maddr);
			grmaddr = (MFORM *)LLGET((intptr_t)maddr);
		}

		gsx_mfset(grmaddr);
	}
}


#if AES3D
/*
 * AES #76 - graf_slidebox - Graphics slide box
 *
 * Change code to compensate 3D objects
 */
/* 306de: 00e1e6fe */
int16_t gr_slidebox(P(LPTREE) tree, P(int16_t) parent, P(int16_t) obj, P(int16_t) isvert)
PP(register LPTREE tree;)
PP(int16_t parent;)
PP(int16_t obj;)
PP(int16_t isvert;)
{
#if AES3D
	register GRECT *pt, *pc;			/* new pointer for Reg Opt  */
	GRECT t, c;
	register int32_t divnd, divis;
	OBJECT *objc;
	int16_t pflags, cflags;
	int16_t ret, setxy, setwh;

	UNUSED(setwh);
	
	pt = &t;
	pc = &c;
	/* get the parent real position */
	ob_actxywh(tree, parent, pc);
	/* get the relative position    */
	ob_relxywh(tree, obj, pt);

	objc = (OBJECT *)tree;
	pflags = objc[parent].ob_flags;
	cflags = objc[obj].ob_flags;

	if (pflags & IS3DOBJ)
		pflags = 1;
	else
		pflags = 0;

	if ((cflags & IS3DOBJ))
		cflags = 1;
	else
		cflags = 0;

	setxy = 0;

	if (cflags)							/* if the child is 3D, then check       */
	{
		if (!pflags)					/* if parent is not 3D, then the child is   */
		{								/* ADJ3DPIX off, we need to adjust it       */
			pt->g_x -= ADJ3DPIX;
			pt->g_y -= ADJ3DPIX;
			setxy = 1;
		}
		pt->g_w += (ADJ3DPIX << 1);
		pt->g_h += (ADJ3DPIX << 1);
	}

	gr_dragbox(pt->g_w, pt->g_h, pt->g_x + pc->g_x, pt->g_y + pc->g_y, pc, &pt->g_x, &pt->g_y);

	if (isvert)							/* vertical movement    */
	{
		divnd = pt->g_y - pc->g_y;
		divis = pc->g_h - pt->g_h;
	} else								/* horizontal movement  */
	{
		divnd = pt->g_x - pc->g_x;
		divis = pc->g_w - pt->g_w;
	}

	if (setxy)
	{
		if (divnd)
			divnd += ADJ3DPIX;
	}

	if (divis)
	{
		ret = (divnd * 1000) / divis;
		if (ret > 1000)
			ret = 1000;
		return ret;
	} else
		return 0;
#else
	register GRECT *pt, *pc;						/* new pointer for Reg Opt  */
	GRECT t, c;
	register int16_t divnd, divis;

	pt = &t;
	pc = &c;

	ob_actxywh(tree, parent, pc);
	ob_relxywh(tree, obj, pt);
	gr_dragbox(pt->g_w, pt->g_h, pt->g_x + pc->g_x, pt->g_y + pc->g_y, pc, &pt->g_x, &pt->g_y);

	if (isvert)
	{
		divnd = pt->g_y - pc->g_y;
		divis = pc->g_h - pt->g_h;
	} else
	{
		divnd = pt->g_x - pc->g_x;
		divis = pc->g_w - pt->g_w;
	}
	if (divis)
		return mul_div(divnd, 1000, divis);
	else
		return 0;
#endif
}

#else

/* no AES3D: assembler version used */

#endif


/*
 *	Routine to watch the mouse while the button is down and
 *	it stays inside/outside of the specified rectangle.
 *	Return TRUE as long as the mouse is down.  Block until the
 *	mouse moves into or out of the specified rectangle.
 */
#if AESVERSION >= 0x330
BOOLEAN gr_stilldn(P(int16_t) out, P(int16_t) x, P(int16_t) y, P(int16_t) w, P(int16_t) h)
PP(int16_t out;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t w;)
PP(int16_t h;)
{
	BOOLEAN status;

	/*
	 * compiler had better put the values out, x, y, w, h in the 
	 * right order on the stack to form a MOBLK
	 */
	while (1)
	{
		forker();
		if (!(button & 0x01))
		{
			status = FALSE;
			break;
		} else
		{
			if (out != inside(xrat, yrat, (GRECT *)&x)) /* WTF */
			{
				status = TRUE;
				break;
			}
		}
	}
	return status;
}
#else
/* assembler version used */
#endif


#if 0
/*
 *	Routine to watch the mouse while the button is down and
 *	it stays inside/outside of the specified rectangle.
 *	Return TRUE as long as the mouse is down.  Block until the
 *	mouse moves into or out of the specified rectangle.
 */

BOOLEAN gr_stilldn(P(int16_t) out, P(int16_t) x, P(int16_t) y, P(int16_t) w, P(int16_t) h))
PP(int16_t out;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t w;)
PP(int16_t h;)
{
	int16_t rets[6];
	int16_t event;

	/*
	 * compiler had better put the values out, x, y, w, h in the 
	 * right order on the stack to form a MOBLK WTF
	 */
	while (1)
	{
		event = ev_multi(MU_BUTTON | MU_M1 | MU_TIMER, &out, NULL, 0x0L, 0x00010100L, 0x0L, &rets[0]); /* 01ff00L */

		if (event & MU_BUTTON)			/* button up */
			return FALSE;

		if (event & MU_M1)
			return TRUE;

		if (event & MU_TIMER)
		{
			if (!(rets[2] & 0x01))
				return FALSE;
		}
	}
}
#endif
