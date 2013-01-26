#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "m_function.h"

/* ****************************** Learn Function ****************************** */

void m_helloworld(void)
{
	printf("Hello, world !\n");
}

/* ****************************** Bash Function ****************************** */

void m_bash_clear(void)
{
	printf("\e[2J");
}

void m_bash_pause(const char *message)
{
	int newline;
	char *p, *tail;

	newline = 0;
	if (message) {
		for (tail = (char *)message + m_strlen(message) - 1; isspace(*tail) && tail >= message; tail--)
			;
		for (p = (char *)message; p <= tail; p++) {
			printf("%c", *p);
			if (*p == '\n')
				newline++;
		}
		printf("\n");
	} else
		printf("press Enter to continue ...\n");
	while (getchar() != '\n')
		;
	printf("\e[%dA\e[J", newline + 2);
}

/* ****************************** Input/Output Function ****************************** */

int m_getline(char *str, int maxsize)
{
	char c, *p;

	for (p = str; p - str < maxsize - 1 && (c = getchar()) != EOF && c != '\n'; p++)
		*p = c;
	if (c == '\n')
		*p++ = c;
	*p = '\0';

	return p - str;
}

/* ****************************** String Function ****************************** */

char *m_strcpy(char *to, const char *from)
{
	char *p;

	p = to;
	while ((*p++ = *from++) != '\0')
		;
	return to;
}

char *m_strncpy(char *to, const char *from, int n)
{
	char *p;
	
	for (p = to; p - to < n && (*p = *from++) != '\0'; p++)
		;
	*p = '\0';

	return to;
}

char *m_strcat(char *to, const char *from)
{
	m_strcpy(to + m_strlen(to), from);
	return to;
}

char *m_strncat(char *to, const char *from, int n)
{
	m_strncpy(to + m_strlen(to), from, n);
	return to;
}

char *m_strinsert(char *to, const char *from, int pos)
{
	int tolen, fromlen;
	char *tocpy;

	tolen = m_strlen(to);
	fromlen = m_strlen(from);
	if ((tocpy = malloc((tolen + fromlen) * sizeof *to + 1)) == NULL)
		return NULL;
	m_strncpy(tocpy, to, pos);
	m_strcpy(tocpy + pos, from);
	m_strcpy(tocpy + pos + fromlen, to + pos);
	m_strcpy(to, tocpy);
	free(tocpy);
	return to;
}

char *m_strfield(char *to, const char *from, int key)
{
	int i, inblank, inkey;
	char *p;

	for (p = to, i = 0, inblank = 1, inkey = 0; *from != '\0'; from++) {
		if (inblank && !isspace(*from)) {
			if (++i == key)
				inkey = 1;
			inblank = 0;
		} else if (!inblank && isspace(*from)) {
			if (inkey)
				break;
			inblank = 1;
		}

		if (inkey)
			*(p++) = *from;
	}
	*p = '\0';
	return to;
}

char *m_strrev(char *str)
{
	char *ps, *pe;

	ps = str;
	pe = str + m_strlen(str) - 1;
	for (; ps < pe; ps++, pe--)
		*ps ^= *pe, *pe ^= *ps, *ps ^= *pe;
	return str;
}

char *m_trim(char *str)
{
	char *head, *tail;
	for (tail = str + m_strlen(str) - 1; isspace(*tail) && tail >= str; tail--)
		;
	*(tail + 1) = '\0';
	for (head = str; isspace(*head) && *head != '\0'; head++)
		;
	if (head != str)
		m_strncpy(str, head, tail - head + 1);
	return str;
}

char *m_strtolower(char *str)
{
	char *p;

	for (p = str; *p != '\0'; p++)
		if (isupper(*p))
			*p += 32;

	return str;
}

char *m_strtoupper(char *str)
{
	char *p;

	for (p = str; *p != '\0'; p++)
		if (islower(*p))
			*p -= 32;

	return str;
}

int m_strlen(const char *str)
{
	char *p = (char *)str;

	while (*p != '\0')
		p++;
	return p - str;
}

int m_strcmp(const char *to, const char *from)
{
	for (; *to == *from && *to != '\0'; to++, from++)
		;
	return *to - *from;
}

int m_strncmp(const char *to, const char *from, int n)
{
	int i;

	for (i = 0; i < n && *to == *from && *to != '\0'; i++, to++, from++)
		;
	return i >= n ? 0 : *to - *from;
}

int m_strcasecmp(const char *to, const char *from)
{
	int t, f;

	for (; *to != '\0'; to++, from++) {
		t = islower(*to) ? *to - 32 : *to;
		f = islower(*from) ? *from - 32 : *from;
		if (t != f)
			return t - f;
	}
	return 0;
}

int m_strend(const char *str, const char *needle)
{
	return m_strcmp(str + m_strlen(str) - m_strlen(needle), needle) == 0;
}

int m_strpos(const char *str, const char *needle)
{
	int len_needle;
	char *ps;

	if (*needle == '\0')
		return -1;
	len_needle = m_strlen(needle);
	for (ps = (char *)str; *ps != '\0'; ps++)
		if (m_strncmp(ps, needle, len_needle) == 0)
			return ps - str;
	return -1;
	/* Original version on Page:69
	int i, j, k;
	for (i = 0; str[i] != '\0'; i++) {
		for (j = i, k = 0; needle[k] != '\0' && str[j] == needle[k]; j++, k++)
			;
		if (needle[k] == '\0')
			return i;
	}
	return -1;*/
}

int m_strrpos(const char *str, const char *needle)
{
	int len_needle;
	char *ps;

	if (*needle == '\0')
		return -1;
	len_needle = m_strlen(needle);
	for (ps = (char *)str + m_strlen(str) - len_needle; ps >= str ; ps--) {
		if (m_strncmp(ps, needle, len_needle) == 0)
			return ps - str;
	}
	return -1;
}

/* ****************************** Math Function ****************************** */
/* Reference */ // 自己动手写C语言浮点数转换字符串函数@http://www.cnblogs.com/maozefa/archive/2011/12/21/2295731.html
/* Reference */ // 浮点数的存储格式@http://blog.csdn.net/ganxingming/article/details/1449526

char *m_int2str(int dec, char *str, int base, int minwidth, char filler)
{
	static char basemap[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int sign;
	char *p;

	if (base < 2 || base > 36) {
		fprintf(stderr, "error: Cannot support base %d\n", base);
		exit(EXIT_FAILURE);
	}

	//Sign
	sign = dec < 0 ? -1 : 1;

	//Dec to any base
	p = str;
	do {
		*(p++) = basemap[(dec % base) * sign];
	} while (dec /= base);
	if (sign < 0)
		*(p++) = '-';
	while (p - str < minwidth)
		*(p++) = filler;
	*p = '\0';

	return m_strrev(str);
}

int m_str2int(const char *str, unsigned int base)
{
	return (int) m_str2float(str, base);
}

double m_str2float(const char *str, unsigned int base)
{
	int sign, c;
	double dec, power;

	if (base < 2 || base > 36) {
		fprintf(stderr, "error: Cannot support base %d\n", base);
		exit(EXIT_FAILURE);
	}

	power = dec = 0;
	//Sign
	sign = *str == '-' ? -1 : 1;
	if (*str == '-' || *str == '+')	str++;

	//Any base to dec
	for (; *str != '\0'; str++) {
		if (power == 0 && *str == '.') {
			power = 1;
			continue;
		} else if (isdigit(*str)) {
			c = *str - '0';
		} else if (islower(*str)) {
			c = *str - 'a' + 10;
		} else if (isupper(*str)) {
			c = *str - 'A' + 10;
		} else {
			break;
		}
		if (power > 0)
			power *= base;
		dec = (dec * base) + c;
	}

	return power > 0 ? sign * dec / power : sign * dec;
}

/* ****************************** [Experimental] ****************************** */

/* ****************************** [Deprecated] ****************************** */
#define ALLOCSIZE 1048576
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char *m_alloc(int n)
{
	if (allocp - allocbuf + n < ALLOCSIZE) {
		allocp += n;
		return allocp - n;
	} else {
		fprintf(stderr, "error: No space to alloc\n");
		return NULL;
	}
}

void m_afree(char *p)
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
		allocp = p;
}
