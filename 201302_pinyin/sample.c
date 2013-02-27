#include "../include/m_function.h"
#include <error.h>
#include <string.h>
#include "c/pinyin.h"

int main(int argc, char *argv[])
{
	PYTABLE *pt;
	char str_gbk[BUFSIZ], str_py[BUFSIZ];
	char str_utf[BUFSIZ] = "王雨晨";

	// Convert to GBK
	m_iconv("UTF-8", str_utf, strlen(str_utf), "GBK", str_gbk, sizeof str_gbk);

	// py_open
	if ((pt = py_open("table/gbk.gb")) == NULL) {
		fprintf(stderr, "error: can not open pinyin table\n");
		return 1;
	}

	// get Pinyin by string
	{
		int nconv;
		char *inbuf, *outbuf;
		size_t insize, outsize;
	
		inbuf = str_gbk, insize = strlen(str_gbk);
		outbuf = str_py, outsize = sizeof str_py;
printf("------------------------------------------------\n");
printf(" in: %p, %lu\nout: %p, %lu\n", inbuf, insize, outbuf, outsize);
		nconv = py_convstr(&inbuf, &insize, &outbuf, &outsize, PY_UPPER_I/* | PY_INITIAL*/, pt);
		if (nconv == (size_t) -1) {
			error(0, 0, "py_convstr: outbuf too small");
			return -1;
		}
printf(" in: %p, %lu\nout: %p, %lu\n", inbuf, insize, outbuf, outsize);
printf("nconv: %ld\n", (long) nconv);
printf("------------------------------------------------\n");
		m_iconv("GBK", str_py, strlen(str_py), "UTF-8", str_utf, sizeof str_utf);
printf("%s\n", str_utf);
	}

	// py_close
	py_close(pt);

	return 0;
}
