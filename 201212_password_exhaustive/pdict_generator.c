#include <stdio.h>
#include <string.h>
#include <math.h>
#include "m_function.h"
void exhaustive(int length, char *ctable);

int main(int argc, char *argv[])
{
	int length;
	char ctable[64];

	//������
	if (argc != 3) {
		printf("********** Password Dictionary Generator **********\n");
		printf("error: Wrong aguments\n");
		printf("usage: %s {length} {chartable}\n", argv[0]);
		return 1;
	}
	length = m_str2int(argv[1], 10);
	strcpy(ctable, argv[2]);

	//����
	exhaustive(length, ctable);

	return 0;
}

void exhaustive(int length, char *ctable)
{
	int i, base, pos[length];

	//��ʼ������
	base = strlen(ctable);
	for (i = 0; i < length; i++)
		pos[i] = 0;

	while(1) {
		//�����ǰ�ַ�
		for (i = length - 1; i >= 0; i--)
			printf("%c", *(ctable + pos[i]));
		printf("\n");

		//��λ
		pos[0]++;
		for (i = 0; i < length; i++) {
			if (pos[i] >= base) {
				if (i == length - 1) return;
				pos[i] = 0;
				pos[i + 1]++;
			}
		}
	}
}
