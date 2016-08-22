/****************************************************************************/
/*                                                                          */
/*                      X d o p r t f p   R o u t i n e                     */
/*                      -------------------------------                     */
/*                                                                          */
/*      This file contains subroutines called from "_doprt()" which are     */
/*      specific to floating point.  The purpose of having a separate file  */
/*      is so that these routines may be declared global in a special       */
/*      version of "s.o", to allow running without the floating point       */
/*      library routines.                                                   */
/*                                                                          */
/*      Entry Points:                                                       */
/*                                                                          */
/*              petoa(^float, ^buff, prec);                                 */
/*              pftoa(^float, ^buff, prec);                                 */
/*                                                                          */
/*      ^float  is a pointer to the floating number to convert              */
/*      ^buff   is a pointer to the buffer                                  */
/*      prec    is the precision specifier                                  */
/*                                                                          */
/****************************************************************************/

#include "lib.h"
#include <math.h>
#include <string.h>

char *_pftoa(P(double *) addr, P(char *) buf, P(int) prec, P(int) c)
PP(double *addr;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	double fp;
	
	UNUSED(c);
	prec = prec < 0 ? 6 : prec;
	fp = *addr;
	return ftoa(fp, buf, prec);
}

char *_petoa(P(double *) addr, P(char *) buf, P(int) prec, P(int) c)
PP(double *addr;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	double fp;
	
	UNUSED(c);
	prec = prec < 0 ? 6 : prec;
	fp = *addr;
	return etoa(fp, buf, prec);
}

char *_pgtoa(P(double *) addr, P(char *) buf, P(int) prec, P(int) c)
PP(double *addr;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	char *sp;

#ifdef __ALCYON__
	sp = _pftoa(addr, buf, prec);
#else
	sp = _pftoa(addr, buf, prec, c);
#endif
	if (strlen(buf) > (7 + prec))		/* Smallest FP string           */
#ifdef __ALCYON__
		sp = _petoa(addr, buf, prec);
#else
		sp = _petoa(addr, buf, prec, c);
#endif
	return sp;
}
