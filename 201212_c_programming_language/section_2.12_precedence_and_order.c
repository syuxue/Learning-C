#include <stdio.h>

/*
����undefined behavior
http://www.linuxsir.org/bbs/thread239136.html

����¥������û����⣬������ϸ������������Ƕ�Ӣ��ժ���������Ҫ������������еĸ���������Ԫ�ص������Ⱥ�˳�򣬶����ǹ�ϵ��prefix increment�Ķ����� 
r = (++i) + (++i); 
��������ǰ������++i�ĵ���˳���Լ�ȡi��˳���ǲ�ȷ���ġ�����undefined behavior��ȡ���ڱ���������������ȡ��i��ֵ�������2����Ȼ��ֱ�������++i������ӣ����ǽ�����ǣ� 
r = 6 
Ȼ��������Ҳ�����ȼ����һ��++i�����ܵڶ����������Ȱ�i������Ȼ���ֵ���i��Ȼ����ִ�еڶ���++i��Ȼ��ֱ�ӽ����εĽ��tmpֵ�������Ľ�������ǣ� 
r = 7 
�����ǣ������ȡi�ĵ�ַ����ֵ��ӣ����õ��� 
r = 8 
��Щ˳��C�ı�׼��û�й涨�ģ���ȡ���ڱ���������Ϊ���Ӳ�������Լ��Ż�֮���������ء� 

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
