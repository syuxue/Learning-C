#include <stdio.h>
#include "include/m_function.h"

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	// Fval
	float fval;
	if (0) {
		srand((unsigned long) argv);
		fval = (float)(rand() % 1000 + 1) / (rand() % 1000 + 1) - (rand() % 3);
	} else {
		//fval = -678.478;
		fval = -0.000000000000000000000000000000000000000000025;
		fval = 0.5625;
		fval = 340282347000000000000000000000000000000.0;
	}

	// Parse float to bit then Recalculate
	char fstr[128];
	m_float2str(fval, fstr, 10, 50, ' ');
	printf("****************************** ******************************\n");
	printf("fstr: [%s]\n", fstr);
	printf("fval: [%50.0f]\n", fval);
	M_printf(m_str2float(fstr, 2));

	return 0;
}
