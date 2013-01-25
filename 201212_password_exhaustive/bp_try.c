#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"
#include "m_function.h"
void bp_md5(char *input, char *md5str);
void m_md5(char *input, char *md5str);


int main()
{
	char text[32], md5str[32];

	while (m_getline(text, 1024) > 0) {
		bp_md5(text, md5str);
		printf("%s\n", text);
	}
	
	return 0;
}

void bp_md5(char *input, char *md5str)
{
	int i;
	m_strcpy(md5str, input);
	for (i = 0; i < 10; i++)
		m_md5(md5str, md5str);
}

void m_md5(char *input, char *md5str)
{
	int i;
	unsigned char output[16];

	md5((unsigned char *)input, strlen(input), output);

	for (i = 0; i < 16; i++)
		m_int2str(output[i], (md5str + i*2), 16, 2, '0');
}
