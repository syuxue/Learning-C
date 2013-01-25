#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "m_function.h"

/* ****************************** Declaration ****************************** */
#define	MAXTOKEN 100
enum	{ NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);
int tokentype;								/* type of last token */
char token[MAXTOKEN];						/* last token string */
char name[MAXTOKEN];						/* identifier name */
char datatype[MAXTOKEN];					/* data type = char, int, etc. */
char out[1000];

/* ****************************** Function ****************************** */
void debug(void)
{
	printf(M_bash_YELLOW "Type:%d Token:%s Datatype:%s\n" M_bash_default, tokentype, token, datatype);
}

/* dcl: parse a declarator */
void dcl(void)
{
	int ns;

	for (ns = 0; gettoken() == '*'; )		/* count *'s */
		ns++;
	debug();
	dirdcl();
	while (ns-- > 0)
		strcat(out, " pointer to");
}

/* dirdcl: parse a direct declarator */
void dirdcl(void)
{
	int type;

	if (tokentype == '(') {					/* ( dcl ) */
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	} else if (tokentype == NAME)			/* variable name */
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");
	
	while ((type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}

/* return next token */
int gettoken(void)
{
	int c;
	char *p = token;

	while ((c = getchar()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getchar()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetc(c, stdin);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getchar()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getchar()); )
			*p++ = c;
		*p = '\0';
		ungetc(c, stdin);
		return tokentype = NAME;
	} else
		return tokentype = c;
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	while (gettoken() != EOF) {				/* 1st token on line */
		
		//Datatype
		strcpy(datatype, token);
		out[0] = '\0';

		debug();
		dcl();								/* parse rest of line */

		//Display
		if (tokentype != '\n')
			printf("syntax error\n");
		printf("%s: %s %s\n", name, out, datatype);
	}

	return 0;
}
