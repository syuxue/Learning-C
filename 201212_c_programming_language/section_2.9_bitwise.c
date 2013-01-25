#include <stdio.h>
#include <math.h>
#define MAXBIT	32

void showbin(int dec);
int dec2bin(int dec,char bin[]);
int bitcount(int dec);
unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);
unsigned invert(unsigned x, int p, int n);

main()
{
	int shift = 1024;
	int option = 256;

	//Timetest for bitcount
	if(0) {
		int i;
		for (i = 1; i < 1000000; i++) {
			bitcount(i);
		}
		return 0;
	}

	//OR [set on]
	printf("OR\n");
	printf("before: ");
	showbin(option);
	printf("OR 4:   ");
	showbin(4);
	printf("after:  ");
	option |= 4;
	showbin(option);
	printf("\n");

	//AND [check]
	printf("AND\n");
	printf("AND 2:  ");
	showbin(option & 2);
	printf("AND 4:  ");
	showbin(option & 4);
	printf("\n");

	//XOR [difference]
	printf("XOR\n");
	printf("XOR 0:  ");
	showbin(0 ^ option);
	printf("XOR 511:");
	showbin(511 ^ option);
	printf("\n");

	//NOT
	printf("NOT\nSet The Last 4 Bits To Zero\n");
	printf("& ~15:  ");
	showbin(option & ~15);
	printf("\n");

	//Shift
	printf("Shift\n");
	printf("Ori:    ");
	showbin(shift);
	printf("<< 5:   ");
	showbin(shift << 5);
	printf(">> 3:   ");
	showbin(shift >> 3);
	printf("\n");

	//getbits
	printf("Getbits\n");
	showbin(getbits(65500,7,7));

	//setbits
	printf("Setbits\n");
	showbin(setbits(65536,5,3,1));
	showbin(setbits(131071,5,3,0));

	//invert
	printf("Invert\n");
	showbin(65500);
	showbin(invert(65500,10,10));

	return 0;
}

unsigned invert(unsigned x, int p, int n)
{
	return (x | ~(~0 << n) << (p - n)) & ~(((x >> (p - n)) & ~(~0 << n)) << (p - n));
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	if (y)
		return x | ~(~0 << n) << (p - n);
	else
		return x & ~(~(~0 << n) << (p - n));
}

unsigned getbits(unsigned x, int p, int n)
{
	return (x >> (p - n)) & ~(~0 << n);
}

void showbin(int dec)
{
	char bin[MAXBIT];
	dec2bin(dec,bin);
	printf("%s\n",bin);
}

int dec2bin(int dec,char bin[])
{
	int i,curpow,count;
	
	i = count = bitcount(dec);
	while ( i >= 0 ) {
		if (dec > 0) {
			curpow = (int)pow(2,i);
			bin[count - i] = '0' + (int)(dec / curpow);
			dec %= curpow;
		} else {
			bin[count - i] = '0';
		}
		i--;
	}
	bin[count + 1] = '\0';
}

int bitcount(int dec)
{
	int count;
	
	//1m:0.108;	10m:1.235s
	for (count = 0; dec != 0; dec >>= 1, count++) ;
	//1m:3.139;	10m:38.9s
	//for (count = 0; pow(2,count) <= dec; count++) ;
	
	return count;
}
