#include <stdio.h>
#include <string.h>
#define LINE_SIZE	4096
#define EXT_LENGTH		20

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	FILE *fp;
	char line[LINE_SIZE + 1], *ext;

	// 检查参数
	if (argc < 2) {
		fprintf(stderr, "error: empty argument\n");
		return 1;
	}

	// 扫描文件
	while (--argc) {
		++argv;

		if ((fp = fopen(*argv, "r")) == NULL)
			continue;

		while (fgets(line, LINE_SIZE, fp)) {
			if ((ext = strstr(line, "ext=")) == NULL)
				continue;

			ext += 4;
			*(ext + EXT_LENGTH) = '\0';
			printf("%s\n", ext);
		}
	}

	return 0;
}
