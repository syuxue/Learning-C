#include <stdio.h>
#include "include/m_function.h"
void echo(int argc, char *argv[]);
void grep(char *pattern, int op_except, int op_number);

void echo(int argc, char *argv[])
{
	while (--argc > 0)
		printf("#Argv:[%s]\n", *(++argv));
	printf("\n");
}

void grep(char *pattern, int op_except, int op_number)
{
	int num;
	char line[4096];

	num = 0;
	while (m_getline(line, 4096) > 0) {
		num++;
		if (
			(op_except && m_strpos(line, pattern) == -1)
			|| (!op_except && m_strpos(line, pattern) > -1)
		) {
			if (op_number)
				printf(M_bash_green "%d" M_bash_cyan ":" M_bash_default, num);
			printf("%s", line);
		}
	}
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	//func:echo
	printf("[Argv]\n");
	echo(argc, argv);
	
	//func:grep
	printf("[Grep]\n");
	int op_except, op_number;
	char *pattern;
	
	pattern = NULL;
	op_except = op_number = 0;
	while (--argc > 0) {
		argv++;
		if (m_strcmp(*(argv), "-n") == 0)
			op_number = 1;
		else if (m_strcmp(*(argv), "-v") == 0)
			op_except = 1;
		else if (pattern == NULL)
			pattern = *argv;
		else {
			printf("error: wrong arguments\n");
			return 1;
		}
	}

	if (pattern != NULL)
		grep(pattern, op_except, op_number);

	return 0;
}
