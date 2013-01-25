#include <stdio.h>

main()
{
	int i,j,c;
	int last = -1;
	int inword = 0;

	double count,count_line,count_blank,count_word;
	count = count_line = count_blank = count_word = 0;
	int count_char[128];
	for(i = 0;i < 128;i++) count_char[i] = 0;

	while((c = getchar()) != EOF){

		//Replacing each string of one or more blanks by a single blank
		if(last == ' ' && c == ' ')
			continue;

		//Count for \n and blank
		count++;
		if(c == '\n')
			count_line++;
		else if(c == ' ')
			count_blank++;

		//Count for char
		if(c >= 0 && c <= 128) count_char[c]++;

		//Count for word
		if(c < 'A' || (c > 'Z' && c < 'a') || c > 'z'){
			if(inword > 0)
				printf("]");
			inword = 0;
		}else if(inword == 0){
			printf("[");
			count_word++;
			inword = 1;
		}

		last = c;

		//Make tabs,backspace visible in an unambiguous way
		if(c == '\b')
			printf("\\b");
		else if(c == '\t')
			printf("\\t");
		else
			putchar(c);
	}

	printf("Char:  %.0f\n",count);
	printf("Word:  %.0f\n",count_word);
	printf("Line:  %.0f\n",count_line);
	printf("Blank: %.0f\n",count_blank);
	printf("Digit: ");for(i = 48;i < 58;i++) printf("%d ",count_char[i]);printf("\n");/* 只有一个printf在for循环中 */
	
	if(0){
		for(i = 32;i < 128;i++){
			printf("%c ",i);
			for(j = 0;j < count_char[i];j++) printf("|");
			printf("\n");
		}
	}
	if(1){
		//Border Top
		for(i = 0;i < 98;i++) printf("--");
		printf("\n");
		
		//Find Height-Max
		int count_char_max = 0;
		for(i = 0;i < 128;i++){
			if(count_char[i] > count_char_max)
				count_char_max = count_char[i];
		}
		count_char_max++;
		
		//Display Bar
		for(i = count_char_max;i >= 0;i--){
			printf("| ");
			for(j = 32;j < 128;j++){
				if(count_char[j] < i || i == 0)
					printf("  ");
				else
					printf("* ");
				
			}
			printf(" |\n");
		}
		
		//Display Char
		printf("| ");
		for(i = 32;i < 128;i++){
			printf("%c ",i);
		}
		printf(" |");
		printf("\n");

		//Border Bottom
		for(i = 0;i < 98;i++) printf("--");
		printf("\n");
	}
	printf("\n");
}
