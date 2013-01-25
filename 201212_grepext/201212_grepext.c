#include <stdio.h>
#include "m_function.h"
#define EXTSIZE		20
#define LINESIZE	4096

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int i;
	char line[LINESIZE], *p;

	while (m_getline(line, LINESIZE) > 0) {
		for (p = line + m_strpos(line, "ext=") + 4,i = 0; i < EXTSIZE; i++) {
			printf("%c", *(p + i));
		}
		printf("\n");
	}

	return 0;
}
