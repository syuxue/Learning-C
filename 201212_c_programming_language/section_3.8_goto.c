#include <stdio.h>

int main()
{
	int i,j,k,c;
	char odd[1024], even[1024];

	i = j = k = 0;
	goto getchar;

/* Goto Block */
getchar:
	if ((c = getchar()) != EOF) {
		i++;
		if (i % 2 == 0)
			goto odd;
		else
			goto even;
	} else {
		odd[j] = even[k] = '\0';
		printf("odd:  %s\neven: %s\n",odd,even);
	
		return 0;
	}

odd:
	odd[j++] = c;
	goto getchar;

even:
	even[k++] = c;
	goto getchar;

}
