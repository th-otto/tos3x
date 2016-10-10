/*
*******************************  cbssdefs.c  **********************************
*
*
* $Revision: 3.1 $	$Source: /u/lozben/projects/vdi/mtaskvdi/RCS/cbssdefs.c,v $
* =============================================================================
* $Author: lozben $	$Date: 91/07/29 11:44:43 $     $Locker:  $
* =============================================================================
*
* $Log:	cbssdefs.c,v $
* Revision 3.1  91/07/29  11:44:43  lozben
* Declared vdivars structure and a pointer to it and another pointer
* that point to one of the structures elements.
* 
* Revision 3.0  91/01/03  15:06:26  lozben
* New generation VDI
* 
* Revision 2.1  89/02/21  17:18:35  kbad
* *** TOS 1.4  FINAL RELEASE VERSION ***
* 
*******************************************************************************
*/

#include "vdi.h"
#include "fontdef.h"
#include "attrdef.h"
#include "scrndev.h"
#include "lineavar.h"

/*
 * Storage declarations for C structures
 */
ATTRIBUTE virt_work;			/* attribute areas for workstations */
#if !LINEA_HACK
VDIVARS	vdivars;
#endif
#if TOSVERSION >= 0x400
int16_t *lineAVar = &vdivars.vPlanes; /* accessed by asm code only */
VDIVARS	*la = &vdiars; /* accessed by C code only */
#endif
