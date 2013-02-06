#include "section_8.5_file.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int debug_getc(char *filename)
{
	int c;
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return 1;

	getc(fp);
	//fseek(fp, 10L, SEEK_SET);
	fseek(fp, 9L, SEEK_CUR);
	while ((c = getc(fp)) != EOF) {
		putc(c, stdout);
		putc('.', stderr); // 模拟读取
		usleep(2000);
	}

	putc('\n', stdout);
	return fclose(stdout);
}

int debug_putc(char *filename)
{
	int i, j;
	FILE *fp;

	fp = fopen(filename, "w");
	if (fp == NULL)
		return 1;

	for (i = 0; i < 26; i++)
		putc(i + 'A', fp);
	putc('\n', fp);
	fseek(fp, -17L, SEEK_CUR);
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			putc((i + j) % 10 + '0', fp);
		}
		putc('\n', fp);
	}

	fclose(fp);

	// System cat
	char cmd[1024];
	strcpy(cmd, "cat ");
	strcat(cmd, filename);
	system(cmd);

	return 0;
}

int main(int argc, char *argv[])
{
	//debug_getc(argv[1]);
	debug_putc(argv[1]);

	return 0;
}
