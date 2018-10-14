#include "aesalt.h"

#ifdef __TURBOC__

MRETS *_AesCtrl(long code);

void Graf_mkstate(MRETS *m)
{
	*m = _AesCtrl(79);
}
#endif
