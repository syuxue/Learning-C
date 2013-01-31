#include <stdio.h>
#include "../include/m_function.h"
char *m_double2str(const double dec, char *str);


// Main
int main(void)
{
	char dstr[1024];
	double dval;

	dval = 2.2250738585072014e-308;
	dval = 2.2250738585072014e-323;
	dval = -2.2200000000500000e-322;
	dval = 5.321005654001230e+100;
	dval = 2123;
	dval = 2e1;
	m_double2str(dval, dstr);
	printf("****************************************************\n");
	printf("[%50s]\n", dstr);
	printf("[%50g]\n", dval);
	printf("[%50g]\n", m_str2double(dstr, 10));

	return 0;
}
