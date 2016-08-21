/****************************************************************************/
/*									    */
/*			S i g n a l   F u n c t i o n			    */
/*			-----------------------------			    */
/*									    */
/*	The "signal" function allows a routine in C to catch a 68000 	    */
/*	interrupt, and service it with a C function.			    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = signal(sig,func);					    */
/*									    */
/*	Where:								    */
/*		ret	Is always 0 under CP/M.				    */
/*		sig	Is the UNIX signal number			    */
/*		func	-> the function to service the exception	    */
/*									    */
/****************************************************************************/

#include "lib.h"
#include <stdio.h>						/* Include standard stuff   */
#include <signal.h>						/* Include definitions      */
#include <stdlib.h> /* for _exit() */
#include <osif.h>						/* Include BDOS Functions   */

extern VOID _illinst PROTO((NOTHING));			/* -> Illegal instruction ep */

extern VOID _trace PROTO((NOTHING));			/* -> Trace trap epa        */

extern VOID _trap PROTO((NOTHING));				/* -> TRAP instruction epa  */

extern VOID _buserr PROTO((NOTHING));			/* -> BUSERR epa        */

extern VOID _arith PROTO((NOTHING));			/* -> Arithmetic traps epa  */

sighandler_t __signal[NSIG];			/* Holds user func addresses */

#define TRAP(i) ((i) + 32)

#if GEMDOS
#define Setexc(v,f) bios(5,v,(long)(f))
static VOIDPTR sysvecs[48];
static VOIDPTR etv_term;
static int installed;


static VOID resetvecs(NOTHING)
{
	register int i;
	
	if (installed)
	{
		for (i = 2; i < 48; i++)
			if (i != TRAP(1) && i != TRAP(2) && i != TRAP(13) && i != TRAP(14))
				Setexc(i, sysvecs[i]);
		Setexc(0x102, etv_term);
		etv_term = 0;
		installed = 0;
	}
}

#endif




static VOID _setvec(P(int) vector, P(VOIDPTR) func)
PP(int vector;)								/* Vector #         */
PP(VOIDPTR func;)								/* Function address     */
{
#if GEMDOS
	Setexc(vector, func);
#else
	struct
	{									/* A CP/M EPB structure     */
		int vec;						/* Vector number        */
		char *userepa;					/* User's epa           */
		char *bdosepa;					/* BDOS's epa           */
	} epb;

	epb.vec = vector;					/* Set up vector        */
	epb.userepa = func;					/* and function         */
	epb.bdosepa = 0L;					/* Clear return word        */
	/* Do it. */
	__BDOS(SETVEC, (long)&epb);
#endif
}


static VOID do_nothing(P(int) sig)
PP(int sig;)
{
	UNUSED(sig);
}


static VOID do_abort(P(int) sig)
PP(int sig;)
{
	_exit(sig | 0x80);
}


sighandler_t signal(sig, func)
register int sig;						/* Signal number        */
register sighandler_t func;			/* Function address     */
{
	register int i;							/* Temp             */
	sighandler_t prev;
	
	if (sig >= NSIG || sig <= 0 || func == SIG_ERR)			/* Too big?         */
		return SIG_ERR;				/* Yes, return UNIX NFG code */

	prev = __signal[sig];
	if (prev == do_nothing)
		prev = SIG_IGN;
	else if (prev == do_abort)
		prev = SIG_DFL;
	
	if (func == SIG_IGN)
		func = do_nothing;
	else if (func == SIG_DFL)
		func = do_abort;
	__signal[sig] = func;				/* Save the function addr   */

#if GEMDOS
	if (!installed)
	{
		for (i = 2; i < 48; i++)
			sysvecs[i] = (VOIDPTR)Setexc(i, (VOIDPTR)-1L);
		etv_term = (VOIDPTR)Setexc(0x102, resetvecs);
		installed = 1;
	}
#endif

	switch (sig)						/* Do signal processing     */
	{
	case SIGILL:
		_setvec(4, _illinst);			/* Illegal Instruction      */
		_setvec(8, _illinst);			/* Privilege violation      */
		_setvec(10, _illinst);			/* Set vector #10 (line A)  */
		_setvec(11, _illinst);			/* Set vector #11 (line F)  */
		break;

	case SIGTRAP:						/* Trace trap           */
		_setvec(9, _trace);				/* Set vector #9        */
		break;

	case SIGIOT:						/* Bad TRAP instruction     */
		_setvec(TRAP(0), _trap);				/* Trap 0           */
		for (i = TRAP(3); i < TRAP(16); i++)	/* Traps 4 - 15         */
#if GEMDOS
			if (i != TRAP(13) && i != TRAP(14))		/* to not overwrite bios/xbios */
#endif
				_setvec(i, _trap);
		break;

	case SIGBUS:						/* BUSERR PDP-11 trap 4 & 10 */
	case SIGSEGV:
		_setvec(2, _buserr);			/* Set nxm vector       */
		_setvec(3, _buserr);			/* Set addressing vector    */
		break;

	case SIGFPE:						/* Arithmetic section       */
		for (i = 5; i < 8; i++)			/* Set DIV0/CHK/TRAPV       */
			_setvec(i, _arith);
		break;
	}
	/* Return OK */
	return prev;
}
