#include <stdio.h>
#include <stdlib.h>
#include "include/m_function.h"
//#define BENCH

// Use getc, fopen, fclose in stdio.h write the program Cat
int main(int argc, char *argv[])
{
	int i, c, filenum, linenum;
	char *filelist[argc - 1];
	FILE *fp;

	// 获取文件名列表
	filenum = argc - 1;
	for (i = 0; i < filenum; i++) {
		filelist[i] = argv[i + 1];
	}

	// 输出文件内容
	for (i = 0; i < filenum; i++) {
		if ((fp = fopen(filelist[i], "r")) == NULL) {
			fprintf(stderr, "%s: can't open %s\n", argv[0], filelist[i]);
			exit(1);
		}

#ifdef BENCH
		while ((c = getc(fp)) != EOF)
			;
#else
		linenum = 0;
		printf(M_bash_magenta "********** %d/%d %s **********\n" M_bash_default, i + 1, filenum, filelist[i]);
		printf(M_bash_green "%-4d" M_bash_cyan ": " M_bash_default, ++linenum);
		while ((c = getc(fp)) != EOF) {
			putchar(c);
			if (c == '\n')
				printf(M_bash_green "%-4d" M_bash_cyan ": " M_bash_default, ++linenum);
		}
		putchar('\n');
#endif
		
		fclose(fp);

		if (ferror(stdout)) {
			fprintf(stderr, "%s: error writing stdout\n", argv[0]);
			exit(2);
		}
	}

	return 0;
}
