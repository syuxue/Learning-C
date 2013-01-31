/*
TODO:输出任意精度
	sprintf
	取余入栈，循环处理
	将2进制分段处理
	用多个int保存mantissa
*/
#include <string.h>
#include <ieee754.h>
#include <limits.h>
#include "../include/m_function.h"

#define IEEE754_DOUBLE_EMAX			IEEE754_DOUBLE_BIAS
#define IEEE754_DOUBLE_EMIN			1 - IEEE754_DOUBLE_EMAX
#define IEEE754_DOUBLE_MANT_DIG		53
#define UINT64_MAX					ULONG_MAX

#if __WORDSIZE == 64
typedef unsigned long UINT64;
#else
typedef unsigned long long UINT64;
#endif

static const UINT64 tab_exponent_base[] = {
	0x0000000000000001, 0x000000000000000A, 0x0000000000000064, 0x00000000000003E8
	, 0x0000000000002710, 0x00000000000186A0, 0x00000000000F4240, 0x0000000000989680
	, 0x0000000005F5E100, 0x000000003B9ACA00
#if __WORDSIZE == 64
	, 0x00000002540BE400, 0x000000174876E800, 0x000000E8D4A51000, 0x000009184E72A000
	, 0x00005AF3107A4000, 0x00038D7EA4C68000, 0x002386F26FC10000, 0x016345785D8A0000
	, 0x0DE0B6B3A7640000, 0x8AC7230489E80000
#endif
};

char *m_double2str(const double dec, char *str)
{
	const int base = 10;
	
	char *p;
	union ieee754_double *pdouble;
	int exponent, exponent_base, exponent_base_mant, exponent_base_sign;
	UINT64 mantissa;

	// 将浮点数按照IEEE 754标准提取出exponent, mantissa部分
	pdouble = (union ieee754_double *) &dec;
	exponent = pdouble->ieee.exponent - IEEE754_DOUBLE_BIAS;
	mantissa = (UINT64) pdouble->ieee.mantissa0 << 32 | pdouble->ieee.mantissa1;

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
		mantissa |= (UINT64) 1 << (IEEE754_DOUBLE_MANT_DIG - 1); // Normal
	}

	// 以unsigned long保存小数中的有效部分，处理时将10进制的指数保存
	exponent_base = 0;
	if (exponent - IEEE754_DOUBLE_MANT_DIG + 1 > 0) {
		for (; exponent - IEEE754_DOUBLE_MANT_DIG + 1 > 0; --exponent) {
			if (mantissa > UINT64_MAX / 2) {
				mantissa /= base;
				++exponent_base;
			}
			mantissa <<= 1;
		}
	} else {
		for (; exponent - IEEE754_DOUBLE_MANT_DIG + 1 < 0; ++exponent) {
			if ((mantissa & 1) && mantissa < UINT64_MAX / 10) {
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
