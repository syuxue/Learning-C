#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define WORD_LENGTH	4096
struct word {
	char content[WORD_LENGTH];
	unsigned int line;
};
struct lnode {
	unsigned int line;
	struct lnode *next;
	struct lnode *prev;
};
struct tnode {
	char *word;
	int count;
	struct lnode *firstline;
	struct lnode *lastline;
	struct tnode *left;
	struct tnode *right;
};

/* ****************************** Function ****************************** */
int getword(struct word *pword)
{
	static unsigned int line = 1;
	char c, type, *word;

	// Skip space
	while (isspace(c = getchar()))
		if (c == '\n')
			line++;

	word = pword->content;

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
				*(word++) = c = getchar();
			if (c == '\n')
				line++;
		}
		line++;
	} else if (c == '/') { // Comment
		type = *(word++) = c;
		if ((c = getchar()) == '/') { // Single line
			*(word++) = c;
			while ((*(word++) = getchar()) != '\n')
				;
			word--;
			line++;
		} else if (c == '*') { // Multi line
			*(word++) = c;
			while ((c = getchar())) {
				*(word++) = c;
				if (c == '\n')
					line++;
				if (c == '*' && (*(word++) = getchar()) == '/')
					break;
			}
		} else
			ungetc(c, stdin);
	} else {
		type = *(word++) = c;
	}
	*word = '\0';

	pword->line = line;

	return type;
}

void tnode_show(struct tnode *node)
{
	struct lnode *tmp;

	printf("%4d %s\n\t", node->count, node->word);
	for (tmp = node->firstline; tmp; tmp = tmp->next)
		printf("%u,", tmp->line);
	printf("\n");
}

struct lnode *line_add(unsigned linenum, struct lnode *prev)
{
	struct lnode *pline;
	pline = malloc(sizeof(struct lnode));
	pline->line = linenum;
	pline->prev = prev;
	if (prev)
		prev->next = pline;
	return pline;
}

void tree_print(struct tnode *p)
{
	if (p == NULL)
		return;
	tree_print(p->left);
	tnode_show(p);
	tree_print(p->right);
}

struct tnode *tree_add(struct tnode *p, struct word *pword, int *qtt)
{
	int cond;

	if (p == NULL) {
		p = (struct tnode *) malloc(sizeof(struct tnode));
		(*qtt)++;
		p->word = strdup(pword->content);
		p->count = 1;
		p->right = p->left = NULL;
		p->firstline = p->lastline = line_add(pword->line, NULL);
	} else if ((cond = strcmp(pword->content, p->word)) == 0) {
		p->count++;
		if (p->lastline->line != pword->line)
			p->lastline = line_add(pword->line, p->lastline);
	} else if (cond > 0) {
		p->right = tree_add(p->right, pword, qtt);
	} else if (cond < 0) {
		p->left = tree_add(p->left, pword, qtt);
	}

	return p;
}

struct tnode **tree_nodelist(struct tnode *root, int qtt)
{
	static struct tnode **node_list;
	struct tnode **node_stack, **pstack, **pnode, *pcur;

	// use Stack instead of Recursion
	if (!node_list) {
		pnode = node_list = malloc(sizeof(struct tnode *) * qtt);
		pstack = node_stack = malloc(sizeof(struct tnode *) * qtt);
		*pstack = root;
		while (pstack >= node_stack) {
			pcur = *(pnode++) = *(pstack--);
			if (pcur->right)
				*(++pstack) = pcur->right;
			if (pcur->left)
				*(++pstack) = pcur->left;
		}
		free(node_stack);
		node_stack = NULL;
	}
	return node_list;
}

int node_sort_by_count(const void *to , const void *from)
{
	return ((*(struct tnode **)to)->count - (*(struct tnode **)from)->count) * -1;
}

int node_sort_by_word(const void *to , const void *from)
{
	return strcmp((*(struct tnode **)to)->word, (*(struct tnode **)from)->word);
}

/* ****************************** Main ****************************** */
int main(void)
{
	int tree_qtt;
	struct word word;
	struct tnode *tree_root, **node_list, **pnode;

	// Add tree node
	tree_qtt = 0;
	tree_root = NULL;
	while (getword(&word) != EOF)
		if (isalpha(word.content[0]))
			tree_root = tree_add(tree_root, &word, &tree_qtt);

	// Convert tree to Linear list
	node_list = tree_nodelist(tree_root, tree_qtt);

	// Print in order by word
	if (0) {
		printf("Sort by word:\n");
		tree_print(tree_root);
	}

	// Print in order by count
	printf("Sort by count:\n");
	qsort(node_list, tree_qtt, sizeof node_list[0], node_sort_by_count);
	for (pnode = node_list; pnode < node_list + tree_qtt; pnode++)
		tnode_show(*pnode);

	return 0;
}
