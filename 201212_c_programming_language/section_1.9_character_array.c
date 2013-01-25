#include <stdio.h>
#define MAXSIZE 1024

char longest[MAXSIZE];

int readline(char input[],int size);
void saveline(char from[]);
void showline(char line[]);
void reverse(char line[]);

//PHP
int php_strlen(char text[]);
void php_strrev(char text[]);

main()
{
	int i;
	int len_buff,len_cur,len_max;
	int line_cur,line_max;
	char buff[MAXSIZE];
	//如果变量定义与调用的函数在一个文件中，则extern说明并非必须
	//extern char longest[];

	//初始化buff
	for (i = 0; i < MAXSIZE; i++)
		buff[i] = '\0';

	line_cur = len_cur = len_max = 0;
	while ((len_buff = readline(buff,MAXSIZE)) != 0) {
		if (buff[len_buff - 1] == '\n') {
			line_cur++;
			len_cur += len_buff;
			if (len_cur > len_max) {
				len_max = len_cur;
				line_max = line_cur;
				saveline(buff);
			}
			//printf("%d,%d: %s",line_cur,len_cur,buff);
			len_cur = 0;
		} else {
			//printf("%s",buff);
			len_cur += len_buff;
		}
	}

	if (len_max > 0) {
		printf("%d,%d: %s",line_max,len_max,longest);
	}

	return 0;
}

int readline(char input[],int size)
{
	int i,c;
	//读取输入直到EOF或换行
	for (i = 0; i < size && (c = getchar()) != EOF && c != '\n'; ++i)
		input[i] = c;
	//c should in ( EOF, \n )
	//i should in (   0,  i )

	if (i < size) {
		//将角标移动到末尾空格中最左边的空格
		for(; i > 0 && (input[i-1] == '\t' || input[i-1] == ' '); i--)
			;
	}

	//将换行符置于当前位置，并把角标推后1位
	if (c == '\n') {
		input[i] = c;
		i++;
	}

	//结束符
	input[i] = '\0';
	
	//反转
	//reverse(input);

	return i;
}

void saveline(char from[])
{
	int i;
	extern char longest[];
	i = 0;
	while ((longest[i] = from[i]) != '\0')
		i++;
}

void showline(char line[])
{
	int i;
	printf("[");
	for (i = 0; line[i] != '\0'; i++)
		if (line[i] == '\n')
			printf("\\n");
		else
			printf("%c",line[i]);
	printf("]\n");
}

void reverse(char line[])
{
	int len;
	len = php_strlen(line);
	if (line[len - 1] == '\n') {
		line[len - 1] = '\0';
		php_strrev(line);
		line[len - 1] = '\n';
		line[len] = '\0';
	} else {
		php_strrev(line);
	}
}

int php_strlen(char text[])
{
	int i;
	
	i = 0;
	while (text[i] != '\0') 
		++i;
	return i;
}

void php_strrev(char text[])
{
	int i,len,mid;
	len = php_strlen(text);
	mid = (len / 2);

	for (i = 0; i < mid; i++) {
		text[i] ^=text[len - i - 1];
		text[len - i - 1] ^= text[i];
		text[i] ^= text[len - i - 1];
	}
}
//PHP中strrev的原型
/* {{{ proto string strrev(string str)
   Reverse a string 
PHP_FUNCTION(strrev)
{
    char *str;
    char *e, *n, *p;
    int  str_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE) {
        return;
    }

    n = emalloc(str_len+1);
    p = n;

    e = str + str_len;

    while (--e>=str) {
        *p++ = *e;
    }

    *p = '\0';

    RETVAL_STRINGL(n, str_len, 0);
}
 }}} */
