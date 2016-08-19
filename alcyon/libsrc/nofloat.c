/* NOFLOAT: provides 'stubroutines' to satisfy floating point library	    */
/*	references from 'printf' et al.					    */

#include "lib.h"
#include <option.h>
#include <math.h>

VOID nofloat(NOTHING) { ; }					/* stubroutine 'tag'	    */

static char const __nofloat_msg[] = "floating point";

char *petoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double fp;)
PP(char *buf;)
PP(int prec;)
{
	UNUSED(fp);
	UNUSED(buf);
	UNUSED(prec);
	_optoff(__nofloat_msg);
}


char *pftoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double *fp;)
PP(char *buf;)
PP(int prec;)
{
	UNUSED(fp);
	UNUSED(buf);
	UNUSED(prec);
	_optoff(__nofloat_msg);
}


char *pgtoa(P(double) fp, P(char *) buf, P(int) prec)
PP(double *fp;)
PP(char *buf;)
PP(int prec;)
{
	UNUSED(fp);
	UNUSED(buf);
	UNUSED(prec);
	_optoff(__nofloat_msg);
}


double atof(P(const char *) str)
PP(const char *str;)
{
	UNUSED(str);
	_optoff(__nofloat_msg);
	return 0.0;
}
