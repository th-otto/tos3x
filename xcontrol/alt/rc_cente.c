#include "aesalt.h"

void Rc_center(const GRECT *rs, GRECT *rd, WORD xoffset, WORD yoffset)
{
	*rd = *rs;
	rc_shrink(rd, xoffset + xoffset, yoffset + yoffset);
	rc_inset(rd, xoffset, yoffset);
}
