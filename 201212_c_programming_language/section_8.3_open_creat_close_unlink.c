#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include "include/m_function.h"

#define PERMS 0644

void error(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

int main(int argc, char *argv[])
{
	int fd_from, fd_to, n;
	char buf[BUFSIZ];

	if (argc != 3)
		error("Usage: cp from to");

	if ((fd_from = open(argv[1], O_RDONLY, 0)) == -1)
		error("cp: can't open %s", argv[1]);

	if ((fd_to = creat(argv[2], PERMS)) == -1)
		error("cp: can't create %s, mode %03o", argv[2], PERMS);

	while ((n = read(fd_from, buf, BUFSIZ)) > 0)
		if (write(fd_to, buf, n) != n)
			error("cp: write error on file %s", argv[2]);

	return 0;
}
