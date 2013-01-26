#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "include/m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define HASH_SIZE 8
#define WORD_LENGTH 4096
struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};
static struct nlist *hashtab[HASH_SIZE];

/* ****************************** Function ****************************** */
int getword(char *word)
{
	char c, type;

	// Skip space
	while (isspace(c = getchar()))
		;

	// Get word
	if (c == EOF) { // EOF
		type = EOF;
	} else if (isalpha(c)) { // Alpha
		type = *(word++) = c;
		while (isalnum(c = getchar()) || c == '_')
			*(word++) = c;
		ungetc(c, stdin);
	}  else if (isdigit(c)) { // Digit
		type = *(word++) = c;
		while (isdigit(c = getchar()) || c == '.')
			*(word++) = c;
		ungetc(c, stdin);
	} else if (c == 39) { // Single quote
		type = *(word++) = c;
		while ((*(word++) = getchar()) != 39)
			;
	} else if (c == '"') { // Double quote
		type = *(word++) = c;
		while ((*(word++) = getchar()) != '"')
			;
	} else if (c == '#') { // Preprocess
		type = *(word++) = c;
		while ((c = getchar()) != '\n') {
			*(word++) = c;
			if (c == '\\')
				*(word++) = getchar();
		}
	} else if (c == '/') { // Comment
		type = *(word++) = c;
		if ((c = getchar()) == '/') { // Single line
			*(word++) = c;
			while ((*(word++) = getchar()) != '\n')
				;
			word--;
		} else if (c == '*') { // Multi line
			*(word++) = c;
			while ((c = getchar())) {
				*(word++) = c;
				if (c == '*' && (*(word++) = getchar()) == '/')
					break;
			}
		} else
			ungetc(c, stdin);
	} else {
		type = *(word++) = c;
	}
	*word = '\0';

	return type;
}

void hashtable_show(void)
{
	int i;
	struct nlist *np;

	for (i = 0; i < HASH_SIZE; i++) {
		printf(M_bash_YELLOW "hashtab[%d]: " M_bash_default, i);
		if (hashtab[i] != NULL)
			for (np = hashtab[i]; np != NULL; np = np->next)
				printf(M_bash_GREEN "%s" M_bash_default " -> ", np->name);
		printf(M_bash_RED "NULL\n" M_bash_default);
	}
}

unsigned hash(const char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s * 31 + hashval;
	return hashval % HASH_SIZE;
}

struct nlist parseword(const char *word)
{
	int ns;
	char *p;
	struct nlist nlist;

	nlist.next = NULL;

	// Name
	word++;
	while (isalpha(*word))
		word++;
	while (isspace(*word))
		word++;
	for (ns = 0, p = (char *)word; *p != '\0'; p++) {
		if (*p == '(')
			ns++;
		if (*p == ')')
			ns--;
		if (isspace(*p) && ns == 0)
			break;
	}
	nlist.name = strndup(word, p - word);
	word = p;

	// Define
	while (isspace(*word))
		word++;
	nlist.defn = strdup(word);

	return nlist;
}

struct nlist *lookup(const char *s)
{
	struct nlist *np;
	
	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

struct nlist *install(const char *name, const char *defn)
{
	unsigned hashval;
	struct nlist *np; 

	if ((np = lookup(name)) == NULL) {
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else
		free((void *) np->defn);
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

void undef(const char *s)
{
	struct nlist *np, *last;
	
	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			last->next = np->next;
			free((void *) np);
			return;
		}
		last = np;
	}
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char word[WORD_LENGTH];
	struct nlist nlist;

	// Install
	while (getword(word) != EOF) {
		if (word[0] == '#' && word[1] == 'd') {
			nlist = parseword(word);
			install(nlist.name, nlist.defn);
			free(nlist.name), free(nlist.defn);
		}
	}
	hashtable_show();

	return 0;
}
