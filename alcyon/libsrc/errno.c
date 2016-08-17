/*
 * errno.c : declares storage for errno, a variable containing the last
 *		system error.
 */
#include <osif.h>
#include "lib.h"
#include <errno.h>

int errno = 0;							/* set by low level i/o, unset by user (if at all) */

int _errcpm = 0;						/* assign BDOS return val when assigning errno */
