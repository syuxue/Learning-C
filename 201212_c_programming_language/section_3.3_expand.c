#include <stdio.h>

void expand(char from[], char to[]);

int main()
{
	char from[] = "lkjlkjqweqwea-z";
	char to[1024];
	
	expand(from,to);
	printf("%s\n",to);
	
	return 0;
}

void expand(char from[], char to[])
{
	int i,j,last,c;
	
	last = -1;
	for (i = 0,j = 0; from[i] != '\0'; i++) {
		if (
			from[i] == '-' 
			&& ('a' <= last && last <= 'z') 
			&& ('a' <= from[i + 1] && from[i + 1] <= 'z') 
		) {
			to[j++] = '(';
			for (c = last + 1; c < from[i + 1]; c++) {
				to[j++] = c;
			}
			to[j++] = ')';
		} else {
			to[j++] = from[i];
		}
		last = from[i];
	}
	to[j] = '\0';
}
