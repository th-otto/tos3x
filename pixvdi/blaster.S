******************* Revision Control System *****************************
*
* $Author: lozben $
* =======================================================================
*
* $Date: 91/01/03 15:06:08 $
* =======================================================================
*
* $Locker:  $
* =======================================================================
*
* $Log:	blaster.s,v $
* Revision 3.0  91/01/03  15:06:08  lozben
* New generation VDI
* 
* Revision 2.1  89/02/21  17:17:57  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
* Revision 1.1  87/11/20  15:14:08  lozben
* Initial revision
* 
*
* =======================================================================
*
* $Revision: 3.0 $
* =======================================================================
*
* $Source: /u/lozben/projects/vdi/mtaskvdi/RCS/blaster.s,v $
* =======================================================================
*
*************************************************************************

#include "../common/config.h"
#include "vdidefs.h"
#include "asmdefs.h"
#include "tosvars.inc"

#include "devequ.inc"
#include "lineaequ.inc"


		xdef	BLASTER
		xdef	st_blaster

BLASTER		equ	$FF8A00

st_blaster:	rts

