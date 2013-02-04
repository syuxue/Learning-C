#include <stdio.h>
#include <string.h>
#include "include/m_function.h"
#define FILE_NUM 2
#define LINE_SIZE 4096

int main(int argc, char *argv[])
{
	int i, linenum;
	char *Lfile[FILE_NUM], Lline[FILE_NUM][LINE_SIZE], *Lstatus[FILE_NUM];
	FILE *Lfp[FILE_NUM];

	if (argc != 3) {
		fprintf(stderr, "error: arguments wrong\n");
		return 1;
	}

	// 打开文件句柄
	for (i = 0; i < FILE_NUM; i++) {
		Lfile[i] = argv[i + 1];
		if ((Lfp[i] = fopen(Lfile[i], "r")) == NULL) {
			fprintf(stderr, "error: can not open %s\n", Lfile[i]);
			return 1;
		}
	}
	printf(M_bash_CYAN "%s" M_bash_YELLOW " %s\n" M_bash_default, Lfile[0], Lfile[1]);

	// 找到第一行不同
	for (linenum = 1; ; linenum++) {
		Lstatus[0] = fgets(Lline[0], LINE_SIZE, Lfp[0]);
		Lstatus[1] = fgets(Lline[1], LINE_SIZE, Lfp[1]);
		if (strcmp(Lline[0], Lline[1]) != 0) { // 两行不同
			printf("%d:" M_bash_CYAN "%s" M_bash_default "%d:" M_bash_YELLOW "%s\n" M_bash_default, linenum, Lline[0], linenum, Lline[1]);
			break;
		}
		if (Lstatus[0] == NULL || Lstatus[1] == NULL)
			break;
	}

	// 关闭文件句柄
	for (i = 0; i < FILE_NUM; i++) {
		fclose(Lfp[i]);
	}

	return 0;
}
