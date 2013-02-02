#include <stdio.h>
#include "include/m_function.h"

static const char *tab_char[] = {
	"\\0",
	"",			// start of heading
	"",			// start of text
	"",			// end of text
	"",			// end of transmission
	"",			// enquiry
	"",			// acknowledge
	"\\a",		// bell
	"\\b",		// backspace
	"\\t",		// horizontal tab
	"\\n",		// new line
	"\\v",		// vertical tab
	"\\f",		// form feed
	"\\r",		// carriage ret
	"",			// shift out
	"",			// shift in
	"",			// data link escape
	"",			// device control 1
	"",			// device control 2
	"",			// device control 3
	"",			// device control 4
	"",			// negative ack.
	"",			// synchronous idle
	"",			// end of trans. blk
	"",			// cancel
	"",			// end of medium
	"",			// substitute
	"",			// escape
	"",			// file separator
	"",			// group separator
	"",			// record separator
	"",			// unit separator
};

// Main
int main(int argc, char *argv[])
{
	int c;
	unsigned char tab_char_length;
	tab_char_length = sizeof tab_char / sizeof tab_char[0];

	while ((c = getchar()) != EOF) {
		
		if (c < tab_char_length) // Non-Graphic
			printf(M_bash_yellow "%s" M_bash_default "%c", tab_char[(int) c], c);
		else if (c < 128) // Alpha
			putchar(c);
		else {
			if ((c & 0xE0) == 0xE0) { // UTF-8
				printf(M_bash_yellow "(UTF8" M_bash_default);
				do {
					putchar(c);
					putchar(getchar());
					putchar(getchar());
				} while (((c = getchar()) & 0xE0) == 0xE0);
				printf(M_bash_yellow ")" M_bash_default);
				ungetc(c, stdin);
			} else {
				printf(M_bash_yellow "(GBK" M_bash_default);
				do {
					putchar(c);
					putchar(getchar());
				} while ((c = getchar()) > 127);
				printf(M_bash_yellow ")" M_bash_default);
				ungetc(c, stdin);
			}
		}

	}

	return 0;
}
