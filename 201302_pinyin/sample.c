#include "../include/m_function.h"
#include "pinyin.h"

int main(int argc, char *argv[])
{
//	int nconv;
//	char str_utf[] = "°¡", str_gbk[1024];

//	if (0) {
//		// UTF-8
//		printf("UTF-8: (%u) %s\n", (unsigned int) strlen(str_utf), str_utf);
//
//		nconv = m_iconv("UTF-8", str_utf, strlen(str_utf),
//						"GBK", str_gbk, sizeof str_gbk);
//		M_printd(nconv);
//
//		// GBK
//		//printf("  GBK: (%u) %s\n", (unsigned int) strlen(str_gbk), str_gbk);
//		int i;
//		for (i = 0; i < nconv; i++) {
//			printf("%u, ", (gbchar) str_gbk[i]);
//			if ((i + 1) % 10 == 0)
//				putchar('\n');
//		}
//		putchar('\n');
//		M_printd(isgbk_ptr((gbchar *) str_gbk));
//	}

	//char gbchar[] = "°¡";

	PYTABLE *pyt;
	pyt = py_open("table_gbk.gb");
	M_print("%p", pyt);

M_printu(pyt->c2p_tail - pyt->c2p_head);
M_print("%u", pyt->c2p_head->code);
M_print("%s", pyt->c2p_head->pinyin);
M_print("%u", pyt->c2p_tail->code);
M_print("%s", pyt->c2p_tail->pinyin);

//char str[] = {176, 161, 0};
char str[] = {0xd5, 0xdb, 0};
{
	int i;
	for (i = 0; i < 50000; i++)
		py_convchar(str, pyt);
}
M_print("[%s]", py_convchar(str, pyt));

	return 0;
}
