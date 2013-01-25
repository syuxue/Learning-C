#include <stdio.h>

int php_strpos(char haystack[], char needle[]);

main()
{	
	char haystack[] = " aaawosrld";
	char needle[] = "wosr";
	
	printf("%d\n",php_strpos(haystack,needle));
	return 0;
}

int php_strpos(char haystack[], char needle[])
{
	int i,j,pos;
	
	pos = -1;
	i = j = 0;
	while (haystack[i] != '\0') {
		if (haystack[i++] == needle[j++]) {
			if (j == 1)	pos = i - 1;
			if (needle[j] == '\0') break;
		} else {
			j = 0;
			pos = -1;
		}
	}
	
	return pos;
}
