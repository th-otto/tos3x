/* NOFLOAT: provides 'stubroutines' to satisfy floating point library	    */
/*	references from 'printf' et al.					    */

#include "lib.h"
#include <option.h>

VOID nofloat(NOTHING) { ; }					/* stubroutine 'tag'	    */

static const char *__nofloat_msg = "floating point";

char *_petoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	_optoff(__nofloat_msg);
}


char *_pftoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	_optoff(__nofloat_msg);
}


char *_pgtoa(P(double *) pb, P(char *) buf, P(int) prec, P(int) c)
PP(double *pb;)
PP(char *buf;)
PP(int prec;)
PP(int c;)
{
	UNUSED(pb);
	UNUSED(buf);
	UNUSED(prec);
	UNUSED(c);
	_optoff(__nofloat_msg);
}


double _atof(P(const char *) str)
PP(const char *str;)
{
	UNUSED(str);
	_optoff(__nofloat_msg);
}
