#include <stdlib.h>
#include <string.h>
#include "include/m_function.h"

#define LINE_SIZE 4097

// Parameter
char *pattern;
int op_line_number, op_invert_match, op_with_filename;

void grep(char *pattern, char *filename)
{
	int i, num, len, pos;
	char line[LINE_SIZE];
	FILE *fp;

	fp = filename == NULL ? stdin : fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "error: can not found %s\n", filename);
		exit(EXIT_FAILURE);
	}

	for (num = 1; fgets(line, LINE_SIZE, fp); num++) {
		pos = m_strpos(line, pattern);
		if ((!op_invert_match && pos > -1) || (op_invert_match && pos == -1)) {
			len = strlen(line);
			if (line[len - 1] == '\n')
				line[len - 1] = '\0';
			if (op_with_filename)
				printf(M_bash_magenta "%s" M_bash_cyan ":" M_bash_default, filename ? filename : "stdin");
			if (op_line_number)
				printf(M_bash_green "%d" M_bash_cyan ":" M_bash_default, num);
			// show pattern in color
			if (op_invert_match)
				printf("%s\n", line);
			else {
				for (i = 0; i < pos; i++)
					putchar(line[i]);
				printf(M_bash_RED);
				for (; i < pos + strlen(pattern); i++)
					putchar(line[i]);
				printf(M_bash_default);
				for (; line[i] != '\0'; i++)
					putchar(line[i]);
				printf("\n");
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int i, filenum;
	char *p, *filelist[argc - 1];

	// Parameter
	pattern = NULL;
	filenum = op_with_filename = op_invert_match = op_line_number = 0;
	while (--argc > 0) {
		argv++;
		if (strchr(*argv, '-') == *argv) {
			for (p = *argv + 1; *p != '\0'; p++) {
				if (*p == 'n')
					op_line_number = 1;
				else if (*p == 'H')
					op_with_filename = 1;
				else if (*p == 'v')
					op_invert_match = 1;
				else {
					fprintf(stderr, "error: unknown argument -%c\n", *p);
					exit(EXIT_FAILURE);
				}
			}
		} else if (pattern == NULL)
			pattern = *argv;
		else
			filelist[filenum++] = *argv;
	}
	op_with_filename = filenum > 1 ? 1 : op_with_filename;
	if (filenum == 0)
		filelist[filenum++] = NULL;
	if (pattern == NULL) {
		fprintf(stderr, "error: empty pattern\n");
		exit(EXIT_FAILURE);
	}

	// For
	for (i = 0; i < filenum; i++) {
		grep(pattern, filelist[i]);
	}

	return 0;
}
