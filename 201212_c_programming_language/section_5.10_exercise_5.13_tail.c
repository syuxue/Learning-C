#include <stdio.h>
#include <stdlib.h>
#include "m_function.h"

/* ****************************** Declaration ****************************** */
#define MAXLENGTH	4096
void head(int n);
void tail(int n);

/* ****************************** Fcuntion ****************************** */
void head(int n)
{
	int i;
	char line[MAXLENGTH];
	
	for (i = 0; i < n && m_getline(line, MAXLENGTH) > 0; i++)
		printf("%s", line);
}

//TODO:如何提升效率，目前耗费时间为tail@linux的10倍
void tail(int n)
{
	int i, j, len;
	char **lineptr, line[MAXLENGTH];

	//Alloc for lineptr
	lineptr = malloc(sizeof *lineptr * n);
	for (i = 0; i < n; i++)
		*(lineptr + i) = NULL;

	//Getline
	i = 0;
	while ((len = m_getline(line, sizeof line)) > 0) {
		
		//Free
		if (*(lineptr + i))
			free(*(lineptr + i));

		//Set lineptr
		if (line[len - 1] == '\n')
			line[--len] = '\0';
		*(lineptr + i) = malloc(len + 1);
		m_strcpy(*(lineptr + i), line);
		
		//Reset i
		i = (i + 1) % n;
	}

	//Display
	j = i;
	while (1) {
		if (*(lineptr + i)) {
			printf("%s\n", *(lineptr + i));
			free(*(lineptr + i));
		}
		i = (i + 1) % n;
		if (i == j)
			break;
	}
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	//Argument
	void (*op_function)();
	int op_arg_n;
	op_function = NULL;
	op_arg_n = -1;
	while (--argc > 0) {
		argv++;
		if (op_arg_n == -2)
			op_arg_n = m_str2int(*argv, 10);
		else if (m_strcmp(*argv, "tail") == 0)
			op_function = ( void (*)(int) ) tail;	//set op_function point to tail()
		else if (m_strcmp(*argv, "head") == 0)
			op_function = head;						//set op_function point to head()
		else if (m_strcmp(*argv, "-n") == 0)
			op_arg_n = -2;
		else if (m_strncmp(*argv, "-n", 2) == 0)
			op_arg_n = m_str2int((*argv + 2), 10);
	}
	op_arg_n = op_arg_n >= 0 ? op_arg_n : 10;

	if (op_function)
		(*op_function)(op_arg_n);
	else
		printf("error: unknown function\n");

	return 0;
}
