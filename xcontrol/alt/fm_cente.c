#include "aesalt.h"

WORD *_AesCtrl(long code);

void Form_center(const OBJECT *tree, GRECT *r)
{
	long *p;
	long *gr = (long *)r;
	
	_GemParBlk.addrin[0] = tree;
	p = (long *)_AesCtrl(54);
	*gr++ = *p++;
	*gr++ = *p++;
}