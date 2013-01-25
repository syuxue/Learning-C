#include <stdio.h>
#include <ctype.h>
#include "m_function.h"
#define SIZE 1000

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int i, n;
	double array[SIZE], getfloat(double *);

	for (n = 0; n < SIZE && getfloat(&array[n]) != EOF; n++) ;
	for (i = 0; i < n; i++)
		printf("array[%d] = %g\n",i,array[i]);

	return 0;
}

double getfloat(double *pa)
{
	int i, c, inwhich;
	char buff[32];

	inwhich = i = 0;
	while ((c = getchar()) != EOF) {
		if (inwhich == 0 && c == '-' || c == '+') {
			buff[i++] = c;
			inwhich = 2;
		}else if (isdigit(c) || (inwhich == 1 && c == '.')) {
			buff[i++] = c;
			inwhich = 1;
		} else {
			if (!inwhich)
				continue;
			else if (inwhich == 2) {
				inwhich = i = 0;
				ungetc(c, stdin);
			} else if (inwhich) 
				break;
		}
	}
	buff[i] = '\0';

	*pa = m_str2float(buff, 10);

	return c;
}
