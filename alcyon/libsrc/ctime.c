#include "lib.h"
#include <time.h>

char *ctime(P(const time_t *) ptime)
PP(const time_t *ptime;)
{
    struct tm *ptm = localtime(ptime);
    if (ptm == NULL)
    {
        return NULL;
    }
    return asctime(ptm);
}
