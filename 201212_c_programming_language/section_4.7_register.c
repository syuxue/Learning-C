#include <stdio.h>
int addself(void);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < 500000000; i++) {
		addself();
	}
	printf("%d\n",addself());
	
	return 0;
}

int addself(void)
{
	register int i = 0;
	return i++;
}
