#include "section_8.7_malloc.h"
#define STRACE_START write(2, "__strace_start__\n", 17)
#define STRACE_END write(2, "__strace_end__\n", 15)

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	STRACE_START;
	showalloc();

	/* code start */

	// Init
	int i, j, max = 1000;
	char *p[max];
	for (i = 0; i < max; i++)
		p[i] = NULL;
	i = 0;

	// Alloc
	for (j = 0; j < 10; j++)
		p[i++] = m_malloc(sizeof(Header) * 30);
	showalloc();

	m_free(p[1]);
	p[1] = NULL;
	showalloc();

	for (j = 0; j < 10; j++)
		p[i++] = m_malloc(sizeof(Header) * 10);
	showalloc();

	for (j = 0; j < 400; j++)
		p[i++] = m_malloc(sizeof(Header) * 1);
	showalloc();

	// Free
	for (i = 0; i < max; i++)
		if (p[i] != NULL)
			m_free(p[i]);

	/* code end */

	showalloc();
	STRACE_END;
	return 0;
}
