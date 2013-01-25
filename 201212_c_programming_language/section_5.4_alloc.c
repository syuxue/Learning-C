#include <stdio.h>
#include <ctype.h>
#include "m_function.h"
#define ALLOCSIZE 102400
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;
char *alloc(int n);
void afree(char *p);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char garbage[] = "Pointers and integers are not interchangeable. Zero is the sole exception: the constant zeromay be assigned to apointer, and a pointer may be compared with the constant zero. Thesymbolic constant NULL is";
	char *p;

	p = alloc(m_strlen(garbage));
	m_strset(garbage, p);
	printf("%d\n%s\n",(int)(allocp - allocbuf) , p);
	afree(p);
	printf("%d\n%s\n",(int)(allocp - allocbuf) , p);

	p = alloc(10);
	m_strset("0123456789",p);
	printf("%d\n%s\n",(int)(allocp - allocbuf) , allocbuf);

	return 0;
}

char *alloc(int n)
{
	if (allocp - allocbuf + n < ALLOCSIZE) {
		allocp += n;
		return allocp - n;
	} else {
		return 0;
	}
}

void afree(char *p)
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
		allocp = p;
}
