#include "section_8.7_malloc.h"
#include "include/m_function.h"
#include <stdio.h>

/* ****************************** Standard ****************************** */
static Header base;				// empty list to get started
static Header *freep = NULL;	// start of free list

/* morecore: ask system for more memory */
/* nu: number of Headers */
static Header *morecore(unsigned int nu)
{
	char *cp;
	Header *hp;

	if (nu < NALLOC)
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if (cp == (char *) -1)		// sbrk returs -1 when error
		return NULL;
	
	hp = (Header *) cp;
	hp->s.size = nu;
	
	m_free((void *) (hp + 1));
	return freep;
}

void m_free(void *ap)
{
	Header *hp, *p;

	hp = (Header *) ap - 1;		// pointer to this header
	for (p = freep; hp >= p->s.ptr || hp <= p; p = p->s.ptr)
		if (p >= p->s.ptr && (hp > p || hp < p->s.ptr))
			break; /* freed block at start or end of arena */

	if (hp + hp->s.size == p->s.ptr) { /* join to upper nbr */
		hp->s.size += p->s.ptr->s.size;
		hp->s.ptr = p->s.ptr->s.ptr;
	} else
		hp->s.ptr = p->s.ptr;

	if (p + p->s.size == hp) { /* join to lower nbr */
		p->s.size += hp->s.size;
		p->s.ptr = hp->s.ptr;
	} else
		p->s.ptr = hp;

	freep = p;
}

void *m_malloc(unsigned int nbytes)
{
	Header *p, *prevp;
	unsigned int nunits;

	// ceil(nbytes / size) + 1
	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

	// No free list yet
	if ((prevp = freep) == NULL) {
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}

	// Find slot
	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) { /* big enough */
			if (p->s.size == nunits) /* exactly */
				prevp->s.ptr = p->s.ptr;
			else { /* allocate tail end */
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p + 1);
		}
		if (p == freep) /* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL; /* none left */
	}
}

/* ****************************** debug ****************************** */
void showalloc(void)
{
	Header *p, *origin;

	p = &base;

	printf(M_bash_RED "BASE " M_bash_default);
	for (origin = p; p != NULL; p = p->s.ptr) {
		printf(M_bash_YELLOW "%p" M_bash_default "(" M_bash_CYAN "%u" M_bash_default ") -> ", p, p->s.size);
		if (p->s.ptr == origin)
			break;
	}
	printf(M_bash_RED "%s" M_bash_default,	(p == NULL) ? "NULL" : 
											(p->s.ptr == origin) ? "BASE" 
											: "????");
	printf("\n");
}
