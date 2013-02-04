#include <stdio.h>
#include <string.h>

// 2nd
int isupper_0(char c)
{
	return (c > 64 && c < 91);
}

// 3rd
int isupper_1(char c)
{
	return (c >= 'A' && c <= 'Z');
}

// 4th
int isupper_2(char c)
{
	return strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", c) != NULL;
}

// 1st
int isupper_3(char c)
{
	const static char table[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0aaaaaaaaaaaaaaaaaaaaaaaaaa\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	return table[(int) c];
}

int main(int argc, char *argv[])
{
	int i;

	for (i = 0; i < 10000000; i++)
		isupper_3('G');
	printf("%d\n", isupper_3('G'));

	return 0;
}
