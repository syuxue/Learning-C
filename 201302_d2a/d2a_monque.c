#include <stdio.h>
#include <stdlib.h>
//#include <ieee754.h>
#include "../include/m_function.h"

/*
可移植
	include ieee754.h
	支持double，根据机器不同调整double的类型
输出无限精度
	sprintf
	取余入栈，循环处理
	将2进制分段处理
*/

typedef struct {
	float			original;
	unsigned int	negative:1;
	unsigned int	exponent:8;
	unsigned int	mantissa:23;
} ieee754_float;

typedef struct {
	double			original;
	unsigned long	negative:1;
	unsigned long	exponent:11;
	unsigned long	mantissa:52;
} ieee754_double;

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
	char *p, expo_base;
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
		unsigned char tmp_expo;

		if (expo == 0xFF) {
			*((unsigned int *) &dec_recal) = (0xFF << 23) | mant;
		} else {
			dec_recal = (float) mant / (1 << 23);
			if (expo >= 127) {
				for (tmp_expo = expo - 127; tmp_expo > 30; tmp_expo -= 30)
					dec_recal *= 1 << 30;
				dec_recal *= 1 << tmp_expo;
			} else {
				for (tmp_expo = 127 - expo; tmp_expo > 30; tmp_expo -= 30)
					dec_recal /= 1 << 30;
				dec_recal /= 1 << tmp_expo;
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
	if (expo > 0x00 && expo < 0xFF) {
		expo_base = 0;
		if (expo - 23 > 127) {
			for (; expo - 23 > 127; expo--) {
				if (mant > 0x7FFFFFFF) {
					mant /= base;
					expo_base++;
				}
				mant <<= 1;
			}
		} else {
			for (; expo - 23 < 127; expo++) {
				if ((mant & 1) && mant < 0x19999999) {
					mant *= base;
					expo_base--;
				}
				mant >>= 1;
			}
		}

		// 去掉尾数0
		while (mant > 0 && mant % base == 0) {
			expo_base++;
			mant /= base;
		}

		while (mant > 0) {
			expo_base++;
			*(p++) = mant % base + '0';
			mant /= base;
		}
		expo_base--;
		*p = *(p - 1);
		*(p - 1) = '.';
		p++;

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

	// E
	if (expo_base != 0) {
		*(p++) = 'e';
		*(p++) = expo_base > 0 ? '+' : '-';
		expo_base = expo_base > 0 ? expo_base : -expo_base;
		while (expo_base > 0) {
			*(p++) = expo_base % 10 + '0';
			expo_base /= 10;
		}
	}
	*p = '\0';

	free(pfloat);
	return str;
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	float fval;
	if (0) {
		srand((unsigned long) argv);
		fval = (float)(rand() % 1000 + 1) / (rand() % 1000 + 1) - (rand() % 3);
	} else {
		//fval = 0.135654;
		fval = -0.000000000000000000000000000000000000000000025;
		fval = 0.00000000002625;
		fval = 123123.2;
		fval = 340282347000000000000000000000000000000.0;
	}

	// Parse float to bit then Recalculate
	int base = 10;
	char fstr[128];
	m_float2str(fval, fstr, base, 100, 50, ' ');
	printf("****************************** ******************************\n");
	printf("fstr: [%s]\n", fstr);
	printf("fval: [%50g]\n", fval);
	M_printf(m_str2float(m_trim(fstr), base));

	return 0;
}

/* ****************************** 以下没用 ****************************** */
	/*
	 * printf的说明
	 * f:   double；形式为[-]mmm.ddd 的十进制表示，其中，d 的数目由精度确确定，默认精度为6。精度为0 时不输出小数点
	 * e,E: double；形式为[-]m.dddddd e ±xx或[-]m.dddddd E ±xx。d 的数目由精度确定，默认精度为6。精度为0 时不输出小数点
	 * g,G: double；当指数小于-4 或大于等于精度时，采用%e 或%E 的格式，否则采用%f 的格式。尾部的0 和小数点不打印
	 */
	/*if (0) {
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
	}*/

		//maozefa是将expo < 127的小数转为大数并保存为float，之后将float转为int进行的输出
		/*
		 * 给定指数位exp，求该数为10的几次幂x
		 * 10^x = 2^exp
		 * x = lg2^exp
		 * x = exp*lg2
		 */
		
		// 找到尾数的实际指数
		/*char expo_base;
		unsigned char expo_mant;
		for (expo_mant = 23; !(mant & (1 << expo_mant)) && expo_mant > 0; expo_mant--)
			;
		if (expo_mant == 23)
			expo_base = (expo - 127) * 0.30102999566398119521373889472449;
		else
			expo_base = (expo - 127 + expo_mant - 23) * 0.30102999566398119521373889472449;
		M_printd(expo_base);*/
