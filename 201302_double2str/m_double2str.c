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

static const UINT64 tab_exponent_mant[] = {
	0x0000000000000000, /* 0 */
	0x0000000000000001, /* 1 << 0 */
	0x0000000000000002, /* 1 << 1 */
	0x0000000000000004, /* 1 << 2 */
	0x0000000000000008, /* 1 << 3 */
	0x0000000000000010, /* 1 << 4 */
	0x0000000000000020, /* 1 << 5 */
	0x0000000000000040, /* 1 << 6 */
	0x0000000000000080, /* 1 << 7 */
	0x0000000000000100, /* 1 << 8 */
	0x0000000000000200, /* 1 << 9 */
	0x0000000000000400, /* 1 << 10 */
	0x0000000000000800, /* 1 << 11 */
	0x0000000000001000, /* 1 << 12 */
	0x0000000000002000, /* 1 << 13 */
	0x0000000000004000, /* 1 << 14 */
	0x0000000000008000, /* 1 << 15 */
	0x0000000000010000, /* 1 << 16 */
	0x0000000000020000, /* 1 << 17 */
	0x0000000000040000, /* 1 << 18 */
	0x0000000000080000, /* 1 << 19 */
	0x0000000000100000, /* 1 << 20 */
	0x0000000000200000, /* 1 << 21 */
	0x0000000000400000, /* 1 << 22 */
	0x0000000000800000, /* 1 << 23 */
	0x0000000001000000, /* 1 << 24 */
	0x0000000002000000, /* 1 << 25 */
	0x0000000004000000, /* 1 << 26 */
	0x0000000008000000, /* 1 << 27 */
	0x0000000010000000, /* 1 << 28 */
	0x0000000020000000, /* 1 << 29 */
	0x0000000040000000, /* 1 << 30 */
	0x0000000080000000, /* 1 << 31 */
	0x0000000100000000, /* 1 << 32 */
	0x0000000200000000, /* 1 << 33 */
	0x0000000400000000, /* 1 << 34 */
	0x0000000800000000, /* 1 << 35 */
	0x0000001000000000, /* 1 << 36 */
	0x0000002000000000, /* 1 << 37 */
	0x0000004000000000, /* 1 << 38 */
	0x0000008000000000, /* 1 << 39 */
	0x0000010000000000, /* 1 << 40 */
	0x0000020000000000, /* 1 << 41 */
	0x0000040000000000, /* 1 << 42 */
	0x0000080000000000, /* 1 << 43 */
	0x0000100000000000, /* 1 << 44 */
	0x0000200000000000, /* 1 << 45 */
	0x0000400000000000, /* 1 << 46 */
	0x0000800000000000, /* 1 << 47 */
	0x0001000000000000, /* 1 << 48 */
	0x0002000000000000, /* 1 << 49 */
	0x0004000000000000, /* 1 << 50 */
	0x0008000000000000, /* 1 << 51 */
	0x0010000000000000  /* 1 << 52 */
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
		mantissa |= tab_exponent_mant[IEEE754_DOUBLE_MANT_DIG]; // Normal
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
