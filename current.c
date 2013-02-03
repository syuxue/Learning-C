#include <stdio.h>
#include "include/m_function.h"
#define FILE_NUM 2
#define LINE_SIZE 4096

int main(int argc, char *argv[])
{
	int i;
	char *Lfile[FILE_NUM], Lline[FILE_NUM][LINE_SIZE];
	FILE *Lfp[FILE_NUM];

	if (argc != 3) {
		fprintf(stderr, "error: arguments wrong\n");
		return 1;
	}

	// 打开文件句柄
	for (i = 0; i < FILE_NUM; i++) {
		Lfile[i] = argv[i + 1];
		Lfp[i] = fopen(Lfile[i], "r");
	}
	printf(M_bash_CYAN "%s" M_bash_YELLOW " %s\n" M_bash_default, Lfile[0], Lfile[1]);

	while (1) {
		for (i = 0; i < FILE_NUM; i++) {
			fgets(Lline[i], LINE_SIZE, Lfp[i]);
			M_print("%s", Lline[i]);
		}
	}

	// 关闭文件句柄
	for (i = 0; i < FILE_NUM; i++) {
		fclose(Lfp[i]);
	}

	return 0;
}
