#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "include/m_function.h"

int minscanf(char *fmt, ...)
{
	va_list ap;
	char *p, c;
	int suppress, number, ival;
	float fval;

	va_start(ap, fmt);
	for (number = 0, p = fmt; *p; p++) {

		// 如果format为%或space，则将输入移至下一个非空的位置
		if (*p == '%' || isspace(*p)) {
			while (isspace(*p))
				p++;
			while ((c = getchar()) != EOF && isspace(c))
				;
			ungetc(c, stdin);
		}

		if (*p == '%') {
			if (*++p == '*') {
				suppress = 1;
				p++;
			} else
				suppress = 0;

			switch (*p) {
			case 'd':
				// 将输入简单转为int，未处理符号
				ival = 0;
				while ((c = getchar()) != EOF && isdigit(c)) {
					ival *= 10;
					ival += c - '0';
				}
				ungetc(c, stdin);
				if (!suppress) {
					++number;
					*(int *) va_arg(ap, int *) = ival;
				}
				break;
			case 'f':
				scanf("%f", &fval);
				if (!suppress) {
					++number;
					*(float *) va_arg(ap, float *) = fval;
				}
				break;
			default:
				M_print("%c", *p);
				break;
			}
		} else if (!isspace(*p)) {
			if (getchar() != *p)
				return number;
		}

	}
	va_end(ap);

	return number;
}

// Main
int main(int argc, char *argv[])
{
	int number, year, day;
	float month;
	char format[] = "year:%*d month:%f %d";

	//number = scanf(format, /*&year,*/ &month, &day);
	number = minscanf(format, /*&year,*/ &month, &day);
	M_print("%d", number);
	M_print("%d", year);
	M_print("%g", month);
	M_print("%d", day);

	return 0;
}
