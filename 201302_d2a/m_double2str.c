#include <string.h>
#include <ieee754.h>
#include <float.h>
#include <limits.h>
#include "../include/m_function.h"

#define IEEE754_DOUBLE_EMAX			IEEE754_DOUBLE_BIAS
#define IEEE754_DOUBLE_EMIN			1 - IEEE754_DOUBLE_EMAX
#define IEEE754_DOUBLE_MANT_DIG		DBL_MANT_DIG
#define DEC_PRECISION_MAX			17

static const unsigned long tab_exponent_base[] = {
	0x0000000000000001, 0x000000000000000a, 0x0000000000000064, 0x00000000000003e8,
	0x0000000000002710, 0x00000000000186a0, 0x00000000000f4240, 0x0000000000989680,
	0x0000000005f5e100, 0x000000003b9aca00, 0x00000002540be400, 0x000000174876e800,
	0x000000e8d4a51000, 0x000009184e72a000, 0x00005af3107a4000, 0x00038d7ea4c68000,
	0x002386f26fc10000, 0x016345785d8a0000, 0x0de0b6b3a7640000, 0x8ac7230489e80000
};

char *m_double2str(const double dec, char *str)
{
	const int base = 10;
	
	char *p;
	union ieee754_double *pdouble;
	int exponent, exponent_base, exponent_base_mant, exponent_base_sign;
	unsigned long mantissa;

	// 将浮点数按照IEEE 754标准提取出exponent, mantissa部分
	pdouble = (union ieee754_double *) &dec;
	exponent = pdouble->ieee.exponent - IEEE754_DOUBLE_BIAS;
	mantissa = (unsigned long) pdouble->ieee.mantissa0 << 32 | pdouble->ieee.mantissa1;

	// 处理IEEE 754中的特殊情况(inf, nan, 0, normal, subnormal)
	if (exponent > IEEE754_DOUBLE_EMAX) {
		if (mantissa == 0)
			return strcpy(str, pdouble->ieee.negative ? "-inf" : "inf");
		else
			return strcpy(str, "NaN");
	} else if (exponent < IEEE754_DOUBLE_EMIN) {
		if (mantissa == 0)
			return strcpy(str, pdouble->ieee.negative ? "-0" : "0");
		else
			exponent = IEEE754_DOUBLE_EMIN; // Subnormal
	} else {
		mantissa |= (unsigned long) 1 << (IEEE754_DOUBLE_MANT_DIG - 1); // Normal
	}

	// 以unsigned long保存小数中的有效部分，处理时将10进制的指数保存
	exponent_base = 0;
	if (exponent - IEEE754_DOUBLE_MANT_DIG + 1 > 0) {
		for (; exponent - IEEE754_DOUBLE_MANT_DIG + 1 > 0; --exponent) {
			if (mantissa > ULONG_MAX / 2) {
				mantissa /= base;
				++exponent_base;
			}
			mantissa <<= 1;
		}
	} else {
		for (; exponent - IEEE754_DOUBLE_MANT_DIG + 1 < 0; ++exponent) {
			if ((mantissa & 1) && mantissa < ULONG_MAX / 10) {
				mantissa *= base;
				--exponent_base;
			}
			mantissa >>= 1;
		}
	}

	// 计算mantissa以10为底的对数
	for (exponent_base_mant = 0; mantissa >= tab_exponent_base[exponent_base_mant]; exponent_base_mant++)
		;
	exponent_base += exponent_base_mant - 1;

	// 存入字符串
	p = str;

	// 字符串：十进制科学计数法中指数部分
	if (exponent_base != 0) {
		exponent_base_sign = exponent_base < 0 ? -1 : 1;
		for ( ; exponent_base != 0; exponent_base /= 10) {
			*(p++) = exponent_base % 10 * exponent_base_sign + '0';
		}
		*(p++) = exponent_base_sign < 0 ? '-' : '+';
		*(p++) = 'E';
	}

	// 字符串：数值部分
	while (mantissa > 0 && mantissa % base == 0) // 去掉mantissa中的尾数0
		mantissa /= base;
	for ( ; mantissa > 0; mantissa /= base) {
		if (p > str && *(p - 1) >= '0' && *(p - 1) <= '9' && mantissa < base)
			*(p++) = '.';
		*(p++) = mantissa % base + '0';
	}

	// 字符串：符号
	if (pdouble->ieee.negative)
		*(p++) = '-';

	// 字符串结束，翻转
	*p = '\0';
	m_strrev(str);

	return str;
}
