#include <stdio.h>

#define	LOWER	0
#define	UPPER	300
#define	STEP	20

main()
{
	float fahr,celsius;

	/* Using While */
	fahr = LOWER;
	printf("Fahr\tCelsius\n");
	while (fahr <= UPPER) {
		celsius = (5.0/9.0) * (fahr - 32);
		printf("%4.0f\t%7.1f\n",fahr,celsius);
		fahr = fahr + STEP;
	}
	
	/* Using For */
	printf("Fahr\tCelsius\n");
	for (celsius = 0;celsius < 101;celsius += 5) {
		printf("%4.0f\t%7.1f\n",celsius / (5.0/9.0) + 32,celsius);
	}
}