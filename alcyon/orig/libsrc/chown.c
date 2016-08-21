#include <osif.h>
#include "lib.h"
#include <osiferr.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/**************************************************/
/* chown - change owner: like access() under CP/M */
/**************************************************/
int chown(P(const char *) name, P(uid_t) owner, P(gid_t) group)
PP(const char *name;)
PP(uid_t owner;)
PP(gid_t group;)
{
	UNUSED(owner);
	UNUSED(group);
	return access(name, F_OK);
}
