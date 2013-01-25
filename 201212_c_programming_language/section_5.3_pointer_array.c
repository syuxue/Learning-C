#include <stdio.h>
#include <ctype.h>
#include "m_function.h"
int m_strlen2(char *str);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char str[] = "xixi@xix";
	char str2[] = "hahahaha";
	char *ps[] = {str, &str2[0]};

	printf("%s\n",ps[0]);
	printf("%s\n",ps[1]);
	
	printf("# All of these are same:\n");
	printf("#     str[4] = %c\n",	str[4]		);
	printf("#   *(str+4) = %c\n",	*(str+4)	);
	printf("#   ps[0][4] = %c\n",	ps[0][4]	);
	printf("# *(ps[0]+4) = %c\n",	*(ps[0]+4)	);
	printf("#   (*ps)[4] = %c\n",	(*ps)[4]	);
	printf("# *((*ps)+4) = %c\n",	*((*ps)+4)	);
	printf("%d,%d\n", (int)&str[4], (int)(str+4));


	{
		int i;
		for (i = 0; *(str+i) != '\0'; i++)
			printf("%c",*(str+i));
		printf("\n");
	}
	
	{
		// char s[] === char *s
		char *str = "this is a big day !";
		printf("*str = %c\n", *str);
		printf("%d\n", m_strlen2(str));
		printf("*str = %c\n", *str);
	}

	{
		int *arr = {1,2,3};
		printf("%d\n", *arr);
	}

	return 0;
}

int m_strlen2(char *str)
{
	int n;
	for (n = 0; *str != '\0'; str++, n++) ;
	return n;
}
