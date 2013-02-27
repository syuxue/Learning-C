#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pinyin.h"

#define TABLE_LINESIZE 128
enum {
	NON_GB = 0,
	GB2312_LEVEL_1, GB2312_LEVEL_2,
	GBK_LEVEL_3, GBK_LEVEL_4, GBK_LEVEL_5,
	GBK_USERDEF
};

static int
py_getpinyin_bsearch(const void *keyval, const void *datam)
{
	return (int) *(GBCODE *) keyval - ((PYNODE *) datam)->code;
}

static size_t
py_getpinyin_firstlen(const char *pinyin)
{
	const char *p;
	for (p = pinyin; *p != '\0' && *p != ' '; p++)
		;
	return (size_t) (p - pinyin);
}

PYTABLE *
py_open(const char *tablefile)
{
	PYTABLE *ptable;
	PYNODE *pnode;
	GBCODE last;
	FILE *fp;
	int linenum, len;
	char line[TABLE_LINESIZE];

	// Open table file
	if ((fp = fopen(tablefile, "r")) == NULL)
		return NULL;

	// Alloc
	if ((ptable = malloc(sizeof (PYTABLE))) == NULL)
		return NULL;

	// Count	
	for (linenum = 0; fgets(line, TABLE_LINESIZE, fp); linenum++)
		;
	if ((ptable->c2p_head = malloc(linenum * sizeof (PYNODE))) == NULL)
		return NULL;
	ptable->c2p_length = linenum;

	// Store
	fseek(fp, 0, SEEK_SET);
	for (last = 0, pnode = ptable->c2p_head; fgets(line, TABLE_LINESIZE, fp); pnode++) {
		pnode->code = py_getcode(line);
		if (pnode->code <= last)
			return NULL;

		pnode->pinyin = strdup(strchr(line, ' ') + 1);

		// Stripe new line
		len = strlen(pnode->pinyin);
		if (*(pnode->pinyin + len - 1) == '\n')
			*(pnode->pinyin + len - 1) = '\0';

		last = pnode->code;
	}

	return ptable;
}

void
py_close(PYTABLE *ptable)
{
	PYNODE *pnode;

	for (pnode = ptable->c2p_head; pnode < ptable->c2p_head + ptable->c2p_length; pnode++)
		free(pnode->pinyin);
	free(ptable->c2p_head);
	free(ptable);
}

int
py_isgbk(const char *str)
{
	return py_isgbk_func((GBCHAR) *str, (GBCHAR) *(str + 1));
}

int
py_isgbk_func(GBCHAR high, GBCHAR low)
{
	if (!py_isgbk_high(high))
		return NON_GB; // fast return

	/**/ if (high >= 0xA1 && high <= 0xA9 && low >= 0xA1 && low <= 0xFE)
		return GB2312_LEVEL_1;
	else if (high >= 0xB0 && high <= 0xF7 && low >= 0xA1 && low <= 0xFE)
		return GB2312_LEVEL_2;
	else if (high >= 0x81 && high <= 0xA0 && low >= 0x40 && low <= 0xFE && low != 0x7F)
		return GBK_LEVEL_3;
	else if (high >= 0xAA && high <= 0xFE && low >= 0x40 && low <= 0xA0 && low != 0x7F)
		return GBK_LEVEL_4;
	else if (high >= 0xA8 && high <= 0xA9 && low >= 0x40 && low <= 0xA0 && low != 0x7F)
		return GBK_LEVEL_5;
	else if (high >= 0xAA && high <= 0xAF && low >= 0xA1 && low <= 0xFE)
		return GBK_USERDEF;
	else if (high >= 0xF8 && high <= 0xFE && low >= 0xA1 && low <= 0xFE)
		return GBK_USERDEF;
	else if (high >= 0xA1 && high <= 0xA7 && low >= 0x40 && low <= 0xA0 && low != 0x7F)
		return GBK_USERDEF;
	else
		return NON_GB;
}

GBCODE
py_getcode(const char *str)
{
	return py_getcode_func((GBCHAR) *str, (GBCHAR) *(str + 1));
}

GBCODE
py_getcode_func(GBCHAR high, GBCHAR low)
{
	return (GBCODE) (high << 8) + low;
}

char *
py_getpinyin(const char *str, const PYTABLE *ptable)
{
	return py_getpinyin_func((GBCHAR) *str, (GBCHAR) *(str + 1), ptable);
}

char *
py_getpinyin_func(GBCHAR high, GBCHAR low, const PYTABLE *ptable)
{
	PYNODE *pnode;
	GBCODE code;

	code = py_getcode_func(high, low);
	pnode = bsearch(
		&code,
		ptable->c2p_head,
		ptable->c2p_length,
		sizeof (PYNODE),
		py_getpinyin_bsearch
	);

	return pnode != NULL ? pnode->pinyin : NULL;
}

size_t
py_convstr(char **inbuf, size_t *inleft, char **outbuf, size_t *outleft,
	unsigned int flag, const PYTABLE *ptable)
{
	char *pinyin, *p;
	size_t inlen, outlen;

	while (*inleft > 0) {

		pinyin = NULL;
		if (*inleft > 1 && py_isgbk(*inbuf) != NON_GB) {
			inlen = 2;
			if ((pinyin = py_getpinyin(*inbuf, ptable)) != NULL) {
				outlen = flag & PY_INITIAL ? 1 : py_getpinyin_firstlen(pinyin);
			} else {
				outlen = 2;
			}
		} else if (*inleft == 1 && py_isgbk_high(**inbuf)) {
			break;
		} else {
			inlen = outlen = 1;
		}

		if (outlen > *outleft)
			return (size_t) -1L;

		if (pinyin == NULL) {
			strncat(*outbuf, *inbuf, outlen);
			*outbuf += outlen;
		} else {
			*((*outbuf)++) = flag & PY_UPPER_I ? toupper(*pinyin) : *pinyin;
			for (p = pinyin + 1; p < pinyin + outlen; p++, (*outbuf)++) {
				**outbuf = flag & PY_UPPER_T ? toupper(*p) : *p;
			}
		}

		*inleft -= inlen;
		*inbuf += inlen;
		*outleft -= outlen;
	}

	return (size_t) 0;
}
