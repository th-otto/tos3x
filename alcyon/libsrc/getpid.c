/* getpid - return process ID */
/*	returns phony number under CP/M */

#include <sys/types.h>
#include <unistd.h>

#define PHONYPID 222

pid_t getpid(NOTHING)
{
	return PHONYPID;
}
