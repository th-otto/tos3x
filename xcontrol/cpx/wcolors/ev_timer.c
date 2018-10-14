#include "..\alt\aesalt.h"

void Evnt_timer(long tim)
{
	evnt_timer((int)(tim & 0xffff), (int)(tim >> 16));
}
