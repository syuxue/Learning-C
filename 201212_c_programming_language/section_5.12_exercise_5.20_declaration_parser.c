#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "m_function.h"

/* ****************************** Definition ****************************** */
#define NOTATION_LENGTH					1024
#define TOKEN_LENGTH					128
#define DECLARATION_NUM					127
#define NEST_DEPTH						127

// Option
int		op_verbose = 					0;

// Token
int		token_type;
char	token[TOKEN_LENGTH];
enum	token_get_op					{TO_INIT, TO_UNGET, TO_NEXT};
enum	token_type						{TT_ERROR = -1, TT_END, TT_ALPHA, TT_DIGIT, TT_POINTER, TT_PAREN_L, TT_PAREN_R
										, TT_BRACKET_L, TT_BRACKET_R, TT_COMMA};

// Description
enum	desc_get_op						{DO_ID, DO_TYPE, DO_DESC, DO_NEST, DO_INIT};

// Parse
int		dcl_order;
int		dcl_nest;
enum	parse_result					{PR_ERROR = 0, PR_FINE};
char	*type_specifier_list[] =		{"unknown type", "void", "char", "short", "int", "long", "float", "double"
										, "signed", "unsigned"};

/* ****************************** Declaration ****************************** */
int		notation_get(char *notation);
char	*desc_get(int op, int order);
void	desc_show(void);
int		token_get(int op, char *token);
void	token_show(char *notation, char *cur, char *token);
int		token_find_type_specifier(char *token);
int		parse_declaration(void);
int		parse_type_specifier(void);
int		parse_declarator(void);
int		parse_direct_declarator(void);
int		parse_constant_expression(void);
int		parse_parameter_list(void);
void	parse_notation(char *notation);


/* ****************************** Basic Function ****************************** */
int notation_get(char *notation)
{
	int i;
	char c;

	for (i = 0; (c = getchar()) != EOF && c != ';'; i++)
		*(notation + i) = c;
	*(notation + i) = '\0';
	m_trim(notation);

	return strlen(notation);
}

char *desc_get(int op, int order)
{
	static char *desc_list[DECLARATION_NUM * 4];
	int i;

	if (op != DO_INIT) {
		return desc_list[order * 4 + op];
	} else {
		for (i = 0; i < DECLARATION_NUM * 4; i++) {
			if (!desc_list[i]) {
				if (i % 4 == DO_DESC)
					desc_list[i] = malloc(NOTATION_LENGTH * 4);
				else if (i % 4 == DO_NEST)
					desc_list[i] = malloc(1);
				else
					desc_list[i] = malloc(TOKEN_LENGTH);
			}
			*desc_list[i] = '\0';
		}
		return NULL;
	}	
}

void desc_show(void)
{
	int i, j;

	for (i = 0; i < DECLARATION_NUM && *desc_get(DO_ID, i) != '\0'; i++) {
		printf("%2d: ", i + 1);
		for (j = 0; j < *desc_get(DO_NEST, i); j++)
			printf("\t");
		printf(M_bash_GREEN "%s" M_bash_WHITE " =" "%s" M_bash_CYAN " %s\n" M_bash_default, 
			desc_get(DO_ID, i), 
			desc_get(DO_DESC, i), 
			desc_get(DO_TYPE, i)
		);
	}
}

int token_get(int op, char *token)
{
	static char *cur, *last, *notation;
	int type;
	char *token_ori = token;

	// Option
	if (op == TO_INIT) {
		cur = last = notation = token;
		return TT_END;
	} else if (op == TO_UNGET) {
		if (cur == last)
			printf(M_bash_RED "Programe warning: token_get UNGET twice\n" M_bash_default);
		cur = last;
		return TT_END;
	} else if (op == TO_NEXT) {
		last = cur;
	} else {
		printf(M_bash_RED "Programe error:  unsupport op %d\n" M_bash_default, op);
		return TT_ERROR;
	}

	// Skip space
	for (; isspace(*cur); cur++) ;

	// Get token
	if (*cur == '\0') { // End
		cur = notation;
		type = TT_END;
	} else if (isalpha(*cur) || *cur == '_') { // Alpha
		while (isalnum(*cur) || *cur == '_')
			*(token++) = *(cur++);
		type = TT_ALPHA;
	} else if (isdigit(*cur)) {
		while (isdigit(*cur))
			*(token++) = *(cur++);
		type = TT_DIGIT;
	} else if (*cur == '*') { // Pointer
		*(token++) = *(cur++), type = TT_POINTER;
	} else if (*cur == '(') { // Parenthesis
		*(token++) = *(cur++), type = TT_PAREN_L;
	} else 	if (*cur == ')') {
		*(token++) = *(cur++), type = TT_PAREN_R;
	} else if (*cur == '[') { // Brackets
		*(token++) = *(cur++), type = TT_BRACKET_L;
	} else if (*cur == ']') { 
		*(token++) = *(cur++), type = TT_BRACKET_R;
	} else if (*cur == ',') { // Comma
		*(token++) = *(cur++), type = TT_COMMA;
	} else { // Error
		printf(M_bash_RED "Parse error:  unexpected '%c'\n" M_bash_default, *cur);
		type = TT_ERROR;
	}
	*token = '\0';

	if (op_verbose && type > TT_ERROR)
		token_show(notation, cur, token_ori);

	return type;
}

void token_show(char *notation, char *cur, char *token)
{
	printf(M_bash_YELLOW "[token] " M_bash_default);

	if (*token != '\0') {
		// before token
		while (notation < cur - strlen(token))
			printf("%c", *(notation++));

		// token
		printf(M_bash_YELLOW);
		while (*(token++) != '\0')
			printf("%c", *(notation++));
		printf(M_bash_default);

		// after token
		while (*notation != '\0')
			printf("%c", *(notation++));
	} else
		printf("%s" M_bash_YELLOW "$" M_bash_default, notation);

	printf("\n");
}

int token_find_type_specifier(char *token)
{
	int i, typecount;

	typecount = sizeof type_specifier_list / sizeof token;
	for (i = 1; i < typecount; i++) {
		if (strcmp(*(type_specifier_list + i), token) == 0) {
			return i;
		}
	}
	
	return 0;	
}

/* ****************************** Parse Function ****************************** */
int parse_declaration(void)
{
	*desc_get(DO_NEST, dcl_order) = dcl_nest;
	if (parse_type_specifier() && parse_declarator())
		return PR_FINE;
	else
		return PR_ERROR;
}

int parse_type_specifier(void)
{
	int type_specifier;

	while (
		(token_type = token_get(TO_NEXT, token)) == TT_ALPHA 
		&& (type_specifier = token_find_type_specifier(token))
	) {
		strcat(desc_get(DO_TYPE, dcl_order), *(type_specifier_list + type_specifier));
		strcat(desc_get(DO_TYPE, dcl_order), " ");
	}

	if (token_type < TT_END)
		return PR_ERROR;

	if (*desc_get(DO_TYPE, dcl_order) == 0) {
		printf(M_bash_RED "Parse error:  undefined type-specifier\n" M_bash_default);
		return PR_ERROR;
	}

	m_trim(desc_get(DO_TYPE, dcl_order));
	token_get(TO_UNGET, NULL);
	return PR_FINE;
}

int parse_declarator(void)
{
	int p_count, dcl_order_ori;

	// Pointer
	for (p_count = 0; (token_type = token_get(TO_NEXT, token)) == TT_POINTER; )
		p_count++;
	if (token_type < TT_END)
		return PR_ERROR;
	token_get(TO_UNGET, NULL);

	dcl_order_ori = dcl_order;

	// Direct-declarator
	if (!parse_direct_declarator())
		return PR_ERROR;

	// Pointer write
	for (; p_count > 0; p_count--)
		strcat(desc_get(DO_DESC, dcl_order_ori), M_bash_CYAN " pointer" M_bash_default " to");

	return PR_FINE;
}

int parse_direct_declarator(void)
{
	token_type = token_get(TO_NEXT, token);
	if (token_type == TT_PAREN_L) { // (Declarator)
		if (!parse_declarator())
			return PR_ERROR;
		if (token_get(TO_NEXT, token) != TT_PAREN_R) {
			printf(M_bash_RED "Parse error:  unexpected '%c', expecting ')'\n" M_bash_default, *token);
			return PR_ERROR;
		}
	} else if (token_type == TT_ALPHA) { // Identifier
		strcpy(desc_get(DO_ID, dcl_order), token);
	} else { // Identifier Anonymous
		strcat(desc_get(DO_ID, dcl_order), "[Anonymous]");
		token_get(TO_UNGET, NULL);
		/* Use anonymous to instead a unspecify identifier error
		printf(M_bash_RED "Parse error:  unexpected '%s', expecting identifier, (declarator)\n" M_bash_default, token);
		return PR_ERROR;
		*/
	}

	while (
		(token_type = token_get(TO_NEXT, token)) == TT_BRACKET_L
		|| (token_type == TT_PAREN_L)
	) {
		if (token_type == TT_PAREN_L) { // Direct-declarator ( Parameter-type-list )
			strcat(desc_get(DO_DESC, dcl_order), M_bash_MAGENTA " function" M_bash_default " returning");
			if ((token_type = token_get(TO_NEXT, token)) > TT_END && token_type != TT_PAREN_R) {
				token_get(TO_UNGET, NULL);			
				if (++dcl_nest > NEST_DEPTH) {
					printf(M_bash_RED "Parse error:  nest depth of Declaration must be less than %d\n" M_bash_default, NEST_DEPTH);
					return PR_ERROR;
				}
				if (!parse_parameter_list())
					return PR_ERROR;
				dcl_nest--;
				if (token_get(TO_NEXT, token) != TT_PAREN_R) {
					printf(M_bash_RED "Parse error:  unexpected '%c', expecting ')'\n" M_bash_default, *token);
					return PR_ERROR;
				}
			}
		} else { // Direct-declarator [ Constant-expression(opt) 
			strcat(desc_get(DO_DESC, dcl_order), M_bash_CYAN " array");
			if (!parse_constant_expression())
				return PR_ERROR;
			if (token_get(TO_NEXT, token) != TT_BRACKET_R) {
				printf(M_bash_RED "Parse error:  unexpected '%c', expecting ']'\n" M_bash_default, *token);
				return PR_ERROR;
			}
			strcat(desc_get(DO_DESC, dcl_order), M_bash_default " of");
		}
	}

	if (token_type < TT_END)
		return PR_ERROR;

	token_get(TO_UNGET, NULL);
	return PR_FINE;
}

int parse_parameter_list(void)
{
	do {
		if (++dcl_order >= DECLARATION_NUM) {
			printf(M_bash_RED "Parse error:  number of Declaration must be less than %d\n" M_bash_default, DECLARATION_NUM);
			return PR_ERROR;
		}
		if (!parse_declaration())
			return PR_ERROR;
	} while (token_get(TO_NEXT, token) == TT_COMMA);

	if (token_type < TT_END)
		return PR_ERROR;

	token_get(TO_UNGET, NULL);
	return PR_FINE;
}

int parse_constant_expression(void)
{
	strcat(desc_get(DO_DESC, dcl_order), "[");
	while ((token_type = token_get(TO_NEXT, token)) > TT_END && token_type != TT_BRACKET_R) 
		strcat(desc_get(DO_DESC, dcl_order), token);
	strcat(desc_get(DO_DESC, dcl_order), "]");

	if (token_type < TT_END)
		return PR_ERROR;

	token_get(TO_UNGET, NULL);
	return PR_FINE;
}

/* ****************************** Wrap Function ****************************** */
void parse_notation(char *notation)
{
		// Comment
		if (*notation == '/' && *(notation + 1) == '/')
			return;

		// Init
		dcl_order = dcl_nest = 0;
		desc_get(DO_INIT, 0);
		token_get(TO_INIT, notation);	

		// Display Notation
		printf(M_bash_YELLOW "%s;\n" M_bash_default, notation);

		// Parse Declaration
		if (parse_declaration()) {
			desc_show();
		}
		printf("\n");
}

/* ****************************** Main ****************************** */
int main(int argc, char *argv[])
{
	char notation[NOTATION_LENGTH];

	// Argument
	while (--argc > 0) {
		argv++;
		if (strcmp(*argv, "-v") == 0)
			op_verbose = 1;
	}


	while (notation_get(notation) > 0) {

		// Traverse
		//while (token_get(TO_NEXT, token) > TT_END) ;

		// Parse
		parse_notation(notation);

	}

	return 0;
}
