/* pre-increment operator always executed before the evaluation
  of expression, even if it contains conditional operator */

main()
{
	int a, b;

	a=1;
	b=1;
	printf("%d\n", a!=1?++b:b);
}
