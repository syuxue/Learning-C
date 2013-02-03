#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "include/m_function.h"

void minprintf(char *fmt, ...)
{
	va_list ap;
	char *p;
	int leftadj, field_width, precision;

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {

		// Non-conversion
		if (*p != '%') {
			putchar(*p);
			continue;
		}

		// %%
		if (*(++p) == '%') {
			putchar('%');
			continue;
		}

		// Left adjustment
		leftadj = 0;
		if (*p == '-') {
			leftadj = 1;
			p++;
		}

		// Field width
		for (field_width = 0; isdigit(*p); p++) {
			field_width *= 10;
			field_width += *p - '0';
		}

		// Precision
		precision = 0;
		if (*p == '.') {
			p++;
			for (; isdigit(*p); p++) {
				precision *= 10;
				precision += *p - '0';
			}
		}

		// Output
		if (*p == 'd') {
			printf("%*d", field_width, va_arg(ap, int));
		} else if (*p == 'f') {
			printf("%*.*f", field_width, precision, va_arg(ap, double));
		}

	}
	va_end(ap);

}

// Main
int main(int argc, char *argv[])
{
	minprintf("%%%100.30f", 123.2);

	return 0;
}
