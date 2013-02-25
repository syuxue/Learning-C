#include <string.h>
#include "include/m_function.h"

/* ****************************** Testing ****************************** */
int
is_gbk(unsigned char high, unsigned low)
{
	return 0;
}

int
is_gb2312(unsigned char high, unsigned low)
{
	// reference: http://zh.wikipedia.org/wiki/GB_2312
	if (high >= 0xA1 && high <= 0xF7
		&& low >= 0xA1 && low <= 0xFE) {
		return high <= 0xD7 ? 1 : 2;
	} else
		return 0;
}

int
is_ptr_gb2312(unsigned char *p)
{
	return is_gb2312(*p, *(p + 1));
}

/* ****************************** Main ****************************** */
int
main(int argc, char *argv[])
{
	int nconv;
	char str_utf[] = "鏖", str_gbk[1024];

	// UTF-8
	printf("UTF-8: (%u) %s\n", (unsigned int) strlen(str_utf), str_utf);

	nconv = m_iconv("UTF-8", str_utf, strlen(str_utf),
					"GBK", str_gbk, 2);
	M_printd(nconv);

	// GBK
	//printf("  GBK: (%u) %s\n", (unsigned int) strlen(str_gbk), str_gbk);
	int i;
	for (i = 0; i < nconv; i++) {
		printf("%u, ", (unsigned char) str_gbk[i]);
		if ((i + 1) % 10 == 0)
			putchar('\n');
	}
	putchar('\n');
	M_printd(is_ptr_gb2312((unsigned char *) str_gbk));

	return 0;
}
