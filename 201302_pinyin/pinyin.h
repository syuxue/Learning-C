// Type
typedef unsigned char GBCHAR;

typedef unsigned int GBCODE;

typedef struct {
	GBCODE code;
	char *pinyin;
} PYNODE;

typedef struct {
	PYNODE *c2p_head;
	PYNODE *c2p_tail;
} PYTABLE;

// Function Declaration
PYTABLE *
py_open(const char *tablefile);

GBCODE
py_getcode(const char *str);

GBCODE
py_getcode_func(GBCHAR high, GBCHAR low);

char *
py_convchar(const char *str, const PYTABLE *ptable);

char *
py_convchar_func(GBCHAR high, GBCHAR low, const PYTABLE *ptable);
