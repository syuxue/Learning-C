#include <stdio.h>
#include "include/m_function.h"

/*
TODO:
	jump
	读标准库
	GNU Coding Standards
*/

int main(int argc, char *argv[])
{
	double dval = 3.1415926535897932384;
	char dstr[1024];

	m_double2str(dval, dstr);
	M_print("[%-50.40f]", dval);
	M_print("[%-50s]", dstr);
	
	return 0;
}
