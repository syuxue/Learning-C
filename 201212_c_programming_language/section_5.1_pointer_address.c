#include <stdio.h>
void swap(int *px, int *py);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	
	//Int
	{
		int i, *ip;
		i = 0;
		printd(i);
		ip = &i;
		printd(*ip);
		for (i = 0; i < 10; i++) printd(*ip);
		for (*ip = 0; *ip < 10; (*ip)++) printd(*ip);
	}
	
	//Array
	{
		char str[] = "Hello, world !";
		char *p;

		for (p = &str[0]; *p != '\0'; p++) printf("%c",*p);
		printf("\n");
	}
	
	//Address
	{
		int i = 1, i2 = 2, i3 = 3, i4 = 4;
		int *p = &i,*q;
		
		printd(*p);
		q = p;
		*q = 99;
		printd(*p);

		printd(i2);
		i2 = *p + 5;
		printd(i2);
		
		printd(i3);
		printd(i4);
		swap(&i3, &i4);
		printd(i3);
		printd(i4);		
	}
	
	return 0;
}

void swap(int *px, int *py)
{
	int tmp;
	tmp = *px;
	*px = *py;
	*py = tmp;
}
