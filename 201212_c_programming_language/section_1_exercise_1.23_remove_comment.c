#include <stdio.h>
#define MAXSIZE		1048576
#define OUT			0
#define INCOMMENT1	1
#define INCOMMENT2	2
#define INDEFINE	3
#define INQUOTESS	4
#define INQUOTESD	5

main()
{
	int i,j,k,state;
	int c,last,last2;
	char input[MAXSIZE];
	char define[MAXSIZE];
	char comment[MAXSIZE];

	last = last2 = 0;
	state = OUT;
	for (i = 0,j = 0,k = 0; i < MAXSIZE && (c = getchar()) != EOF; ) {

		if (state == OUT) {
			if (c == '"' && last != '\\')
				state = INQUOTESD;
			else if (c == '\'' && last != '\\')
				state = INQUOTESS;
			else if (c == '#') {
				state = INDEFINE;
			}else if (c == '/' && last == '/') {
				state = INCOMMENT1;
				comment[j++] = '/';
				input[--i] = '\0';
			}else if (c == '*' && last == '/') {
				state = INCOMMENT2;
				comment[j++] = '/';
				input[--i] = '\0';
			}
		} else if (state == INQUOTESD && c == '"' && last != '\\') {
			state = OUT;
		} else if (state == INQUOTESS && c == '\'' && last != '\\') {
			state = OUT;
		} else if (state == INDEFINE && c == '\n') {
			define[k++] = c;
			state = OUT;
		} else if (state == INCOMMENT1 && c == '\n') {
			comment[j++] = c;
			state = OUT;
		} else if (state == INCOMMENT2 && last == '/' && last2 == '*') {
			comment[j++] = '\n';
			state = OUT;
		}

		//分开记录
		if (state == INCOMMENT1 || state == INCOMMENT2)
			comment[j++] = c;
		else
			input[i++] = c;
		if (state == INDEFINE)
			define[k++] = c;

		last2 = last;
		last = c;
	}

	printf("%s\n",input);
	printf("\nCOMMENT:\n%s\n",comment);
	printf("\nDEFINE:\n%s\n",define);

	return 0;
}
