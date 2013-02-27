#include "pinyin.h"
#include <string.h>
#include <stdlib.h>
#include "../include/m_function.h"

#define LINE_SIZE 4096

enum {
	NON_GBK = 0,
	GBK_LEVEL_3, GBK_LEVEL_4, GBK_LEVEL_5,
	GB2312_LEVEL_1, GB2312_LEVEL_2
};

#define isgbk_ptr(p) isgbk(*p, *(p + 1))
int
isgbk(unsigned char high, unsigned char low)
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

static pynode **table_c2p;
static int table_c2p_len;

gbcode
getcode(gbchar high, gbchar low)
{
	return (unsigned int) (high << 8) + low;
}

void
table_init_c2p()
{
	FILE *fp;
	char line[LINE_SIZE];
	pynode **pt;

	// Count table
	if ((fp = fopen(PINYIN_TABLE, "r")) == NULL)
		; // TODO
	for (table_c2p_len = 0; fgets(line, LINE_SIZE, fp); table_c2p_len++)
		;

	// Alloc
	if ((table_c2p = malloc(sizeof table_c2p * table_c2p_len)) == NULL)
		; // TODO

	// Store table
	fseek(fp, 0, SEEK_SET);
	for (pt = table_c2p; fgets(line, LINE_SIZE, fp); pt++) {
		*pt = malloc(sizeof (pynode));
		(*pt)->code = getcode(line[0], line[1]);
		(*pt)->pinyin = strdup(strchr(line, ' ') + 1);
	}
}

char *
convchar(gbchar high, gbchar low)
{
	unsigned int incode;
	pynode **pt;

	incode = getcode(high, low);

	for (pt = table_c2p; pt < table_c2p + table_c2p_len; pt++) {
		if ((*pt)->code == incode)
			break;
	}

	return (*pt)->pinyin;
}

