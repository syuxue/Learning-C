#include <stdio.h>
#include <math.h>

main()
{	
	int i;

	//Integer,Float
	i = 1;
	float f = 0.69;
	printf("%d,%f,%d",i,f,f);
	printf("\n");
	
	//Char
	char c = 's';
	printf("%c,%c",c,c - 1);
	printf("\n");

	//toUpper
	i = 0;
	int last = '\0';
	while ((c = getchar()) != EOF) {
		printf("%c",('a' <= c && c <= 'z' && (last == '\0' || last == ' ')) ? c - 32 : c);
		last = c;
	}
	printf("\n");
	
	//compare Unsigned to Signed
	long l1 = -1;
	unsigned long l2 = 1;
	printf("%d > %d = %d\n",l1,l2,l1 > l2);
	
	//SQRT
	i = 4;
	printf("sqrt(%d) = %f\n",i,sqrt(i));
	
	return 0;
}

