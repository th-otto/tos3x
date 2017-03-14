#include "aeslib.h"

/**************************************************************************/
/* Function:    rc_equal()						  */
/* Description: check if 2 rectangles are equal				  */
/**************************************************************************/
short rc_equal(P(const GRECT *) p1, P(const GRECT *) p2)
PP(register const GRECT *p1;)
PP(register const GRECT *p2;)
{
	if (p1->g_x != p2->g_x ||
		p1->g_y != p2->g_y ||
		p1->g_w != p2->g_w ||
		p1->g_h != p2->g_h)
		return FALSE;
	return TRUE;
}
