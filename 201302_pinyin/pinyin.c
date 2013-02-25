#include <string.h>
#include "include/m_function.h"

/* ****************************** Testing ****************************** */
enum {
	NON_GBK = 0,
	GBK_LEVEL_3, GBK_LEVEL_4, GBK_LEVEL_5,
	GB2312_LEVEL_1, GB2312_LEVEL_2
};

#define isgbk_ptr(p) isgbk(*p, *(p + 1))
int
isgbk(unsigned char high, unsigned low)
{
	if (high < 0x81)
		return NON_GBK; // fast return

	if (high >= 0xA1 && high <= 0xF7 && low >= 0xA1 && low <= 0xFE) {
		return high <= 0xD7 ? GB2312_LEVEL_1 : GB2312_LEVEL_2;
	} else if (high >= 0x81 && high <= 0xA0 && low >= 0x40 && low <= 0xFE) {
		return GBK_LEVEL_3;
	} else if (high >= 0xAA && high <= 0xFE && low >= 0x40 && low <= 0xA0) {
		return GBK_LEVEL_4;
	} else if (high >= 0xA8 && high <= 0xA9 && low >= 0x40 && low <= 0xA0) {
		return GBK_LEVEL_5;
	} else
		return NON_GBK;
}

/* ****************************** Search ****************************** */


/* ****************************** Main ****************************** */
int
main(int argc, char *argv[])
{
	int nconv;
	char str_utf[] = "翯", str_gbk[1024];

	// UTF-8
	printf("UTF-8: (%u) %s\n", (unsigned int) strlen(str_utf), str_utf);

	nconv = m_iconv("UTF-8", str_utf, strlen(str_utf),
					"GBK", str_gbk, sizeof str_gbk);
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
	M_printd(isgbk_ptr((unsigned char *) str_gbk));

	return 0;
}
