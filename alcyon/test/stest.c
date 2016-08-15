#include <stdio.h>
#include <signal.h>
main()
{
	REG	WORD	*a,b,c;
	WORD	berr(),zdiv();
	signal(10,berr);
	signal( 8,zdiv);
	a = -1;
	b = *a;
	printf("Return from BUSERR\n");
	c = 0;
	b = 1;
	b = b/c;
	printf("Return from Zero divide\n");
}
berr()
{
	printf("Buserr exception!\n");
}
zdiv()
{
	printf("Zero divide exception\n");
}
