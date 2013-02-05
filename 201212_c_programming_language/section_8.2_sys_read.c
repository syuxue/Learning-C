#include <stdio.h>
#include <unistd.h>
#include "include/m_function.h"

#undef BUFSIZ
#define BUFSIZ 10

int m_getchar(void)
{
	char c;

	return read(0, &c, 1) == 1 ? (unsigned char) c : EOF;
}

// 当read返回< BUFSIZ时，函数返回EOF
int m_getchar_buff(void)
{
	static char buff[BUFSIZ];
	static int p = BUFSIZ, len;

	if (p == BUFSIZ) {
		len = read(0, buff, BUFSIZ);
		M_printd(len);
		p = 0;
	}

	return p < len ? (unsigned char) buff[p++] : EOF;
}

// 当read返回0时，函数返回EOF
int m_getchar_buff2(void)
{
	static char buf[BUFSIZ];
	static char *bufp = buf;
	static int n = 0;
	if (n == 0) {
		n = read(0, buf, sizeof buf);
		M_printd(n);
		bufp = buf;
	}

	return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

int main(int argc, char *argv[])
{
	int c;

	while ((c = m_getchar_buff2()) != EOF) {
		printf("[%d]\n", c);
		//putchar(c);
	}

	return 0;
}
