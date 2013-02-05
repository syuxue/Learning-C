#include "section_8.5_file.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
	int c;
	FILE *fp;

	// 获取文件指针
	fp = fopen(argv[1], "r");
	if (fp == NULL)
		return 1;

	while ((c = getc(fp)) != EOF) {
		putc(c, stdout);
		putc('.', stderr); // 模拟读取
		usleep(10000);
	}

	putc('\n', stdout);
	fclose(stdout);

	return 0;
}
