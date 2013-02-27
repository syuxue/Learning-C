#define PY_UPPER_I 1
#define PY_UPPER_T 2
#define PY_INITIAL 4

typedef unsigned char GBCHAR;

typedef unsigned int GBCODE;

typedef struct {
	GBCODE code;
	char *pinyin;
} PYNODE;

typedef struct {
	PYNODE *c2p_head;
	int c2p_length;
} PYTABLE;

PYTABLE *
py_open(const char *tablefile);

void
py_close(PYTABLE *);

int
py_isgbk(const char *str);

#define py_isgbk_high(high) ((GBCHAR) high >= 0x81)

int
py_isgbk_func(GBCHAR high, GBCHAR low);

GBCODE
py_getcode(const char *str);

GBCODE
py_getcode_func(GBCHAR high, GBCHAR low);

char *
py_getpinyin(const char *str, const PYTABLE *);

char *
py_getpinyin_func(GBCHAR high, GBCHAR low, const PYTABLE *);

size_t
py_convstr(char **inbuf, size_t *inleft, char **outbuf, size_t *outleft,
	unsigned int flag, const PYTABLE *);
