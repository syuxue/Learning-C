#include <stdio.h>
#include <stdlib.h>
#include "include/m_function.h"

int main(void)
{
	unsigned long ul, last;

	for (last = 0, ul = 1; ul > last; ul *= 10) {
		printf("%016lx\n", ul);
		last = ul;
	}

	printf("%lu\n", 0x8ac7230489e80000);

	return 0;
}