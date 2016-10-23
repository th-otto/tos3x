/*
*************************************************************************
*                       Revision Control System
* =======================================================================
*  $Revision: 2.2 $     $Source: /u2/MRS/osrevisions/aes/gemaplib.c,v $
* =======================================================================
*  $Author: mui $       $Date: 89/04/26 18:20:33 $
* =======================================================================
* Revision 2.2  89/04/26  18:20:33  mui
* TT
* 
* Revision 2.1  89/02/22  05:24:07  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:30:46  lozben
* Initial revision
* 
*************************************************************************
*/
/*      Fix the ap_tplay, so after it finished, it stay where it is     */
/*      4/17/86                 Derek Mui                               */
/*      Fix at ap_tplay 4/8/86                  Derek Mui               */
/*      Kludge at ap_tplay 3/11/86 - 3/12/86    Derek Mui               */
/*      Fix the ap_trec 03/10/86                Derek Mui               */
/*      1.1             03/20/85                Lowell Webster          */
/*      Reg Opt         03/09/85                Derek Mui               */
/*      GEMAPLIB.C      03/15/84 - 02/09/85     Lee Lorenzen            */
/*      Fix at ap_trecd to fix the length  4/5/90       D.Mui           */
/*      Add ap_init, ap_exit and rd_mymsg       4/3/91          D.Mui   */
/*      Fix at rd_mymsg for set setting the buffer clear flag 4/15/91   */
/*      New version number                      7/7/92          D.Mui   */

/*
*       -------------------------------------------------------------
*       GEM Application Environment Services              Version 1.1
*       Serial No.  XXXX-0000-654321              All Rights Reserved
*       Copyright (C) 1985                      Digital Research Inc.
*       -------------------------------------------------------------
*/

#include "aes.h"
#include "obdefs.h"
#include "struct88.h"
#include "baspag88.h"
#include "gemlib.h"
#include "funcdef.h"


#define TCHNG 0
#define BCHNG 1
#define MCHNG 2
#define KCHNG 3

int16_t gl_bvdisk;
int16_t gl_bvhard;
int16_t gl_recd;
int16_t gl_rlen;
intptr_t gl_rbuf;
int16_t gl_play;					/* 3/11/86  */
intptr_t gl_store;				/* 3/11/86  */
int16_t gl_mx;					/* 3/12/86  */
int16_t gl_my;					/* 3/12/86  */


/*	Application Init	*/

int16_t ap_init(P(intptr_t) pglobal)
PP(intptr_t pglobal;)
{
	LLSET(pglobal, 0x03400001L);		/* version 3.40     */
	LWSET(pglobal + 4, rlr->p_pid);
	LWSET(pglobal + 20, gl_nplanes);
	LLSET(pglobal + 22, &D);
	LWSET(pglobal + 26, gl_bvdisk);
	LWSET(pglobal + 28, gl_bvhard);
	rlr->p_message[0] = 0;
	return (rlr->p_pid);
}


/*	Application Exit	*/

int16_t ap_exit(NOTHING)
{
	mn_clsda();
	if (rlr->p_qindex)
		ap_rdwr(AQRD, rlr->p_pid, rlr->p_qindex, &D.g_valstr[0]);

	all_run();
	release();
	mn_free(rlr->p_pid);
	return (TRUE);
}


/*	Read the internal process message	*/

int16_t rd_mymsg(P(VOIDPTR) buffer)
PP(VOIDPTR buffer;)
{
	if (rlr->p_message[0])				/* there is a message   */
	{
		LBCOPY(buffer, &rlr->p_message[1], 16);
		rlr->p_message[0] = 0;			/* message is sent  */
		return (TRUE);
	} else
		return (FALSE);
}


/*
*	APplication READ or WRITE
*/
int16_t ap_rdwr(P(int16_t) code, P(int16_t) id, P(int16_t) length, P(intptr_t) pbuff)
PP(int16_t code;)
PP(int16_t id;)
PP(int16_t length;)
PP(intptr_t pbuff;)
{
	/* use id,len,pbuff */
	/*   on stack as a QPB  */
	ev_block(code, ADDR(&id));
}



/*
*	APplication FIND
*/
int16_t ap_find(P(intptr_t) pname)
PP(intptr_t pname;)
{
	register PD *p;
	char temp[9];

	LSTCPY(ADDR(&temp[0]), pname);

	p = fpdnm(&temp[0], 0x0);
	return (((p) ? (p->p_pid) : (-1)));
}




/*
*	Application Tape Player
*/
VOID ap_tplay(P(intptr_t) pbuff, P(int16_t) length, P(int16_t) scale)
P(register intptr_t pbuff;)
P(int16_t length;)
P(int16_t scale;)
{
	register int16_t i;
	FPD f;
	intptr_t ad_f;

	ad_f = &f;
	dsptch();							/* dispatch everybody   */
	gl_play = FALSE;
	gl_mx = xrat;
	gl_my = yrat;

	for (i = 0; i < length; i++)
	{
		/* get an event to play */
		LBCOPY(ad_f, pbuff, sizeof(FPD));
		pbuff += sizeof(FPD);
		/* convert it to machine */
		/*   specific form  */

		switch (((int16_t) (f.f_code)))
		{
		case TCHNG:
			ev_timer((f.f_data * 100L) / scale);
			f.f_code = 0;
			break;
		case MCHNG:
			if (!gl_play)				/* diconnect the cursor from VDI    */
			{							/* until the playing is done        */
				i_lptr1(justretf, 0x0);
				gsx_ncode(CUR_VECX, 0x0L);
				m_lptr2(&drwaddr);
				i_lptr1(justretf, 0x0);	/* not interrupt of mouse */
				gsx_ncode(MOT_VECX, 0x0L);	/* movement       */
				m_lptr2(&gl_store);
			}
			f.f_code = mchange;
			gl_play = TRUE;
			break;
		case BCHNG:
			f.f_code = bchange;
			break;
		case KCHNG:
			f.f_code = kchange;
			break;
		}
		/* play it      */
		if (f.f_code)
			forkq(f.f_code, f.f_data);

		/*  let someone else      */
		/*  hear it and respond   */
		dsptch();
	}

	if (gl_play)						/* connect back the mouse */
	{
#if UNLINKED
		intin[0] = 1;					/* do a mouse sample mode */
		intin[1] = 2;
		gsx_ncode(33, 0, 2);
		drawrat(gl_mx, gl_my);
		ptsin[0] = gl_mx;
		ptsin[1] = gl_my;
		gsx_ncode(28, 1, 0);
#endif
		i_lptr1(drwaddr);
		gsx_ncode(CUR_VECX, 0x0L);
		i_lptr1(gl_store, 0x0);
		gsx_ncode(MOT_VECX, 0x0L);

#if UNLINKED
		xrat = gl_mx;
		yrat = gl_my;
#endif

		gl_play = FALSE;
	}
}


/*
*	APplication Tape RECorDer
*/
int16_t ap_trecd(P(intptr_t) pbuff, P(int16_t) length)
PP(register intptr_t pbuff;)
PP(register int16_t length;)
{
	register int16_t i;
	register intptr_t code;
	FCODE trash;

	/* start recording in   */
	/*   forker()       */
	cli();
	gl_recd = TRUE;
	gl_rlen = length;
	gl_rbuf = pbuff;
	sti();
	/* 1/10 of a second */
	/*   sample rate    */
	while (gl_recd)
		ev_timer(100L);
	/* done recording so    */
	/*   figure out length  */
	cli();
	gl_recd = FALSE;
	length = length - gl_rlen;			/* Fixed 4/5/90     */
	gl_rlen = 0;
/*	length = ((int16_t)(gl_rbuf - pbuff)) / sizeof(FPD);	*/
	gl_rbuf = 0x0L;
	sti();
	/* convert to machine independent recording */
	for (i = 0; i < length; i++)
	{
		code = 0x0L;
		trash = (FCODE) LLGET(pbuff);
		if (trash == tchange)
		{
			code = TCHNG;				/*    int16_t is changed to int32_t   */
/*	    LLSET(pbuff+sizeof(int32_t *), LLGET(pbuff+sizeof(int32_t *)) * 
			gl_ticktime);	*/
		}
		if (trash == mchange)
			code = MCHNG;
		if (trash == kchange)
			code = KCHNG;
		if (trash == bchange)
			code = BCHNG;
		LLSET(pbuff, code);
		pbuff += sizeof(FPD);
	}
	return length;
}
