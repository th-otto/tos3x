#include "aeslib.h"

VOID rc_copy(P(const GRECT *) src, P(GRECT *) dst)
PP(const GRECT *src;)
PP(GRECT *dst;)
{
	register const short *psrc = (const short *)src;
	register short *pdst = (short *)dst;
	*pdst++ = *psrc++;
	*pdst++ = *psrc++;
	*pdst++ = *psrc++;
	*pdst = *psrc;
}
