#include <stdio.h>

void escape(char s[],char t[]);

int main()
{
	int i,c;
	char input[10240],target[10240];
	
	for (i = 0; (c = getchar()) != EOF; i++)
		input[i] = c;

	escape(input,target);
	printf("%s",target);
	
	return 0;
}

void escape(char s[],char t[])
{
	int i,j;
	for (i = 0,j = 0; s[i] != '\0'; i++) {
		switch (s[i]) {
			case '\t':
				t[j++] = '\\';
				t[j++] = 't';
				break;
			case '\n':
				t[j++] = '\\';
				t[j++] = 'n';
				break;
		}
		t[j++] = s[i];
	}
}
