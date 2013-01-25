#include <stdio.h>
#include "m_function.h"
void printd(int n);
int m_int2str_r(int dec, char str[], int pos);
void m_strrev2(char str[]);
int m_strrev_r(char str[], int pe);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char str[1024];
	
	//Printd
	printd(-9992);
	printf("\n");
	
	m_int2str(-9992,str,10,0);
	printf("%s\n",str);
	
	m_int2str_r(-9992,str,0);
	printf("%s\n",str);
	
	char str2[] = "hello, world !";
	m_strrev(str2);
	printf("%s\n\n",str2);

	char str3[] = "0123456789";
	m_strrev2(str3);
	printf("\n");
	printf("%s\n",str3);
	
	return 0;
}

void m_strrev2(char str[])
{
	m_strrev_r(str, 0);
}

int m_strrev_r(char str[], int pe)
{
	int ps,tmp;

	if (str[pe] == '\0') return 0;
	ps = m_strrev_r(str, pe + 1);
	if (ps < pe) {
		tmp = str[pe];
		str[pe] = str[ps];
		str[ps++] = tmp;
	}
	return ps;
}

void printd(int n)
{
	if (n < 0) {
		putchar('-');
		n = -n;
	}
	if (n > 9)
		printd(n / 10);
	putchar((n % 10) + '0');
}

int m_int2str_r(int dec, char str[], int pos)
{
	if (dec < 0) {
		str[pos++] = '-';
		dec = -dec;
	}
	
	if (dec > 9)
		pos = m_int2str_r(dec / 10, str, pos);
	str[pos++] = (dec % 10) + '0';
	str[pos] = '\0';
	return pos;
}
