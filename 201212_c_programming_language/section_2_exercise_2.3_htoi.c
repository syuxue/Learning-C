#include <stdio.h>
#include <math.h>

int htoi(char hex[]);

main()
{	
	int i,c;
	char str[100];
	for (i = 0; i < 100 && (c = getchar()) != EOF && c != '\n'; i++) {
		str[i] = c;
	}
	str[i] = '\0';

	printf("%d\n",htoi(str));
	return 0;
}

int htoi(char hex[])
{
	int		i,c,maxbit;
	long	integer = 0;
	
	if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
		for (i = 2; hex[i] != '\0'; i++) ;//找最大位数
		maxbit = --i;
		
		for (; i > 1; i--) {
			if ('0' <= hex[i] && hex[i] <= '9') c = (int)hex[i] - 48;
			else if ('a' <= hex[i] && hex[i] <= 'f') c = (int)hex[i] - 97 + 10;
			else if ('A' <= hex[i] && hex[i] <= 'F') c = (int)hex[i] - 65 + 10;
			integer += c * pow(16,maxbit - i);
			printf("%d * pow(16,%d - %d)%d\n",c,maxbit,i,(int)pow(16,maxbit - i));
		}
		return integer;
	} else {
		return 0;
	}
}
