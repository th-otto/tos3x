/****************************************************************************
*	_errmalloc / MALLOCND.C : stubroutine for malloc() error handler
*	Let the user program handle it.
*
*	10/83   whf
*****************************************************************************/

#include "lib.h"

/* stubroutine tag */
int malloc_debug(NOTHING)
{
#ifdef __GNUC__
	return 0;
#endif
}

VOID _errmalloc(P(int) etype)
{
	UNUSED(etype);
}
