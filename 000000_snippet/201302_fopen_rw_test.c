#include <stdio.h>
#include <ctype.h>
#include "include/m_function.h"

// 关于fopen中r+的测试
// 把文件的内容删掉，只留下形状。。。
int main(void)
{
	int c;
	FILE *fp;

	fp = fopen("t.c", "r+");
	while (c != EOF) {
		while ((c = fgetc(fp)) != EOF && (c == '*' || isspace(c)))
			;
		fseek(fp, -1L, SEEK_CUR);
		fputc('*', fp);
	}
	fclose(fp);

	return 0;
}
