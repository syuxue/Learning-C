#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pinyin.h"

#define PYLINESIZ 128

static int
py_convchar_bsearch(const void *keyval, const void *datum)
{
	return (int) *(GBCODE *) keyval - ((PYNODE *) datum)->code;
}

PYTABLE *
py_open(const char *tablefile)
{
	PYTABLE *ptable;
	PYNODE *pnode;
	GBCODE last;
	FILE *fp;
	int linenum, len;
	char line[PYLINESIZ];

	// Open table file
	if ((fp = fopen(tablefile, "r")) == NULL)
		return NULL;

	// Alloc
	if ((ptable = malloc(sizeof (PYTABLE))) == NULL)
		return NULL;

	// Count	
	for (linenum = 0; fgets(line, PYLINESIZ, fp); linenum++)
		;
	if ((ptable->c2p_head = malloc(linenum * sizeof (PYNODE))) == NULL)
		return NULL;
	ptable->c2p_length = linenum;

	// Store
	fseek(fp, 0, SEEK_SET);
	for (last = 0, pnode = ptable->c2p_head; fgets(line, PYLINESIZ, fp); pnode++) {
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
py_convchar(const char *str, const PYTABLE *ptable)
{
	return py_convchar_func((GBCHAR) *str, (GBCHAR) *(str + 1), ptable);
}

char *
py_convchar_func(GBCHAR high, GBCHAR low, const PYTABLE *ptable)
{
	PYNODE *pnode;
	GBCODE code;

	code = py_getcode_func(high, low);
	pnode = bsearch(
		&code,
		ptable->c2p_head,
		ptable->c2p_length,
		sizeof (PYNODE),
		py_convchar_bsearch
	);

	return pnode != NULL ? pnode->pinyin : NULL;
}
