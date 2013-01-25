#include <stdio.h>
#include <stdlib.h>
#include "m_function.h"

/* ****************************** Declaration ****************************** */
#define MAXLINES	100000
#define MAXLENGTH	4096
int		line_read(char **lineptr, int maxlines);
void	line_write(char **lineptr, int nlines);
void	line_swap(void **lineptr, int p, int q);
int		line_compare(void *to, void *from, int (*func_compare)(void *, void *), int op_key);
void	line_sort(void **lineptr, int left, int right, int (*func_compare)(void *, void *), int op_reverse, int op_key);

/* ****************************** Comparison ****************************** */
int	cmp_numeric(char *to, char *from)
{
	double vto, vfrom;

	vto = m_str2float(to, 10);
	vfrom = m_str2float(from, 10);
	if (vto < vfrom)
		return -1;
	else if (vto > vfrom)
		return 1;
	else
		return 0;
}

int cmp_stri(char *to, char *from)
{
	int res;

	res = m_strcasecmp(to, from);
	return res == 0 ? (*from - *to) : res;
}

/* ****************************** Function ****************************** */
int line_read(char **lineptr, int maxlines)
{
	int len, i;
	char *p, line[MAXLENGTH];

	i = 0;
	while ((len = m_getline(line, sizeof line)) > 0) {
		if (i >= maxlines)
			return -1;

		//Delete new line
		if (line[len - 1] == '\n')
			line[--len] = '\0';

		//Store line
		p = malloc(len + 1);
		m_strcpy(p, line);
		*(lineptr + i) = p;

		i++;
	}

	return i;
}

void line_write(char **lineptr, int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *(lineptr++));
}

void line_swap(void **lineptr, int p, int q)
{
	char *tmp;

	tmp = *(lineptr + p);
	*(lineptr + p) = *(lineptr + q);
	*(lineptr + q) = tmp;
}

int line_compare(void *to, void *from, int (*func_compare)(void *, void *), int op_key)
{
	if (op_key > 0) {
		char field_to[MAXLENGTH], field_from[MAXLENGTH];
		m_strfield(field_to, to, op_key);
		m_strfield(field_from, from, op_key);
		return (*func_compare)(field_to, field_from);
	} else {
		return (*func_compare)(to, from);
	}
}

void line_sort(void **lineptr, int left, int right, 
	int (*func_compare)(void *, void *), int op_reverse, int op_key)
{
	int i, base;

	if (left >= right)
		return;

	base = left;
	for (i = left + 1; i <= right; i++) {
		if (line_compare(*(lineptr + i), *(lineptr + base), func_compare, op_key) * op_reverse < 0) {
			line_swap(lineptr, i, ++left);
		}
	}
	line_swap(lineptr, base, left);

	line_sort(lineptr, base, left, func_compare, op_reverse, op_key);
	line_sort(lineptr, left + 1, right, func_compare, op_reverse, op_key);
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	int nlines;
	char *lineptr[MAXLINES];
	int op_reverse, op_ignorecase, op_numeric, op_key, op_verbose;
	int (*op_func_compare)();

	//Argument
	op_ignorecase = op_numeric = op_key = op_verbose = 0;
	op_reverse = 1;
	op_func_compare = NULL;
	while (--argc > 0) {
		argv++;
		if (op_key == -1)
			op_key = m_str2int(*argv, 10);
		else if (m_strcmp(*argv, "-n") == 0)
			op_numeric = 1;
		else if (m_strcmp(*argv, "-r") == 0)
			op_reverse = -1;
		else if (m_strcmp(*argv, "-f") == 0)
			op_ignorecase = 1;
		else if (m_strcmp(*argv, "-v") == 0)
			op_verbose = 1;
		else if (m_strcmp(*argv, "-k") == 0)
			op_key = -1;
		else if (m_strncmp(*argv, "-k", 2) == 0)
			op_key = m_str2int((*argv + 2), 10);
	}
	op_key = op_key >= 0 ? op_key : 0;
	if (op_ignorecase) {
		if (op_verbose)
			printf(M_bash_YELLOW "#Function:   cmp_stri\n" M_bash_default);
		op_func_compare = (int (*)(void *, void *)) cmp_stri;
	} else if (op_numeric) {
		if (op_verbose)
			printf(M_bash_YELLOW "#Function:   cmp_numeric\n" M_bash_default);
		op_func_compare = (int (*)(void *, void *)) cmp_numeric;
	} else {
		if (op_verbose)
			printf(M_bash_YELLOW "#Function:   m_strcmp\n" M_bash_default);
		op_func_compare = (int (*)(void *, void *)) m_strcmp;
	}

	//Display Option
	if (op_verbose) {
		printf(M_bash_YELLOW);
		printf("#Ignorecase: %d\n", op_ignorecase);
		printf("#Numeric:    %d\n", op_numeric);
		printf("#Reverse:    %d\n", op_reverse);
		printf("#Key Pos:    %d\n", op_key);
		printf(M_bash_default);
	}

	//Getline
	if ((nlines = line_read(lineptr, MAXLINES)) <= 0) {
		printf("error: input too big to sort\n");
		return -1;
	}

	//Sort
	line_sort((void **)lineptr, 0, nlines - 1, op_func_compare, op_reverse, op_key);

	//Output
	line_write(lineptr, nlines);
	if (op_verbose) {
		printf(M_bash_YELLOW "#Total:     %d\n" M_bash_default, nlines);
	}

	return 0;
}
