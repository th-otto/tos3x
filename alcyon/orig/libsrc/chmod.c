#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/***************************************/
/* chmod - change mode: NOP under CP/M */
/***************************************/
int chmod(P(const char *) name, P(mode_t) mode)
PP(const char *name;)
PP(mode_t mode;)
{
	/* BUG: mode is a total different thing here */
	return access(name, mode);
}
