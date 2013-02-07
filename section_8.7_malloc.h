#include <unistd.h>

typedef long Align;				// for alignment to long boundary

union header {					// block header
	struct {
		union header *ptr;		// next block if on free list
		unsigned int size;		// size of this block
	} s;
	Align x;					// force alignment of blocks
};
typedef union header Header;

void *m_malloc(unsigned int nbytes);
void m_free(void *ap);
void showalloc(void);

#define NALLOC 100				// minimum #units to request
