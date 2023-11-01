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

#include <stdio.h>						/* Include standard stuff   */
#include <signal.h>						/* Include definitions      */
#include <osif.h>						/* Include BDOS Functions   */

extern VOID _illinst PROTO((NOTHING));			/* -> Illegal instruction ep */

extern VOID _trace PROTO((NOTHING));				/* -> Trace trap epa        */

extern VOID _trap PROTO((NOTHING));				/* -> TRAP instruction epa  */

extern VOID _buserr PROTO((NOTHING));			/* -> BUSERR epa        */

extern VOID _arith PROTO((NOTHING));				/* -> Arithmetic traps epa  */

extern sighandler_t __signal[NSIG];			/* Holds user func addresses */


VOID _setvec PROTO((int vector, VOIDPTR func));


sighandler_t signal(sig, func)
register int sig;						/* Signal number        */
register sighandler_t func;			/* Function address     */
{
	register int i;							/* Temp             */

	if (sig >= NSIG || sig < 0)			/* Too big?         */
		return SIG_ERR;				/* Yes, return UNIX NFG code */

	/* BUG: func will be called without checking for SIG_DFL/SIG_IGN */
	__signal[sig] = func;				/* Save the function addr   */

	switch (sig)						/* Do signal processing     */
	{									/*              */
	case SIGILL:						/* Illegal instruction      */
		_setvec(4, _illinst);			/* Set vector #4        */
		_setvec(8, _illinst);			/* Privilege violation      */
		_setvec(10, _illinst);			/* Set vector #10 (line A)  */
		_setvec(11, _illinst);			/* Set vector #11 (line F)  */
		break;

	case SIGTRAP:						/* Trace trap           */
		_setvec(9, _trace);				/* Set vector #9        */
		break;

	case SIGIOT:						/* Bad TRAP instruction     */
		_setvec(32, _trap);				/* Trap 0           */
		for (i = 35; i < 48; i++)		/* Traps 4 - 15         */
			_setvec(i, _trap);
		break;

	case SIGBUS:						/* BUSERR PDP-11 trap 4 & 10 */
		_setvec(2, _buserr);			/* Set nxm vector       */
		_setvec(3, _buserr);			/* Set addressing vector    */
		break;

	case SIGFPE:						/* Arithmetic section       */
		for (i = 5; i < 8; i++)			/* Set all misc vectors     */
			_setvec(i, _arith);
		break;
	}
	/* Return OK */
	return SIG_DFL; /* BUG: should return previous user handler */
}


VOID _setvec(P(int) vector, P(VOIDPTR) func)
PP(int vector;)								/* Vector #         */
PP(VOIDPTR func;)								/* Function address     */
{
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
	__BDOS(SETVEC, (long)&epb);	/* BUG: Calls gemdos(61) = Fopen */
}
