#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define INTERVAL 250000
#define BUFSIZ 4096

int main(int argc, char *argv[])
{
	int n, fd;
	char buf[BUFSIZ];

	if ((fd = open(argv[1], O_RDONLY, 0)) == -1)
		return 1;
	if (lseek(fd, 0L, 2) == -1)
		return 1;

	while (1) {
		n = read(fd, buf, BUFSIZ);
		if (n)
			write(1, buf, n);
		else
			usleep(INTERVAL);
	}


	return 0;
}
