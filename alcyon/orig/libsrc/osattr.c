/****************************************************************************
* OSATTR.C - sets "os_abilities" word, depending on which OS we are running.
*
* Last modified:
*	1/25/84 whf - handle 68K linker problem
*	1/4/83 whf & rsw - add CPMZ8KV11
*	12/29/83 whf - add os_truncates
*	11/4/83 whf - handle CCPM3.1 and networking
*****************************************************************************/

#include "lib.h"
#include <osif.h>
#include <osiferr.h>

#include "osattr.h"

unsigned short os_abilities = 0;
unsigned short os_version = 0;

VOID osattr(NOTHING)
{
#if CPM
	__OSIF(S_BDOSVER, 0);				/* What OS are we running?  */
	os_version = __cpmrv;				/* Use AX, not AL ret by OSIF */

	switch (os_version)					/* What kind of OS?     */
	{
	case CCPM86V10:					/* CCP/M-86 PC v1.0     */
	case MPM86V21:						/* MP/M-86 v2.0, v2.1       */
		os_abilities |= _os_multitasking | _os_oldmultisec | _os_f_parse | _os_filesharing | _os_datetime;
		break;							/*              */

	case CCPM86V20:					/* CCP/M-86 v2.0        */
		__OSIF(S_OSVER, 0);				/* Get the truth        */
		switch (__cpmrv)				/* Which OS really?     */
		{								/*              */
		case CPM86V31:					/* Fooled by the network!   */
			os_abilities |= _os_fastconio;	/* plus all of this:        */
		case CPM86V11:					/* Fooled by the network!   */
			/* Network will emulate these */
			os_abilities |= _os_newmultisec | _os_f_parse | _os_filesharing | _os_datetime | _os_loadtime;
			break;						/*              */

		case 0x1431:					/* Fooled! CCP/M-86 v3.1    */
			os_abilities |= _os_swaps8087;	/* 8087 support?        */
			/* plus all the rest:       */
		case 0x1420:					/* Definitely CCP/M-86 v2.0 */
		default:						/* Maybe CCP/M-86 v2.0      */
			os_abilities |= _os_multitasking | _os_newmultisec | _os_f_parse |
				_os_filesharing | _os_datetime | _os_loadtime | _os_truncates | _os_fastconio;
		}
		net_check();					/* is there a network?      */
		break;

	case CPM86V31:						/* CP/M-86 Plus         */
		os_abilities |= _os_multitasking | _os_newmultisec | _os_f_parse | _os_truncates | _os_datetime | _os_fastconio;
		break;

/*	  case 0x0014:				/ * CP/M-80 v1.4		    */
/*	  case 0x0022:				/ * CP/M-80 v2.2		    */
/*	  case 0x0031:				/ * CP/M-80 Plus		    */
/*	  case 0x0122:				/ * MP/M-80 v1.1		    */
/*	  case 0x0130:				/ * MP/M-80 v2.0, v2.1	    */

	default:
	case CPM86V11:						/* CP/M-86 v1.0, v1.1       */
	case CPM68KV11:						/* CP/M-68K v1.1        */
	case CPMZ8KV11:						/* CP/M-Z8000 v1.1      */
		break;							/*   no attributes...       */
	}

#endif

#if PCDOS

#endif
}


#if CPM
VOID net_check(NOTHING)
{
	__OSIF(N_NETSTAT, 0);				/* Is net installed?        */
	if (__cpmrv != 0xFFFF && __cpmrv != 0)	/* non bogus return?        */
	{
		os_abilities |= _os_netinstalled;	/* Yes, there's a net       */
		return SUCCESS;					/* If called by someone else */
	} else
		return FAILURE;
}
#endif
