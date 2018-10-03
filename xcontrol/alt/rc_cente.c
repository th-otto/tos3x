#include "aesalt.h"

void rc_center(const GRECT *rs, GRECT *rd, WORD xoffset, WORD yoffset)
{
	long *s;
	long *d;
	
	s = (long *)rs;
	d = (long *)rd;
	*d++ = *s++;
	*d++ = *s++;
	rd->g_w -= xoffset;
	rd->g_h += yoffset;
	rc_inset(rd, xoffset, yoffset);
}
