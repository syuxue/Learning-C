#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define WORDSIZE 128
#define NKEYS sizeof keytab / sizeof keytab[0]
struct key {
	char *word;
	int count;
} keytab[] = {
	{"auto", 0},
	{"break", 0},
	{"case", 0},
	{"char", 0},
	{"const", 0},
	{"continue", 0},
	{"default", 0},
	{"do", 0},
	{"double", 0},
	{"else", 0},
	{"enum", 0},
	{"extern", 0},
	{"float", 0},
	{"for", 0},
	{"goto", 0},
	{"if", 0},
	{"int", 0},
	{"long", 0},
	{"register", 0},
	{"return", 0},
	{"short", 0},
	{"signed", 0},
	{"sizeof", 0},
	{"static", 0},
	{"struct", 0},
	{"switch", 0},
	{"typedef", 0},
	{"union", 0},
	{"unsigned", 0},
	{"void", 0},
	{"volatile", 0},
	{"while", 0}
};


/* ****************************** Function ****************************** */
int getword(char *word)
{
	char c, *w = word;

	*w = '\0';
	while (isspace(c = getchar()))
		;
	if (c == EOF) {
		return EOF;
	} else if (c == '/') {
		if ((c = getchar()) == '/') {
			while (getchar() != '\n')
				;
			return '/';
		} else if (c == '*') {
			while ((c = getchar()) != '*' || (c = getchar()) != '/')
				;
			return '*';
		} else {
			ungetc(c, stdin);
			*w++ = '/';
			*w = '\0';
			return '/';
		}
	} else if (c == 39) {
		while (getchar() != 39)
			;
		return 39;
	} else if (c == '"') {
		while (getchar() != '"')
			;
		return '"';
	} else if (!isalpha(c)) {
		*w++ = c;
		*w = '\0';
		return '\0';
	} else
		*w++ = c;

	// Alpha
	while (isalnum(c = getchar()) || c == '_')
		*w++ = c;
	ungetc(c, stdin);
	*w = '\0';

	return *word;
}

struct key *
binsearch(char *word, struct key tab[], int n)
{
	int cond;
	struct key *low, *high, *mid;
	
	low = &tab[0];
	high = &tab[n - 1];
	while (low <= high) {
		mid = low + (high - low) / 2;
		//printf("%2d - %2d m: %2d\n", low - keytab, high - keytab, mid - keytab);
		if ((cond = strcmp(word, mid->word)) > 0)
			low = mid + 1;
		else if (cond < 0)
			high = mid - 1;
		else
			return mid;
	}

	return NULL;
}

/* ****************************** Main ****************************** */
int main(void)
{
	char word[WORDSIZE];
	struct key *p;

	M_printd(sizeof (struct key));
	return 0;

	// Get words
	while (getword(word) != EOF)
		if (isalpha(word[0]))
			if ((p = binsearch(word, keytab, NKEYS)))
				p->count++;

	// Output keytab
	for (p = &keytab[0]; p < keytab + NKEYS; p++) {
		if (p->count > 0)
			printf("%8s: %4d\n", p->word, p->count);
	}

	return 0;
}
