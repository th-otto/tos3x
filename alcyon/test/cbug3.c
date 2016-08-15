#include <stdio.h>

/* diviuision routine for long operands do not act correctly
  for negative operands */

main()
{
	long a, b;

	a= -12;
	b= -3;
	printf("%ld\n", a/b);
}
