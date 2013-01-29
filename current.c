#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/m_function.h"

/* ****************************** Function ****************************** */
/* Reference */ // IEEE_754@http://zh.wikipedia.org/wiki/IEEE_754
/* Reference */ // 自己动手写C语言浮点数转换字符串函数@http://www.cnblogs.com/maozefa/archive/2011/12/21/2295731.html
/* Reference */ // 浮点数的存储格式@http://blog.csdn.net/ganxingming/article/details/1449526
typedef struct {
	float			original;
	unsigned char	negative;
	unsigned char	exponent;
	unsigned int	mantissa;
} ieee754_float;

ieee754_float *parse_float(float val)
{
	ieee754_float *pfloat;

	if ((pfloat = malloc(sizeof(ieee754_float))) == NULL)
		return NULL;

	pfloat->original = val;
	pfloat->negative = (*((unsigned int *)&val) & 0x80000000) ? 1 : 0;
	pfloat->exponent = *((unsigned int *)&val) >> 23 & 0x000000FF;
	pfloat->mantissa = *((unsigned int *)&val) & 0x007FFFFF;

	return pfloat;
}

// sprintf("%{minwidth of filler}.{precision}f\n", dec);
char *m_float2str(float dec, char *str, int base, int minwidth, char filler)
{
	char *p;
	unsigned char expo, tmpexpo;
	unsigned int mant;
	ieee754_float *pfloat;

	p = str;

	// Handle IEEE 754 conditions
	pfloat = parse_float(dec);
	expo = pfloat->exponent;
	mant = pfloat->mantissa;
	if (pfloat->exponent == 0xFF) {
		if (pfloat->mantissa == 0x00) {									// inf, Infinity
			*(p++) = 'f';
			*(p++) = 'n';
			*(p++) = 'i';
		} else {														// NaN, Not A Number
			*(p++) = 'N';
			*(p++) = 'a';
			*(p++) = 'N';
		}
	} else if (pfloat->exponent == 0x00) {
		if (pfloat->mantissa == 0x00) {									// 0
			*(p++) = '0';
		} else {														// subnormal numbers have an implicit leading significand bit of 0
			expo = 1;
		}
	} else if (pfloat->exponent > 0x00 && pfloat->exponent < 0xFF) {	// normal numbers have an implicit leading significand bit of 1
		mant = pfloat->mantissa | 0x800000;
	}

	// Re-Calculate to check
	{
		float dec_recal;

		if (expo == 0xFF) {
			*((unsigned int *) &dec_recal) = (0xFF << 23) | mant;
		} else {
			dec_recal = (float) mant / (1 << 23);
			if (expo >= 127) {
				for (tmpexpo = expo - 127; tmpexpo > 30; tmpexpo -= 30)
					dec_recal *= 1 << 30;
				dec_recal *= 1 << tmpexpo;
			} else {
				for (tmpexpo = 127 - expo; tmpexpo > 30; tmpexpo -= 30)
					dec_recal /= 1 << 30;
				dec_recal /= 1 << tmpexpo;
			}
		}

		if (pfloat->negative)
			dec_recal = -dec_recal;	

		if (dec_recal == dec)
			printf(M_bash_GREEN "%.10g == %.10g\n" M_bash_default, dec_recal, dec);
		else {
			printf(M_bash_RED "%.10g != %.10g\n" M_bash_default, dec_recal, dec);
			M_showvariable(dec);
			M_showvariable(dec_recal);
		}
	}

	// Part Number
	M_printu(expo);
	M_printu(mant);

	// Part Sign
	if (pfloat->negative)
		*(p++) = '-';

	// Fill to min-width
	while (p - str < minwidth)
		*(p++) = filler;
	*p = '\0';

	free(pfloat);
	return m_strrev(str);
}

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
		//fval = -0.00000000000000000000000000000000000000000000078;
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
