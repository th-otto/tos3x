#include <osif.h>
#include "lib.h"
#include <option.h>

#undef MAXCCBS
#define MAXCCBS 5					/* Maximum Num CCBs     */

VOID maxfiles5(NOTHING) { }				/* stubroutine for option.h */

#include "allocc.c"
#include "chinit.c"
#include "chkc.c"
