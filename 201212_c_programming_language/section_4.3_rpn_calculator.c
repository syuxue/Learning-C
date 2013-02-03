#include <stdio.h>
#include "../include/m_function.h"
#include "section_4.3_rpn.h"

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char notation[RPN_NOTATION_MAXSIZE];

	*notation = '\0';
	if (argc == 1) {
		while (m_getline(notation, RPN_NOTATION_MAXSIZE) > 0) {
			if (*notation == '#') continue;
			m_trim(notation);
			printf(M_bash_YELLOW "\"%s\" = %g\n" M_bash_default, notation, rpn_calculate(notation));
		}
	} else {
		while (--argc > 0) {
			m_strcat(notation, *(++argv));
			m_strcat(notation, " ");
		}
		m_trim(notation);
		printf(M_bash_YELLOW "\"%s\" = %g\n" M_bash_default, notation, rpn_calculate(notation));
	}

	return 0;
}
