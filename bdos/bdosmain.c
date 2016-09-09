/* bdosmain.c - 'front end' to bdos.  This is OEM definable module	*/


#include "tos.h"
#include "bios.h"


/*
**  TIMESTAMP - set to 1 if it's okay for the bdos to print the time
**	stamp string at init time.  Otherwise, set to 0.
*/

#define	TIMESTAMP	1


/*
**  supstk - common sup stack for all procs.  OEMs may change the size of 
**	this stack.
*/

#define	SUPSIZ	1024
int16_t supstk[SUPSIZ];



/*
**  bufl - buffer lists
**	two lists:  fat,dir / data
**	these lists should be initialized by the bios.
**	(vme bios does this in biosc.c)
*/

BCB *bufl[2];


/*
**  osinit - the bios calls this routine to initialize the os
*/

VOID osinit(NOTHING)
{
	_osinit();							/*  real routine in bdos.arc    */
#if	TIMESTAMP
	kprintf(bdosts);					/*  kprintf is in tools.arc */
	/*  bdosts is in bdos.arc   */
#endif
}
