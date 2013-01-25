#include <stdio.h>
#include "m_function.h"

/* ****************************** Declaration ****************************** */

/* ****************************** Fcuntion ****************************** */
void showvoidp(void **p)
{
	printf("%c\n", 
		*((char *)  *(p+0) + 1)
	);
	printf("%d\n", 
		*((int  *)  *(p+1) + 1)
	);
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	//使用void指针时，不可以直接使用，需先强制转换成所需类型
	char str[] = "Hello, world !";
	int arr[] = {1,99,3,4,5,6,7,8,9,0};
	void *p[2], *p2;
	p[0] = str;
	p[1] = arr;
	
	showvoidp((void **)p);

	printf("%s\n", (char *) *(p+0) );
	printf("%d\n", *((int  *) *(p+1) + 1) );
	//printf("%d\n", (int  *) **(p+1) );
	printf("%d\n", *((int  *)p[1] + 1));

	p2 = str;
	printf("%c\n", *((char *)p2 + 1));

	p2 = arr;
	printf("%d\n", *((int  *)p2 + 1));

	return 0;
}
