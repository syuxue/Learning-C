#include <stdio.h>
#include <string.h>
#include "m_function.h"
#define MAXLINES	100000
#define MAXLENGTH	4096

/* ****************************** Declaration ****************************** */
char *lineptr[MAXLINES];

/* ****************************** Fcuntion ****************************** */
void swap_pointer(void **parr, int i, int j)
{
	void *temp;

	temp = parr[i];
	parr[i] = parr[j];
	parr[j] = temp;
}

int	numcmp(char *to, char *from)
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

int readlines(char **lineptr, int maxlines)
{
	int len, nlines;
	char *p, line[MAXLENGTH];

	nlines = 0;
	while ((len = m_getline(line, sizeof line)) > 0) {
		if (nlines >= maxlines) {
			return -1;
		} else {
			if (line[len - 1] == '\n')
				line[--len] = '\0';
			p = m_alloc(len + 1);
			strcpy(p, line);
			*(lineptr + nlines++) = p;
		}
	}

	return nlines;
}

void writelines(char *lineptr[], int nlines)
{
	int i;

	for (i = 0; i < nlines; i++)
		printf("%s\n", lineptr[i]);
}

int compare(void *v1, void *v2, int (*comp)(void *, void *))
{
	return (*comp)(v1, v2);
}

void qsort(void *lineptr[], int left, int right,
	int (*comp)(void *, void *), 
	int op_reverse, int op_key)
{
	int i,base;

	if (left >= right) return;

	base = left;
	for (i = left + 1; i <= right; i++) {
		if (compare(lineptr[i], lineptr[base], comp) * op_reverse < 0) {
		//if ((*comp)(lineptr[i], lineptr[base]) * op_reverse < 0) {
			swap_pointer(lineptr, i, ++left);
		}
	}
	swap_pointer(lineptr, base, left);

	qsort(lineptr, base, left, comp, op_reverse, op_key);
	qsort(lineptr, left + 1, right, comp, op_reverse, op_key);
}

/* ****************************** Main ****************************** */
//gcc -Wall ../m_function.c section_5.6_pointer_array.c && ll | awk '{print $5}' | ./a.out -n -r
int main(int argc, char *argv[])
{
	int nlines;
	int op_numeric, op_reverse, op_ignorecase, op_key;
	int (*op_func_comparison)(void *, void *);

	//Argument
	op_numeric = op_ignorecase = 0;
	op_reverse = 1;
	op_key = -1;
	while (--argc > 0) {
		argv++;
		if (op_key == -2)
			op_key = m_str2int(*argv, 10);
		else if (m_strcmp(*argv, "-n") == 0)
			op_numeric = 1;
		else if (m_strcmp(*argv, "-r") == 0)
			op_reverse = -1;
		else if (m_strcmp(*argv, "-f") == 0)
			op_ignorecase = 1;
		else if (m_strcmp(*argv, "-k") == 0)
			op_key = -2;
		else if (m_strncmp(*argv, "-k", 2) == 0)
			op_key = m_str2int((*argv + 2), 10);
	}
	op_key = op_key >= 0 ? op_key : 0;
	if (op_ignorecase)
		op_func_comparison = (int (*)(void*,void*)) m_strcasecmp;
	else if (op_numeric)
		op_func_comparison = (int (*)(void*,void*)) numcmp;
	else
		op_func_comparison = (int (*)(void*,void*)) m_strcmp;

	//Sort
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort(
			(void **)lineptr,
			0,
			nlines - 1,
			op_func_comparison,
			op_reverse,
			op_key
		);
		writelines(lineptr, nlines);
		printf("Total:%d\n", nlines);
		return 0;
	} else {
		printf("error: input too big to sort\n");
		return 1;
	}
}
