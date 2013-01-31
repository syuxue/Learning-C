#include <stdio.h>
#include "include/m_function.h"

// Main
int main(void)
{
	char dstr[1024];
	double dval;

	dval = -2.2200000000500000e-322;
	dval = 5.321005654001230e+150;
	dval = 3.1415926535897932;
	dval = 1.123456789011112345678922507385850720148979321415926535897932123456789011112345678922507385850720148979321415926535897932;
	dval = 2.2250738585072014e-323;
	dval = 2.2250738585072014e+307;

	M_showvariable(dval);
	m_double2str(dval, dstr);
	printf("****************************************************\n");
	printf("[%50s]\n", dstr);
	printf("[%50.30g]\n", dval);
	printf("[%50.30g]\n", m_str2double(dstr, 10));


	return 0;
}
