#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exhaustive(int length, char *ctable)
{
	int i, base, pos[length];

	// 初始化参数
	base = strlen(ctable);
	for (i = 0; i < length; i++)
		pos[i] = 0;

	while(1) {
		// 输出当前字符
		for (i = length - 1; i >= 0; i--)
			putchar(*(ctable + pos[i]));
		putchar('\n');

		// 升位
		pos[0]++;
		for (i = 0; i < length; i++) {
			if (pos[i] >= base) {
				if (i == length - 1)
					return;
				pos[i] = 0;
				pos[i + 1]++;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	// 检查参数
	if (argc != 3) {
		printf("********** Password Dictionary Generator **********\n");
		printf("error: Wrong aguments\n");
		printf("usage: %s {length} {chartable}\n", argv[0]);
		return 1;
	}

	// 生成
	exhaustive(atoi(argv[1]), argv[2]);

	return 0;
}
