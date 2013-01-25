#include <stdio.h>

void array_init(int[],int);/* 参数名并非必需 */
int samplefunc(int x);
int oldfunc();

main()
{
	int i;
	int array[10];
	array_init(array,10);
	for (i = 0;i < 10;++i) {
		printf(" %d",array[i]);
	}
	printf("\n");
	
	printf("samplefunc: %d\n",samplefunc(9));
	printf("oldfunc: %d\n",oldfunc(9));
	
	return 0;
}

//Arguments pass
void array_init(int array[],int size)
{
	int i;
	for(i = 0;i < size;++i)
		array[i] = 0;
}

//ANSI C
int samplefunc(int x)
{
	return x*x;
}

//Old-style
oldfunc(x)
int x;
{
	return x*x;
}
