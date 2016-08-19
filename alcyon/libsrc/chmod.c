#include "lib.h"
#include <sys/stat.h>

int chmod(P(const char *) path, P(mode_t) mode)
PP(const char *path;)
PP(mode_t mode;)
{
	/* NYI */
	UNUSED(path);
	UNUSED(mode);
	return 0;
}
