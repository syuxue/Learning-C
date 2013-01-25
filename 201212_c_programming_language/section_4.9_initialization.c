#include <stdio.h>

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int i;
	int arr[10] = {1,2,3,4}; // 当指明的数量小于初始化的元素数时，剩余元素将被初始化为0，external,static,automatic有效
	int arr2[10];
	char pattern[] = "ould";
	char pattern2[] = {'o', 'u', 'l', 'd', '\0'};
	
	printf("arr = ");
	for (i = 0; i < 10; i++)
		printf("%d,",arr[i]);
	printf("\n");

	printf("arr2 = ");
	for (i = 0; i < 10; i++)
		printf("%d,",arr2[i]);
	printf("\n");
	
	printf("%s\n%s\n",pattern,pattern2);
	
	return 0;
}
