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

char *pftoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double fp;)
PP(char * buf;)
PP(int prec;)
{
	if (prec < 0)
		prec = 6;
	return ftoa(fp, buf, prec);
}


char *petoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double fp;)
PP(char *buf;)
PP(int prec;)
{
	if (prec < 0)
		prec = 6;
	return etoa(fp, buf, prec);
}


char *pgtoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double fp;)
PP(char *buf;)
PP(int prec;)
{
	char *sp;

	sp = pftoa(fp, buf, prec);
	if ((int)strlen(buf) > (7 + prec))		/* Smallest FP string           */
		sp = petoa(fp, buf, prec);
	return sp;
}
