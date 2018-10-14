#include "gemskel.h"


void ToColor(Colorword *c, int d)
{
	*((short *)c) = d;
}

void FColor(Colorword *c, int *d)
{
	*((Colorword *)d) = *c;
}
