#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/m_function.h"

#define    F_DEFDECIMALS    6
#define    F_MAXDECIMALS    100
#define    F_MAXPRECISION   17
#define    F_CONEXPONENT    0x03ff

#define    DECIMAL_EXP(exponent)    ((((exponent - F_CONEXPONENT) * 0x4d10) >> 16) + 1)

/*
步骤
1.解析float的3部分
2.转换为不大于最大精度的整数
3.将整数转换为字符串
*/

/***************************************************************************
* 定义浮点数转换字符串结构                                                 *
***************************************************************************/
typedef struct
{
    short            exponent;    /* 指数（整数位数） */
    unsigned char    negative;    /* 负数（0正，1负）*/
    char           digits[21];    /* 十进制整数字串 */
}FloatRec;

static const unsigned long _tab0[] =
{
    0x3FF0000000000000,    /* 10**0 */
    0x4024000000000000,    /* 10**1 */
    0x4059000000000000,    /* 10**2 */
    0x408F400000000000,    /* 10**3 */
    0x40C3880000000000,    /* 10**4 */
    0x40F86A0000000000,    /* 10**5 */
    0x412E848000000000,    /* 10**6 */
    0x416312D000000000,    /* 10**7 */
    0x4197D78400000000,    /* 10**8 */
    0x41CDCD6500000000,    /* 10**9 */
    0x4202A05F20000000,    /* 10**10 */
    0x42374876E8000000,    /* 10**11 */
    0x426D1A94A2000000,    /* 10**12 */
    0x42A2309CE5400000,    /* 10**13 */
    0x42D6BCC41E900000,    /* 10**14 */
    0x430C6BF526340000,    /* 10**15 */
    0x4341C37937E08000,    /* 10**16 */
    0x4376345785D8A000,    /* 10**17 */
    0x43ABC16D674EC800,    /* 10**18 */
    0x43E158E460913D00,    /* 10**19 */
    0x4415AF1D78B58C40,    /* 10**20 */
    0x444B1AE4D6E2EF50,    /* 10**21 */
    0x4480F0CF064DD592,    /* 10**22 */
    0x44B52D02C7E14AF7,    /* 10**23 */
    0x44EA784379D99DB4,    /* 10**24 */
    0x45208B2A2C280291,    /* 10**25 */
    0x4554ADF4B7320335,    /* 10**26 */
    0x4589D971E4FE8402,    /* 10**27 */
    0x45C027E72F1F1281,    /* 10**28 */
    0x45F431E0FAE6D722,    /* 10**29 */
    0x46293E5939A08CEA,    /* 10**30 */
    0x465F8DEF8808B024     /* 10**31 */
};

static const unsigned long _tab1[] =
{
    0x4693B8B5B5056E17,    /* 10**32 */
    0x4D384F03E93FF9F5,    /* 10**64 */
    0x53DDF67562D8B363,    /* 10**96 */
    0x5A827748F9301D32,    /* 10**128 */
    0x6126C2D4256FFCC3,    /* 10**160 */
    0x67CC0E1EF1A724EB,    /* 10**192 */
    0x6E714A52DFFC679A,    /* 10**224 */
    0x75154FDD7F73BF3C,    /* 10**256 */
    0x7BBA44DF832B8D46,    /* 10**288 */
};

static const double _conPrec = 1E17;

static const unsigned long _cvttab[F_MAXPRECISION] =
{
    0x02386F26FC10000, 0x0038D7EA4C68000, 0x0005AF3107A4000, 0x00009184E72A000,
    0x00000E8D4A51000, 0x00000174876E800, 0x0000002540BE400, 0x00000003B9ACA00,
    0x000000005F5E100, 0x000000000989680, 0x0000000000F4240, 0x0000000000186A0,
    0x000000000002710, 0x0000000000003E8, 0x000000000000064, 0x00000000000000A,
    0x000000000000001
};

static void AdjFloatDigits(unsigned long value, int precision, int decimals, FloatRec *rec)
{
    int i;
    // value是F_MAXPRECISION位十进制整数，故从最高位开始转换为数字串
    for (i = 0; value; i ++)
    {
        rec->digits[i] = (char)((value / _cvttab[i]) | 0x30);
        value %= _cvttab[i];
    }
    memset(rec->digits + i, 0, F_MAXPRECISION - i);
    // 下面对数字串作精度处理
    // 如果总的精度小于0，数字串为空串，该数字转换为'0'
    if ((i = (rec->exponent + decimals)) < 0)
    {
        rec->exponent = rec->negative = rec->digits[0] = 0;
        return;
    }
    if (i > precision) i = precision;
    // 如果精度位数小于18，同时该位大于等于'5'，四舍五入
    if (i < F_MAXPRECISION && rec->digits[i] >= '5')
    {
        do
        {
            rec->digits[i --] = 0;
            rec->digits[i] ++;
        }while (i >= 0 && rec->digits[i] > '9');
        if (i < 0)
        {
            rec->digits[0] = '1';
            rec->exponent ++;
        }
    }
    // 否则，去掉数字串尾部多余的'0'
    else
    {
        if (i > F_MAXPRECISION) i = F_MAXPRECISION;
        do rec->digits[i --] = 0;
        while (i >= 0 && rec->digits[i] == '0');
        if (i < 0) rec->negative = 0;
    }
}


// 解析双精度浮点数为十进制字符串，并存入浮点数记录中
// 参数：浮点数指针，精度，小数位，浮点数记录
void FloatResolve(double *pvalue, int precision, int decimals, FloatRec *rec)
{
    int power;
    double val;

    // 63位：双精度浮点数符号位
    rec->negative = *((unsigned char *)pvalue + 7) >> 7;
    // 52-62位：双精度浮点数阶码(阶码 - 0x3ff = 二进制指数)
    rec->exponent = (*(unsigned long *)pvalue >> 52) & 0x7ff;

    // 特殊情况
    if (!rec->exponent)            // *pvalue = 0
    {
        rec->negative = rec->digits[0] = 0;
        return;
    }
    if (rec->exponent == 0x7ff)// *pvalue = nan or inf
    {
        if ((*(unsigned long *)pvalue & 0xfffffffffffff) == 0)
        {
            strcpy(rec->digits, "INF");
        }
        else
        {
            rec->exponent ++;
            rec->negative = 0;
            strcpy(rec->digits, "NAN");
        }
        return;
    }

// Why
/*M_printd((rec->exponent - 0x03ff) * 0x4d10);
M_printd( 
    (
        (
            (
                (
                    rec->exponent - 0x03ff  // 指数位偏移量 exponent - 1023
                ) * 0x4d10                  // 19728 = 0x4d10 = log(val) * 2^16 / lg2(val)
            ) >> 16
        )// + 1
    ) 
);*/

    // 阶码转换为十进制指数
    rec->exponent = DECIMAL_EXP(rec->exponent);
    
    // 0-51位：双精度浮点数尾数转换成F_MAXPRECISION位十进制浮点整数格式
    val = *pvalue;
    *((unsigned char *)&val + 7) &= 0x7f;// val = fabs(*pvalue)
    power = F_MAXPRECISION - rec->exponent;
    if (power > 0)                // if (power > 0) val *= (10**power)
    {
        val *= *(double *)&_tab0[power & 31];
        power >>= 5;            // power /= 32;
        if (power)
            val *= *(double *)&_tab1[power - 1];
    }
    else if (power < 0)            // if (power < 0) val /= (10**power)
    {
        power = -power;
        //M_printd(power);
        //M_printf(*(double *)&_tab0[power & 31]);
        val /= *(double *)&_tab0[power & 31];
        power >>= 5;            // power /= 32;
        if (power)
            val /= *(double *)&_tab1[power - 1];
    }

    // 16位十进制浮点整数四舍五入
    val += 0.5;
    if (val >= _conPrec)
    {
        val /= 10;
        rec->exponent ++;
    }

    // Why 如何将double的整数位转为int
    // Reason 此时仅仅是将val手动算出结果，并保存为unsigned long
    // 等同于 (unsigned long) val
    // 也就是说在上面的步骤中，已经将val转为整数
    /*M_printf(val);
    printf("%lu\n", 
    (
        (
            *(unsigned long *)&val & 0x000fffffffffffff     // 取出52位尾数
        ) | 0x0010000000000000                              // 将53位置为1
    ) << -(52 - (                                           // 52 - 实际偏移量，此例中向左移3位
                    (
                        *(unsigned long *)&val >> 52        // 取出11位指数位
                    ) - 0x3ff                               // 获取偏移量
                )
            )
    );*/

    // 调整并转换扩展精度浮点数尾数的整数部分rec->digits
    // 清除52-63位，加隐藏的高位，F_MAXPRECISION=17，高位超过52位，所以左移
    // 为了方便理解，将下面代码更换为直接使用C语言强制转换
    /*AdjFloatDigits(((*(unsigned long *)&val & 0x000fffffffffffff) | 0x0010000000000000) <<
        -(52 - ((*(unsigned long *)&val >> 52) - 0x3ff)), precision, decimals, rec);*/
    AdjFloatDigits((unsigned long) val, precision, decimals, rec);
}


// 输出指数字符串到buffer，返回指数字符串长度
int PutExponent(char *buffer, const FloatRec *rec)
{
    char *p = buffer;
    int e, exp = rec->digits[0]? rec->exponent - 1 : 0;
    *p ++ = rec->negative & 0x80? 'E' : 'e';
    if (exp < 0)
    {
        exp = -exp;
        *p ++ = '-';
    }
    else *p ++ = '+';
    if ((e = (exp / 1000)) != 0)
    {
        *p ++ = e + 0x30;
        exp %= 1000;
    }
    *p ++ = exp / 100 + 0x30;
    exp %= 100;
    *(unsigned short *)p = (((exp % 10) << 8) | (exp / 10)) + 0x3030;
    return (int)(p - buffer + 2);
}

// 浮点数转换为字符串。参数：字符串，浮点数
char *FloatToStr(char *str, double value)
{
    int exp;
    FloatRec rec;
    char *pd = rec.digits;
    char *ps = str;

    // 解析浮点数，并将信息保存在rec
    FloatResolve(&value, 15, 9999, &rec);
    //M_prints(rec.digits);
    // 打印负数符号
    if (rec.negative) *ps ++ = '-';
    // NAN or INF
    if (*pd > '9')                
    {
        memcpy(ps, pd, 4);
        return str;
    }
    exp = rec.exponent;
    // 如果十进制指数大于15或者小于-3，转换为指数形式
    if (exp > 15 || exp < -3)    
    {
        *ps ++ = *pd ++;
        if (*pd)
            for (*ps ++ = '.'; *pd; *ps ++ = *pd ++);
        ps += PutExponent(ps, &rec);
        *ps = 0;
        return str;
    }
    // 否则，转换为小数形式
    if (exp <= 0)
    {
        *ps ++ = '0';
        if (*pd)
        {
            for (*(ps ++) = '.'; exp < 0; *(ps ++) = '0', exp ++);
            while ((*(ps ++) = *(pd ++)));
        }
        else *ps = 0;
    }
    else
    {
        for (; exp > 0 && *pd; *ps ++ = *pd ++, exp --);
        if (*pd)
        {
            *ps ++ = '.';
            while ((*(ps ++) = *(pd ++)));
        }
        else
        {
            memset(ps, '0', exp);
            ps[exp] = 0;
        }
    }
    return str;
}

int main(int argc, char* argv[])
{
    char s[32];
    unsigned long inf = 0x7ff0000000000000;
    double v = -1.230E45;

    puts(FloatToStr(s, -10000));
    puts(FloatToStr(s, 123456789012345678));
    puts(FloatToStr(s, 1234567890.12345678));
    puts(FloatToStr(s, 340282347000000000000000000000000000000.0));
    puts(FloatToStr(s, -0.000000000000000000000000000000000000000000025));
    puts(FloatToStr(s, v));
    puts(FloatToStr(s, 0));
    puts(FloatToStr(s, *(double*)&inf));
    puts(FloatToStr(s, 5.123E-304));

    return 0;
}
