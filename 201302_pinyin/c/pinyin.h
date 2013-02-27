// Type
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

// Function Declaration
PYTABLE *
py_open(const char *tablefile);

void
py_close(PYTABLE *ptable);

GBCODE
py_getcode(const char *str);

GBCODE
py_getcode_func(GBCHAR high, GBCHAR low);

char *
py_convchar(const char *str, const PYTABLE *ptable);

char *
py_convchar_func(GBCHAR high, GBCHAR low, const PYTABLE *ptable);
