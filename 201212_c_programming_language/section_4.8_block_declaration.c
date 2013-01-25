#include <stdio.h>
int showext(void);
int ext = 999;

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int i;
	
	printf("# Arguments\n");
	for (i = 0; i < argc; i++) {
		printf("# argv[%d] = %s\n",i,argv[i]);
	}

	printf("%d\n",i);
	
	if (i > 0) {
		int i; //这个i是重新定义的
		printf("%d\n",i);
	}
	
	printf("%d\n",i); //这个i与14行的i相同
	
	showext(); //这里输出函数中定义的111
	
	return 0;
}

int showext(void)
{
	int ext = 111;
	printf("ext = %d\n",ext);
}
