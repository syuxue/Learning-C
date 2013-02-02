#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "include/m_function.h"

// Main
int main(int argc, char *argv[])
{
	char c;
	int (*towhich)(int c);

	if (m_strpos(argv[0], "toupper") > -1)
		towhich = toupper;
	else
		towhich = tolower;

	while ((c = getchar()) != EOF)
		putchar(towhich(c));

	return 0;
}
