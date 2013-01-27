#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/m_function.h"

/* ****************************** Function ****************************** */
/* Reference */ // IEEE_754@http://zh.wikipedia.org/wiki/IEEE_754
/* Reference */ // 自己动手写C语言浮点数转换字符串函数@http://www.cnblogs.com/maozefa/archive/2011/12/21/2295731.html
/* Reference */ // 浮点数的存储格式@http://blog.csdn.net/ganxingming/article/details/1449526
typedef struct {
	float original;
	unsigned char sign;
	unsigned char bias;
	unsigned int frac;
} _float;

_float *parse_float(float val)
{
	_float *pfloat;

	if ((pfloat = malloc(sizeof(_float))) == NULL)
		return NULL;

	pfloat->original = val;
	pfloat->sign = (*((unsigned int *)&val) & 0x80000000) ? 1 : 0;
	pfloat->bias = *((unsigned int *)&val) >> 23 & 0x000000FF;
	pfloat->frac = *((unsigned int *)&val) & 0x007FFFFF;

	return pfloat;
}

float recal_float(_float *pfloat)
{
	float val;
	unsigned char bias;

	if (pfloat->bias == 0xFF && pfloat->frac != 0x00) {
		*(unsigned int *)&val = pfloat->bias << 23;
		*(unsigned int *)&val |= 1;
		bias = 0;
	} else if (pfloat->bias == 0xFF && pfloat->frac == 0x00) {
		*(unsigned int *)&val = pfloat->bias << 23;
		bias = 0;
	} else if (pfloat->bias > 0x00 && pfloat->bias < 0xFF) {
		val = 1.0 + (float) pfloat->frac / (1 << 23);
		bias = pfloat->bias;
	} else if (pfloat->bias == 0x00 && pfloat->frac != 0x00) {
		val = 0.0 + (float) pfloat->frac / (1 << 23);
		bias = 1;
	} else if (pfloat->bias == 0x00 && pfloat->frac == 0x00) {
		val = 0.0;
		bias = 0;
	}

	if (bias > 127) {
		for (bias = bias - 127; bias > 30; bias -= 30)
			val *= 1 << 30;
		val *= 1 << bias;
	} else if (bias < 127) {
		for (bias = 127 - bias; bias > 30; bias -= 30)
			val /= 1 << 30;
		val /= 1 << bias;
	}

	if (pfloat->sign)
		val = -val;	

	return val;
}

void test_float(float val)
{
	float fval;
	_float *pfloat;

	pfloat = parse_float(val);
	
	M_showvariable(pfloat->original);
	M_showvariable(pfloat->sign);
	M_showvariable(pfloat->bias);
	M_showvariable(pfloat->frac);
	
	fval = recal_float(pfloat);
	if (fval == pfloat->original)
		printf(M_bash_GREEN "%g == %g\n" M_bash_default, fval, pfloat->original);
	else
		printf(M_bash_RED "%g != %g\n" M_bash_default, fval, pfloat->original);

	free(pfloat);
}

char *m_float2str(double dec, char *str/*, int base, int minwidth, char filler*/)
{
	char bias;
	unsigned int frac;
	_float *pfloat;

	pfloat = parse_float((float)dec);
	bias = (char) pfloat->bias - 127 - 23;
	frac = pfloat->frac | 0x00800000;

	M_showvariable(frac);
	m_int2str((int)frac, str, 2, 0, ' ');
	while (bias > 0) {
		m_strcat(str, "0");
		bias--;
	}
	while (m_strlen(str) + bias < 0)
		m_strinsert(str, "0", 0);
	m_strinsert(str, ".", m_strlen(str) + bias);
	if (*str == '.')
		m_strinsert(str, "0", 0);
	if (pfloat->sign)
		m_strinsert(str, "-", 0);

	// 提取整数部分
	{
		int part_integer;
		
		bias = (char) pfloat->bias - 127 - 23;
		M_printd(frac);
		M_printd(bias);
		if (bias > 0)
			part_integer = frac << bias;
		else if (bias > -24)
			part_integer = frac >> -bias;
		else
			part_integer = 0;
		M_printd(part_integer);
	}

	return str;
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	float fval;

	srand((unsigned int) argv);
	fval = (float)(rand() % 1000 + 1) / (rand() % 1000 + 1) - (rand() % 3);

	// Parse float to bit then Recalculate
	if (0) {		
		test_float(fval);
	}

	//fval = -678.478;
	//fval = 1024000000.0;
	fval = 0.314;
	char str[128];

	m_float2str(fval, str);
	printf("**************************************************\n%s\n", str);
	M_printf(fval);
	M_printf(m_str2float(str, 2));

	return 0;
}
