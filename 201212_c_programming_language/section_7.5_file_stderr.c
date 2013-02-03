#include <stdio.h>
#include <stdlib.h>
#include "include/m_function.h"

// Main
int main(int argc, char *argv[])
{
	int i, c, filenumber;
	char *filelist[argc - 1];
	FILE *fp;

	// 获取文件名列表
	filenumber = argc - 1;
	for (i = 0; i < filenumber; i++) {
		filelist[i] = argv[i + 1];
	}

	// 输出文件内容
	for (i = 0; i < filenumber; i++) {
		if ((fp = fopen(filelist[i], "r")) != NULL) {
			M_print("%s", filelist[i]);
			while ((c = getc(fp)) != EOF)
				putchar(c);
			fclose(fp);
		} else {
			fprintf(stderr, "%s: can't open %s\n", argv[0], filelist[i]);
			exit(1);
		}
		if (ferror(stdout)) {
			fprintf(stderr, "%s: error writing stdout\n", argv[0]);
			exit(2);
		}
	}

	return 0;
}
