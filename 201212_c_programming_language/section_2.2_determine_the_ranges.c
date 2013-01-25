#include <stdio.h>
#include <limits.h>

main()
{
	int ii;
	short int is,lis;
	long int il;

	//检查short的上下限
	//short: -32768 - 32767
	if (0) {
		printf("define: %d - %d\n",SHRT_MIN,SHRT_MAX);
		for (is = 0,lis = -1; is > lis; is++) lis = is;
		printf("short: %d - ",is);
		for (is = 0,lis = 1; is < lis; is--) lis = is;
		printf("%d\n",is);
	}
	
	//检查int的上下限
	//int: -2147483648 - 2147483647
	if (1) {
		for (ii = 0; ii > -1; ii++) ;
		printf("int: %d - ",ii);
		for (ii = 0; ii < 1; ii--) ;
		printf("%d\n",ii);
	}

	return 0;
}
