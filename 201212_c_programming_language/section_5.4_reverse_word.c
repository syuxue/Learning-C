#include <stdio.h>
#include <ctype.h>
#include "m_function.h"
void reverse_word(char *pc);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char str[] = "  Yuchen Wang fuck Li Zhonghong";
	
	printf("[%s]\n",str);
	reverse_word(str);
	m_strrev(str);
	printf("[%s]\n", str);


	return 0;
}

void reverse_word(char *ps)
{
	char *pe;

	while (1) {
		//找到每个词的开始结尾
		for (; *ps == ' '; ps++) ;
		if (*ps == '\0') break;
		for (pe = ps; *pe != ' ' && *pe != '\0'; pe++) ;
		pe--;

		//反转
		int i;
		for (i = 0; ps < pe-i; ps++, i++)
			*ps ^= *(pe-i), *(pe-i) ^= *ps, *ps ^= *(pe-i);
		ps = pe + 1;
	}
}
