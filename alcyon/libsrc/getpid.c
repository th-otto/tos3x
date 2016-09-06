/* getpid - return process ID */
/*	returns phony number under CP/M */

#include "lib.h"
#include <sys/types.h>
#include <unistd.h>
#include <mint/basepage.h>

pid_t getpid(NOTHING)
{
	return pid_from_basepage(_base);
}
