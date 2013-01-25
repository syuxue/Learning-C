#include <stdio.h>
#include <limits.h>

#define LONGINT 21474836480000000U
enum months {JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOC, DEC};

main()
{
	int i;
	
	printf("%c",'\xb');//Tab
	//printf("%c",'\007');//Bell
	printf("%c",'\x6d');//char:m
	printf("\n");
	
	//用int存字符串
	int stringi[3];
	stringi[0] = 's';
	stringi[1] = 'b';
	stringi[2] = '\0';
	for (i = 0; i < 3; i++) 
		printf("%c",stringi[i]);
	printf("%s",stringi);//只能输出第一个字符
	printf("\n");

	//用char存字符串
	char stringc[3];
	stringc[0] = 's';
	stringc[1] = 'b';
	stringc[2] = '\0';
	for (i = 0; i < 3; i++) 
		printf("%c",stringc[i]);
	printf("%s",stringc);
	printf("\n");
	
	//Enum
	printf("%d,%d",JAN,FEB);
	printf("\n");
	
	//Const
	const double pi = 3.141592653574;
	const char warning[] = "warning: ";
	printf("%f,%s",pi,warning);
	printf("\n");
	
	return 0;
}

