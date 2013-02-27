#include "../include/m_function.h"
#include <string.h>
#include "pinyin.h"

int main(int argc, char *argv[])
{
	PYTABLE *pt;

	pt = py_open("table_gbk.gb");

	char str_utf[] = "你好，世界！", str_gbk[1024];
	m_iconv("UTF-8", str_utf, strlen(str_utf),
			"GBK", str_gbk, sizeof str_gbk);
	printf("%s: %s\n", str_utf, py_convchar(str_gbk, pt));

	py_close(pt);

	return 0;
}
