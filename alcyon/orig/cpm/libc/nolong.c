/* NOLONG: provides 'stubroutines' to satisfy long int conversion
	library references from 'printf' et al.	
*/

#include "lib.h"
#include <option.h>

/* stubroutine 'tag'	    */
VOID nolong(NOTHING) { ; }

static const char * __nolong_msg = "long int conversion";

char *__prtld(P(char *) pobj, P(char **) pbuf, P(int) base, P(int) issigned, P(char *) digs)
PP(char *pobj;)
PP(char **pbuf;)
PP(int base;)
PP(int issigned;)
PP(char *digs;)
{
	UNUSED(pobj);
	UNUSED(pbuf);
	UNUSED(base);
	UNUSED(issigned);
	UNUSED(digs);
	_optoff(__nolong_msg);
#ifndef __ALCYON__
	return 0;
#endif
}

