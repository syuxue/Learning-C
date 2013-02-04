#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define SIZE 10

/* ****************************** Function Declaration ****************************** */
void array_init(int array[], int size);
void array_shuffle(int array[], int size);
void array_show(int array[], int size);
void sort_select(int array[], int size);
void sort_shell(int array[], int size);
void sort_quick(int array[], int left, int right);
void array_swap(int array[], int i, int j);

/* ****************************** Main ****************************** */
int main()
{
	int array[SIZE];

	//Init
	printf("\nInit\n");
	array_init(array, SIZE);
	array_show(array, SIZE);

	//Sort Select
	printf("\nSort Select\n");
	array_shuffle(array, SIZE);
	sort_select(array, SIZE);

	//Sort Select
	printf("\nSort Shell\n");
	array_shuffle(array, SIZE);
	sort_shell(array, SIZE);

	//Sort Quick
	printf("\nSort Quick\n");
	array_shuffle(array, SIZE);
	array_show(array, SIZE);
	sort_quick(array, 0, SIZE - 1);

	return 0;
}

/* ****************************** Sort Function Definition ****************************** */
void sort_select(int array[], int size)
{
	array_show(array, SIZE);
	int i,j,min;

	for (i = 0; i < size - 1; i++) {
		min = i;
		for (j = i + 1; j < size; j++) {
			if (array[j] < array[min]) {
				min = j;
			}
		}
		if (min != i) {
			j = array[i], array[i] = array[min], array[min] = j;
			array_show(array, SIZE);
		}
	}
}

void sort_shell(int array[], int size)
{
	array_show(array, SIZE);
	int gap, i, j, temp;

	for (gap = size/2; gap >0; gap /= 2) {
		for (i = gap; i < size; i++) {
			for (j = i - gap; j >= 0 && array[j] > array[j + gap]; j -= gap) {
				temp = array[j], array[j] = array[j + gap], array[j + gap] = temp;
				array_show(array, SIZE);
			}
		}
	}
}

void sort_quick(int array[], int left, int right)
{
	int i,base;

	if (left >= right) return;

	//array_swap(array, left, (left + right) / 2);
	base = left;
	for (i = left + 1; i <= right; i++) {
		if (array[i] < array[base]) {
			array_swap(array, i, ++left);
		}
	}
	array_swap(array, base, left);
	array_show(array, SIZE);

	sort_quick(array, base, left);
	sort_quick(array, left + 1, right);
}

/* ****************************** Array Function Definition ****************************** */
void array_init(int array[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		array[i] = i+1;
}

void array_shuffle(int array[], int size)
{
	int i,j,temp;

	i = size;
	srand((unsigned)time(0));
	while ( --i) {
		j = rand() % (i + 1);
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

void array_show(int array[], int size)
{
	int i;

	printf("[");
	for (i = 0; i < size; i++) {
		if (i == 0)
			printf("%d", array[i]);
		else
			printf(",%d", array[i]);

	}
	printf("]\n");
}

void array_swap(int array[], int i, int j)
{
	int tmp;
	tmp = array[i];
	array[i] = array[j];
	array[j] = tmp;
}
