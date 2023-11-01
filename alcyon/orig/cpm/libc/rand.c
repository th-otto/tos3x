/* rand.c - kluges a random number generator with addition & overflow */

#include "lib.h"
#include <stdlib.h>


#define NSEEDS 7

static int _seeds[NSEEDS] = { 0, 24213, 12345, 4622, 2143, 32010, 7942 };
static int _seedptr = 0;


int rand(NOTHING)
{
	register unsigned int tot, ii;

	for (tot = 0, ii = 0; ii < NSEEDS; ii++)
		tot += _seeds[ii];				/* ignore overflow */
	if (++_seedptr >= NSEEDS)
		_seedptr = 0;
	_seeds[_seedptr] = tot;
	return tot >> 1;					/* ignore lo bit because of addition */
}


int srand(P(unsigned int) seed1)
PP(unsigned int seed1;)
{
	register int ncs;

	_seeds[0] = seed1;
	for (ncs = seed1 & 077; ncs; ncs--)
		rand();
	return rand();
}
