#include "lib.h"

/* unsigned long divide */

long uldivr;

long uldiv(P(long, al1), P(long, al2))
PP(long, al1;)
PP(long, al2;)
{
	register unsigned long l1, l2;
	register long q, b;

	l1 = al1;
	l2 = al2;
	if (l2 == 0)
	{
		uldivr = 0x80000000;
		return 0x80000000;
	}
	if (l2 > l1)
	{							/** comparison (ble->bls) **/
		uldivr = l1;
		return 0;
	}
	if (l1 == l2)
	{
		q = 1;
		l1 = 0;
		goto doret;
	}
	b = 1;								/* bit value */
	while (l1 >= l2)
	{							/** comparison (blt->blo) **/
		if (l2 > (l2 << 1))		/** comparison (bgt->bhi) **/
			break;						/* detect overflow */
		l2 <<= 1;
		b <<= 1;
	}
	q = 0;
	while (b)
	{									/* now do shifts and subtracts */
		if (l1 >= l2)
		{						/** comparison (blt->blo) **/
			q |= b;
			l1 -= l2;
		}
		b >>= 1;
		l2 >>= 1;
	}
  doret:
	uldivr = l1;
	return q;
}
