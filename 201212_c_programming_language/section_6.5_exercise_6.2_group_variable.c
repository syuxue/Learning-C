#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/m_function.h"

/* ****************************** Definition & Declaration ****************************** */
#define WORD_LENGTH		4096
struct tgroup {
	char *prefix;
	struct tword *wordroot;
	struct tgroup *left;
	struct tgroup *right;
};
struct tword {
	char *word;
	struct tword *left;
	struct tword *right;
};
int op_initiallen;
char *strndup(const char *s, size_t n);

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

struct tword *word_add(struct tword *p, char *word)
{
	int cond;

	if (p == NULL) {
		p = (struct tword *) malloc(sizeof(struct tword));
		p->word = strdup(word);
		p->right = p->left = NULL;
	} else if ((cond = strcasecmp(word, p->word)) == 0) {
		;
	} else if (cond > 0) {
		p->right = word_add(p->right, word);
	} else if (cond < 0) {
		p->left = word_add(p->left, word);
	}

	return p;	
}

struct tgroup *group_add(struct tgroup *p, char *word)
{
	int cond;

	if (p == NULL) {
		p = (struct tgroup *) malloc(sizeof(struct tgroup));
		if (op_initiallen == 0)
			p->prefix = m_strtoupper(strdup(word));
		else
			p->prefix = m_strtoupper(strndup(word, op_initiallen));
		p->wordroot = word_add(p->wordroot, word);
		p->right = p->left = NULL;
	} else if (
		(op_initiallen == 0 && (cond = strcasecmp(word, p->prefix)) == 0)
		||
		(op_initiallen != 0 && (cond = strncasecmp(word, p->prefix, op_initiallen)) == 0)
	) {
		p->wordroot = word_add(p->wordroot, word);;
	} else if (cond > 0) {
		p->right = group_add(p->right, word);
	} else if (cond < 0) {
		p->left = group_add(p->left, word);
	}

	return p;
}

void word_print(struct tword *p)
{
	if (p == NULL)
		return;

	word_print(p->left);
	printf("\t%s\n", p->word);
	word_print(p->right);
}

void group_print(struct tgroup *p)
{
	if (p == NULL)
		return;
	
	/* Format
	原来用%-*s这样的格式就可以带长度的参数了
	static char format[16] = "";
	if (*format == '\0') {
		strcpy(format, "%");
		m_int2str(op_initiallen, format + 1, 10, 0, '\0');
		strcat(format, "s :\n");
	}*/

	group_print(p->left);
	printf("%-*s:\n", op_initiallen, p->prefix);
	word_print(p->wordroot);
	group_print(p->right);
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char word[WORD_LENGTH];
	struct tgroup *root;

	// Argument
	op_initiallen = 0;
	while (--argc > 0) {
		argv++;
		if (op_initiallen == -1)
			op_initiallen = atoi(*argv);
		else if (strcmp(*argv, "-l") == 0)
			op_initiallen = -1;
	}
	op_initiallen = op_initiallen < 0 ? 0 : op_initiallen;

	// Process
	root = NULL;
	while (getword(word) != EOF) {
		//printf("%s\n", word);
		if (isalpha(word[0]))
			root = group_add(root, word);
	}

	// Output
	group_print(root);

	return 0;
}
