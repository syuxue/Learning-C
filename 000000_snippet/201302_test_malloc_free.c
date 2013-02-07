#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define TIMES 100
#define SPEED 50000

int main(int argc, char *argv[])
{
	int i;
	char *p[TIMES];

	for (i = 0; i < TIMES; i++) {
		p[i] = malloc(1048576);
		usleep(SPEED);
	}
	fprintf(stderr, "alloc done\n");

	for (i = 0; i < TIMES; i++) {
		free(p[i]);
		usleep(SPEED);
	}
	fprintf(stderr, "free done\n");

	sleep(1000);
	return 1;
}
