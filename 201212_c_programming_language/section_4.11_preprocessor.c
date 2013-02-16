/* ********** File Inclusion ********** */
#include <stdio.h>
#include "m_function.h"
#include "m_function.h"

/* ********** Macro Substitution ********** */
#define string "Hahahaha"						//as String
#define string2 "Xixixixi"						//as String

#define forever for (;;)						//as Function
#define max(A, B) ((A) > (B) ? (A) : (B))		//as Function

#define printd(val)							\
		printf(#val " = %g", (double)val);	\
		printf("\n")

#define concatenation(front, back)																\
		printf("********** Macro Concatenation **********\n");									\
		printf("#front = " #front ", " #front " = %d\n", front);								\
		printf("#back =  " #back  ", " #back  " = %d\n", back);									\
		printf("front ## back = " #front #back "," #front #back " = %d" "\n", front ## back);	\
		printf("********** Macro Concatenation End **********\n");								\
		// ## = #front#back

/*
Q:如何用宏实现类似PHP中的$$val,$$func()可变变量
A:不可能，
$$val ~= eval(char * "val")，即某变量通过char *idname表示
1.宏的展开是在preprocess阶段处理的，原理是将#define NAME MARCO中，NAME的部分替换为MARCO
2.预处理时程序未进行编译，无法确定宏参数
由于1，可以得出，一切通过宏实现的内容，也可以不通过宏实现
$$val这种用变量$val表示另一个变量的形式，在C语言中是通过指针实现的，无法通过字符串形式的变量名实现
NOTE:
#define EVAL(name) name
int speed;
//So speed == EVAL(speed)
char name = "speed";
//EVAL(name) == "speed"
关于预处理，参见A.12 Preprocessing
*/
#define call_func(func, argv) func(argv)
#define call_val(val) val

//Exercise 4-14. Define a macro swap(t,x,y) that interchanges two arguments of type t.(Block structure will help.)
#define swap(t, x, y) {t tmp;tmp = x, x = y, y = tmp;}

#define console_title(message) printf("********** %s **********\n",message);

#if SYSTEM == SYSV
	#define HDR "sysv.h"
#elif SYSTEM == BSD
	#define HDR "bsd.h"
#elif SYSTEM == MSDOS
	#define HDR "msdos.h"
#else
	#define HDR "default.h"
#endif

#ifndef DD
	#define DD "2"
#endif
#ifndef DD
	#define DD "1"
#endif

int ret_number(void);

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	printf(HDR "\n");
	printf(DD "\n");
	
	//"forever" 单纯替换为 "for (;;)"
	/*forever {
		printf("c");
	}*/

	printf("wo cao" "yuan lai hai ke yi zhe yang" "\n");

	//字符串
	printf("%s" "\n",string);
	#undef string //取消宏定义
	char string[] = "houhou";
	printf("%s\n",string);
	printf("%s%s\n",string,string2);

	//函数
	printf("%d\n",max(10,90));
	printf("%d\n",max(ret_number(),90)); //传入的函数会被call两次，如果传入的是i++，那么结果就会受到影响

	/*
	getchar的来历，在当前版本下不是通过define弄得
	#define putchar(c) putc((c), stdout)
	#define getchar() getc(stdin)
	*/
	#undef getchar
	#define getchar() getc(stdin)
	//printf("%c\n",getchar());

	//作为函数时穿入的并非参数
	int asd = 10;
	printd(asd);
	printd(10 / 1.23);

	int i,i1,i2,i3,i4,i5,i6,i7,i8,i9;
	i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5, i6 = 6, i7 = 7, i8 = 8, i9 = 9;
	for (i = 0; i < 10; i++)
		;//printf("%d\n",paste(i, 1));

	//##
	concatenation(i,1);
	printf("\n");

	//Call function
	call_func(printf,"hello !\n");
	printf(call_val("i") "\n");

	//Swap
	console_title("Swap");
	{
		//Swap Int
		printf("%d,%d\n",i1,i9);
		swap(int, i1, i9);
		printf("%d,%d\n",i1,i9);

		//Swap Char
		char c1 = 'a', c2 = 'z';
		printf("%c,%c\n",c1,c2);
		swap(char, c1, c2);
		printf("%c,%c\n",c1,c2);

		//Swap Double
		double d1 = 1.12, d2 = 8.88;
		printf("%g,%g\n",d1,d2);
		swap(double, d1, d2);
		printf("%g,%g\n",d1,d2);
		
		//Swap Array
		char str[] = "ab";
		printf("%s\n",str);
		swap(char, str[0], str[1]);
		printf("%s\n",str);
	}

	return 0;
}

int ret_number(void)
{
	printf("some where call ret_number\n");
	return 100;
}
