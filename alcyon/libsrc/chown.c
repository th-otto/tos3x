#include "lib.h"
#include <sys/stat.h>
#include <unistd.h>

/**************************************************/
/* chown - change owner: like access() under CP/M */
/**************************************************/
int chown(P(const char *) name, P(uid_t) owner, P(gid_t) group)
PP(const char *name;)
PP(uid_t owner;)
PP(gid_t group;)
{
	UNUSED(name);
	UNUSED(owner);
	UNUSED(group);
	/* NYI */
	return 0;
}
