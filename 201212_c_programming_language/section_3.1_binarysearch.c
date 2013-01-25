#include <stdio.h>

int binsearch(int x, int v[], int n);
int binsearch_2(int x, int v[], int n);
int binsearch_3(int x, int v[], int n);

int main()
{
	int size;
	int array[] = {1,2,3,4,5,6,7,8,9,10,11};
	size = sizeof(array) / 4;
	
	int i,j;
	if(1){
		for (i = 0; i < size; i++)
			printf("%d,",binsearch_2(array[i],array,size));
		printf("\n");
	} else {
		for (j = 0; j < 10000000; j++) {
			for (i = 0; i < size; i++){
				binsearch_3(array[i],array,size);
			}
		}
	}
	
	return 0;
}

int binsearch(int x, int v[], int n)
{
	int low = 0;
	int high = n - 1;
	int mid;

	while (low <= high) {
		mid = (high + low) / 2;
		//printf("%d-%d-%d\n",low,mid,high);
		if (x > v[mid]) {
			low = mid + 1;
		} else if (x < v[mid]) {
			high = mid - 1;
		} else {
			return mid;
		}
	}

	return -1;
}

//FIXME:测试中说再写一个while中只test一次的方法，然后measure效率，他娘的measure出来可能更多
int binsearch_2(int x, int v[], int n)
{
	int low = 0;
	int high = n - 1;
	int mid;

	mid = (high + low) / 2;
	while (low <= high && x != v[mid]) {
		//printf("%d-%d-%d\n",low,mid,high);
		if (x > v[mid]) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
		mid = (high + low) / 2;
	}
	
	if (x == v[mid])
		return mid;
	else
		return -1;
}

//结果是错的
//http://code.google.com/p/k-and-r-exercises/source/browse/3-1.c
int binsearch_3(int x, int v[], int n)
{
	int low = 0;
	int high = n - 1;
	int mid;

	while (low <= high) {
		mid = (high + low) / 2;
		
		if (x < v[mid]) {
			high = mid - 1;
		} else {
			low = mid + 1;			
		}
	}
	
	if (x == v[mid])
		return mid;
	else
		return -1;
}
