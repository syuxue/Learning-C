#include <stdio.h>

/*
关于undefined behavior
http://www.linuxsir.org/bbs/thread239136.html

看来楼主还是没有理解，不妨仔细看看我上面的那段英文摘抄。这儿主要的问题是语句中的各个单独的元素的运算先后顺序，而不是关系到prefix increment的东西： 
r = (++i) + (++i); 
这个语句中前后两个++i的调用顺序以及取i的顺序是不确定的。就是undefined behavior，取决于编译器，它可以先取得i的值（这儿是2），然后分别做两次++i，再相加，于是结果就是： 
r = 6 
然而编译器也可以先计算第一个++i而不管第二个，就是先把i递增，然后把值存回i，然后再执行第二个++i，然后直接将两次的结果tmp值相加这儿的结果将会是： 
r = 7 
或者是，最后还是取i的地址处的值相加，而得到： 
r = 8 
这些顺序C的标准是没有规定的，这取决于编译器，因为这和硬件构架以及优化之类的密切相关。 

The moral is that writing code that depends on order of evaluation   
is a bad programming practice in any language. Naturally, it is  
necessary to know what tings to avoid, but if you don't know how they  
are done on various machines, you won't be tempted to take advantage  
of a particular implementation.  
*/

main()
{
	int i = 0;
	i = (++i)+(++i)+(++i);
	printf("%d",i);
}
