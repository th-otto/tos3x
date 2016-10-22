/*
*************************************************************************
*			Revision Control System
* =======================================================================
*  $Revision: 2.2 $	$Source: /u2/MRS/osrevisions/aes/gembase.c,v $
* =======================================================================
*  $Author: mui $	$Date: 89/04/26 18:20:54 $	$Locker: kbad $
* =======================================================================
*  $Log:	gembase.c,v $
* Revision 2.2  89/04/26  18:20:54  mui
* TT
* 
* Revision 2.1  89/02/22  05:24:25  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  88/06/02  12:30:53  lozben
* Initial revision
* 
*************************************************************************
*/
/*	GEMBASE.C	1/28/84	- 01/07/85	Lee Jay Lorenzen	*/
/*	Clean up 	1/15/88			D.Mui			*/

/*
*	-------------------------------------------------------------
*	GEM Application Environment Services		  Version 1.0
*	Serial No.  XXXX-0000-654321		  All Rights Reserved
*	Copyright (C) 1985			Digital Research Inc.
*	-------------------------------------------------------------
*/

#include <portab.h>
#include <machine.h>
#include <struct88.h>

PD *rlr;
PD *drl;
PD *nrl;

EVB *eul;
EVB *dlr;
EVB *zlr;

int32_t elinkoff;

/*	char	dodisp;		*/
char indisp;

char infork;

int16_t fpt;
int16_t fph;
int16_t fpcnt;									/* forkq tail, head,    */

						/*   count      */
SPB wind_spb;

CDA *cda;

int16_t curpid;
