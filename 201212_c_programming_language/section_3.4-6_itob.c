#include <stdlib.h>
#include <stdio.h>

int showdec(int dec, char str[], int base, int minwidth);
int dec2str(int dec, char str[], int base, int minwidth);

int main()
{
	char str[1024];

	//负数
	dec2str(-2212312,str,10,0);
	printf("%s\n",str);

	/*
	Int
	-2147483648	<= int <= 2147483647
	1 << 31		<= int <= ~(~0 << 31)
	10..(31)..0	<= int <= 01..(31)..1
	*/
	showdec(65535,			str,2,33);
	showdec(1 << 31,		str,2,33);
	showdec(~(~0 << 31),	str,2,33);

	return 0;
}

int showdec(int dec, char str[], int base, int minwidth)
{
	char buffer[1024];
	dec2str(dec, buffer, base, minwidth);
	printf("%s\n",buffer);
}

int dec2str(int dec, char str[], int base, int minwidth) //itob,itoa
{
	int i,j,sign;
	char basemap[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	if (2 > base || base > 36) {
		fprintf(stderr,"Cannot suppoer base %d\n",base);
		exit(EXIT_FAILURE);
	}

	//Sign
	sign = dec < 0 ? -1 : 1;

	//Convert
	i = 0;
	do {
		str[i++] = basemap[(dec % base) * sign];
	} while (dec /= base);
	if (sign < 0)
		str[i++] = '-';
	while (i < minwidth)
		str[i++] = ' ';
	str[i] = '\0';

	//Reverse
	for (j = 0,i--; j < i; i--,j++) {
		str[i] ^= str[j],str[j] ^= str[i],str[i] ^= str[j];
	}
}
