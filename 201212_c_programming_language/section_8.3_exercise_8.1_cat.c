#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 4096

// Use read, open, close in syscalls.h rewrite the program Cat
int main(int argc, char *argv[])
{
	int i, filenum, n;
	char *filelist[argc - 1], buf[BUFF_SIZE];
	int fd;

	// 获取文件名列表
	filenum = argc - 1;
	for (i = 0; i < filenum; i++) {
		filelist[i] = argv[i + 1];
	}

	// 输出文件内容
	for (i = 0; i < filenum; i++) {
		if ((fd = open(filelist[i], O_RDONLY, 0)) == -1)
			exit(1);

		while ((n = read(fd, buf, BUFF_SIZE)) > 0) {
			if (write(1, buf, n) != n)
				exit(1);
		}
		write(1, "\n", 1);
	}

	close(fd);

	return 0;
 }
