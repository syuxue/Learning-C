#include <stdio.h>
#include <stdlib.h>
#include "include/m_function.h"

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
char *m_float2str(float dec, char *str, int base, int precision, int minwidth, char filler)
{
	char *p;
	unsigned char expo;
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
		unsigned char tmpexpo;

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
	{
		M_printu(expo);
		M_printu(mant);
	}

	// Part Sign
	if (pfloat->negative)
		*(p++) = '-';

	// Fill to min-width
	while (p - str < minwidth)
		*(p++) = filler;
	
	// Endding
	*p = '\0';
	m_strrev(str);

	free(pfloat);
	return str;
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
		fval = -0.000000000000000000000000000000000000000000025;
		fval = 0.5625;
		fval = 340282347000000000000000000000000000000.0;
	}

	// Parse float to bit then Recalculate
	char fstr[128];
	m_float2str(fval, fstr, 10, 100, 50, ' ');
	printf("****************************** ******************************\n");
	printf("fstr: [%s]\n", fstr);
	printf("fval: [%50.0f]\n", fval);
	M_printf(m_str2float(fstr, 2));

	/*
	 * printf的说明
	 * f:   double；形式为[-]mmm.ddd 的十进制表示，其中，d 的数目由精度确确定，默认精度为6。精度为0 时不输出小数点
	 * e,E: double；形式为[-]m.dddddd e ±xx或[-]m.dddddd E ±xx。d 的数目由精度确定，默认精度为6。精度为0 时不输出小数点
	 * g,G: double；当指数小于-4 或大于等于精度时，采用%e 或%E 的格式，否则采用%f 的格式。尾部的0 和小数点不打印
	 */
	if (0) {
		printf("****************************** Printf ******************************\n");
		printf("Here is a problem of float, "
				M_bash_RED "(float) 67800.4" M_bash_default " be printf is "
				M_bash_RED "%f" M_bash_default "\n",
				(float) 67800.4
		);
		printf("In Fact " M_bash_RED "(float) 67800.4" M_bash_default " is 10000100011011000.0110011\n");
		printf("And    " M_bash_RED "(double) 67800.4" M_bash_default " is 10000100011011000.0110011"
				M_bash_RED "001100110011001101" M_bash_default "\n");
		printf("Because of Precision ...\n");
		printf("Binary: 10000100011011000.0110011 != 10000100011011000 + 0.0110011\n");
		printf("as  %%f: %24f  != %17d + %8f\n",
			m_str2float("10000100011011000.0110011", 2),
			m_str2int("10000100011011000", 2),
			m_str2float("0.0110011", 2)
		);
		printf("as  %%g: %19g       != %17d + %8g\n",
			m_str2float("10000100011011000.0110011", 2),
			m_str2int("10000100011011000", 2),
			m_str2float("0.0110011", 2)
		);
	}

	return 0;
}
